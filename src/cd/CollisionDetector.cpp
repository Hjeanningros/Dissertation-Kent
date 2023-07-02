#include <vector>
#include "Collision.cpp"
#include "Aircraft.cpp"
#include "Constants.cpp"
#include "Vector2D.cpp"
#include "Vector3D.cpp"
#include "RedBlackTree.cpp"
#include "Motion.cpp"

namespace CD {
    class CollisionDetector {

        private:

            RedBlackTree<CallSign, Vector3D> _state;
            Vector2D _horizontal = Vector2D(Constants::GOOD_VOXEL_SIZE, 0.0);
            Vector2D _vertical = Vector2D(0.0, Constants::GOOD_VOXEL_SIZE);

            static bool isInVoxel(Vector2D voxel, Motion motion) {
                if (voxel._x > Constants::MAX_X ||
                    voxel._x < Constants::MIN_X ||
                    voxel._y > Constants::MAX_Y ||
                    voxel._y < Constants::MIN_Y)
                return false;

                Vector3D init = motion._posOne;
                Vector3D fin = motion._posTwo;

                double v_s = Constants::GOOD_VOXEL_SIZE;
                double r = Constants::PROXIMITY_RADIUS / 2.0;

                double v_x = voxel._x;
                double x0 = init._x;
                double xv = fin._x - init._x;

                double v_y = voxel._y;
                double y0 = init._y;
                double yv = fin._y - init._y;

                double low_x = (v_x - r - x0) / xv;
                double high_x = (v_x + v_s + r - x0) / xv;

                if (xv < 0.0) {
                    double tmp = low_x;
                    low_x = high_x;
                    high_x = tmp;
                }

                double low_y  = (v_y - r - y0) / yv;
                double high_y = (v_y + v_s + r - y0) / yv;

                if (yv < 0.0) {
                    double tmp = low_y;
                    low_y = high_y;
                    high_y = tmp;
                }

                return (((xv == 0.0 && v_x <= x0 + r && x0 - r <= v_x + v_s) /* no motion in x */ ||
                        (low_x <= 1.0 && 1.0 <= high_x) || (low_x <= 0.0 && 0.0 <= high_x) ||
                        (0.0 <= low_x && high_x <= 1.0)) &&
                        ((yv == 0.0 && v_y <= y0 + r && y0 - r <= v_y + v_s) /* no motion in y */ ||
                        ((low_y <= 1.0 && 1.0 <= high_y) || (low_y <= 0.0 && 0.0 <= high_y) ||
                        (0.0 <= low_y && high_y <= 1.0))) &&
                        (xv == 0.0 || yv == 0.0 || /* no motion in x or y or both */
                        (low_y <= high_x && high_x <= high_y) ||
                        (low_y <= low_x && low_x <= high_y) ||
                        (low_x <= low_y && high_y <= high_x)));
            };

            static void putIntoMap(RedBlackTree<Vector2D, std::vector<Motion>> voxelMap, Vector2D voxel, Motion motion) {
                std::vector<Motion> array = voxelMap.get(voxel);
                if (array.empty()) {
                    array = std::vector<Motion>();
                    voxelMap.put(voxel, array);
                }
                array.push_back(motion);
            }

            void recurse(RedBlackTree<Vector2D, std::vector<Motion>> voxelMap, RedBlackTree<Vector2D, bool> seen, Vector2D nextVoxel, Motion motion) {
                if (!isInVoxel(nextVoxel, motion)) {
                    return;
                }

                if (seen.put(nextVoxel, true) == true) {
                    return;
                }

                putIntoMap(voxelMap, nextVoxel, motion);

                recurse(voxelMap, seen, nextVoxel.minus(_horizontal), motion);
                recurse(voxelMap, seen, nextVoxel.plus(_horizontal), motion);
                recurse(voxelMap, seen, nextVoxel.minus(_vertical), motion);
                recurse(voxelMap, seen, nextVoxel.plus(_vertical), motion);
                recurse(voxelMap, seen, nextVoxel.minus(_horizontal).minus(_vertical), motion);
                recurse(voxelMap, seen, nextVoxel.minus(_horizontal).plus(_vertical), motion);
                recurse(voxelMap, seen, nextVoxel.plus(_horizontal).minus(_vertical), motion);
                recurse(voxelMap, seen, nextVoxel.plus(_horizontal).plus(_vertical), motion);
            }

            std::vector<std::vector<Motion>> reduceCollisionSet(std::vector<Motion> motions) {
                RedBlackTree<Vector2D, std::vector<Motion>> voxelMap = RedBlackTree<Vector2D, std::vector<Motion>>();

                for (Motion& motion: motions) {
                    drawMotionOnVoxelMap(voxelMap, motion);
                }

                std::vector<std::vector<Motion>> result;


                voxelMap.forEach({ if (e.value.size() > 1) {
                    result.append(e.value);
                }})
                return result;
            }

            Vector2D voxelHash(Vector3D position) {
                int xDiv = (int) (position._x / Constants::GOOD_VOXEL_SIZE);
                int yDiv = (int) (position._y / Constants::GOOD_VOXEL_SIZE);

                double x = Constants::GOOD_VOXEL_SIZE * xDiv;
                double y = Constants::GOOD_VOXEL_SIZE * yDiv;

                if (position._x < 0) {
                    x -= Constants::GOOD_VOXEL_SIZE;
                }
                if (position._y < 0) {
                    y -= Constants::GOOD_VOXEL_SIZE;
                }

                return {x, y};
            }

            void drawMotionOnVoxelMap(RedBlackTree<Vector2D, std::vector<Motion>> voxelMap, Motion motion) {
                RedBlackTree<Vector2D, bool> seen = RedBlackTree<Vector2D, bool>();
                recurse(voxelMap, seen, voxelHash(motion._posOne), motion);
            }

        public:

            std::vector<Collision> handleNewFrame(const std::vector<Aircraft>& frame) {
                std::vector<Motion> motions = std::vector<Motion>();
                RedBlackTree<CallSign, bool> seen = RedBlackTree<CallSign, bool>();

                for (Aircraft aircraft: frame) {
                    Vector3D oldPosition = _state.put(aircraft._callsign, aircraft->_position);
                    Vector3D newPosition = aircraft._position;
                    seen.put(aircraft._callsign, true);

                    if (oldPosition == Vector3D{}) {
                        // Treat newly introduced aircraft as if they were stationary.
                        oldPosition = newPosition;
                    }

                    motions.push_back(Motion(aircraft._callsign, oldPosition, newPosition));
                }

                std::vector<CallSign> toRemove = std::vector<CallSign>();

                _state.forEach({if (!seen.get(e.key)) {
                    toRemove.append(e.key);
                }});
                
                for (CallSign& e: toRemove) {
                    _state.remove(e);
                }

                std::vector<std::vector<Motion>> allReduced = reduceCollisionSet(motions);
                std::vector<Collision> collisions;

                for (std::vector<Motion> reduced: allReduced) {
                    for (int i = 0; i < reduced.size(); i++) {
                        Motion motion1 = reduced[i];
                        for (int j = 0; i < reduced.size(); j++) {
                            Motion motion2 = reduced[j];
                            Vector3D collision = motion1.findIntersection(motion2);
                            if (collision != Vector3D{}) {
                                collisions.push_back(Collision(motion1._callsign, motion2._callsign, collision));
                            }
                        }
                    }
                }
                return collisions;
            };

    };
};