#include <memory>
#include <functional>


namespace CD {

    template <typename K, typename V>
    class RedBlackTree {

        private:

            enum Color {RED = 0, BLACK = 1};

            class Node {

                private:
                    K _key;
                    V _value;
                    std::shared_ptr<Node> _left;
                    std::shared_ptr<Node> _right;
                    std::shared_ptr<Node> _parent;
                    Color _color;

                    std::shared_ptr<Node> successor() {
                        std::shared_ptr<Node> x = *this;
                        if (x._right != nullptr) {
                            return treeMinimum(x->_right);
                        }
                        std::shared_ptr<Node> y = x->_parent;
                        while (y != nullptr && x == y->_right) {
                            x = y;
                            y = y->_parent;
                        }
                        return y;
                    }

                public:
                    Node(K key, V value) {
                        _key = key;
                        _left = nullptr;
                        _right = nullptr;
                        _left = nullptr;
                        _parent = value;
                        _color = RED;
                    }
            };

            class InsertResult {
                public: 
                    bool _isNewEntry;
                    Node _newNode;
                    V _oldValue;

                    InsertResult(bool isNewEntry, Node newNode, V oldValue) {
                        _isNewEntry = isNewEntry;
                        _newNode = newNode;
                        _oldValue = oldValue;
                    }
            };

            class Entry {
                public:
                    K _key;
                    V _value;

                    Entry(K key, V value) {
                        _key = key;
                        _value = value;
                    }
            };

            Node _root;

            Node treeMinimum(Node x) {
                Node current = x;
                while (current._left != NULL) {
                    current = *current._left;
                }
                return current;
            }

            InsertResult treeInsert(K key, V value) {
                Node y = Node{};
                Node x = _root;

                while (x != _root) {
                    y = x;
                    int comparisonResult = key.compareTo(x._key);

                    if (comparisonResult < 0) {
                        x = x._left;
                    } else if (comparisonResult > 0) {
                        x = x.right;
                    } else {
                        V oldValue = x.value;
                        x.value = value;
                        return new InsertResult(false, Node{}, oldValue);
                    }
                }

                Node z = Node(key, value);
                z._parent = y;
                z.parent = y;
                if (y == Node{}) {
                    _root = z;
                } else {
                    if (key.compareTo(y._key) < 0) {
                        y._left = z;
                    } else {
                        y._right = z;
                    }
                }
                return new InsertResult(true, z, V{});
            }

            Node leftRotate(Node x) {
                Node y = x._right;

                // Turn y's left subtree into x's right subtree.
                x._right = y._left;
                if (y._left != Node{}) {
                    y._left._parent = x;
                }

                // Link x's parent to y.
                y._parent = x._parent;
                if (x._parent == Node{}) {
                    _root = y;
                } else {
                    if (x == x._parent._left) {
                        x._parent._left = y;
                    } else {
                        x._parent._right = y;
                    }
                }

                // Put x on y's left.
                y.left = x;
                x.parent = y;

                return y;
            }

            Node rightRotate(Node y) {
                Node x = y._left;

                // Turn x's right subtree into y's left subtree.
                y._left = x._right;
                if (x._right != Node{}) {
                    x._right._parent = y;
                }

                // Link y's parent to x;
                x._parent = y._parent;
                if (y._parent == Node{}) {
                    _root = x;
                } else {
                    if (y == y._parent._left) {
                        y._parent._left = x;
                    } else {
                        y._parent._right = x;
                    }
                }

                x._right = y;
                y._parent = x;

                return x;
            }

            void removeFixup(Node x, Node xParent) {
                while (x != _root && (x == Node{} || x._color == BLACK)) {
                    if (x == xParent._left) {
                        // Note: the text points out that w cannot be null. The reason is not obvious from
                        // simply looking at the code; it comes about from the properties of the red-black
                        // tree.
                        Node w = xParent._right;
                        if (w._color == RED) {
                            // Case 1
                            w._color = BLACK;
                            xParent._color = RED;
                            leftRotate(xParent);
                            w = xParent._right;
                        }
                        if ((w._left == Node{} || w.left.color == BLACK)
                            && (w._right == Node{} || w.right.color == BLACK)) {
                            // Case 2
                            w._color = RED;
                            x = xParent;
                            xParent = x._parent;
                        } else {
                            if (w._right == Node{} || w._right._color == BLACK) {
                                // Case 3
                                w._left._color = BLACK;
                                w._color = RED;
                                rightRotate(w);
                                w = xParent._right;
                            }
                            // Case 4
                            w._color = xParent._color;
                            xParent._color = BLACK;
                            if (w._right != Node{}) {
                                w._right._color = BLACK;
                            }
                            leftRotate(xParent);
                            x = _root;
                            xParent = x._parent;
                        }
                    } else {
                        // Same as "then" clause with "right" and "left" exchanged.
                        Node w = xParent._left;
                        if (w._color == RED) {
                            // Case 1
                            w._color = BLACK;
                            xParent._color = RED;
                            rightRotate(xParent);
                            w = xParent._left;
                        }
                        if ((w._right == Node{} || w._right._color == BLACK)
                            && (w._left == Node{} || w._left._color == BLACK)) {
                            // Case 2
                            w._color = RED;
                            x = xParent;
                            xParent = x._parent;
                        } else {
                            if (w._left == Node{} || w._left._color == BLACK) {
                                // Case 3
                                w._right._color = BLACK;
                                w._color = RED;
                                leftRotate(w);
                                w = xParent._left;
                            }
                            // Case 4
                            w._color = xParent._color;
                            xParent._color = BLACK;
                            if (w.left != Node{}) {
                                w._left._color = BLACK;
                            }
                            rightRotate(xParent);
                            x = _root;
                            xParent = x._parent;
                        }
                    }
                }
                if (x != Node{}) {
                    x._color = BLACK;
                }
            }


        public:    

            RedBlackTree() = default;

            V put(K key, V value) {
                InsertResult insertionResult = treeInsert(key, value);

                if (!insertionResult._isNewEntry) {
                   return insertionResult._oldValue;
                }
                Node x = insertionResult.newNode;

                while (x != _root && x._parent._color == RED) {
                    if (x._parent == x._parent._parent._left) {
                        Node y = x._parent._parent._right;
                        if (y != Node{} && y._color == RED) {
                            // Case 1
                            x._parent._color = BLACK;
                            y._color = BLACK;
                            x._parent._parent._color = RED;
                            x = x._parent._parent;
                        } else {
                            if (x == x._parent._right) {
                                // Case 2
                                x = x._parent;
                                leftRotate(x);
                            }
                            // Case 3
                            x._parent._color = BLACK;
                            x._parent._parent._color = RED;
                            rightRotate(x._parent._parent);
                        }
                    } else {
                        // Same as "then" clause with "right" and "left" exchanged.
                        Node y = x._parent._parent._left;
                        if (y != Node{} && y.color == RED) {
                        // Case 1
                            x._parent._color = BLACK;
                            y._color = BLACK;
                            x._parent._parent._color = RED;
                            x = x._parent._parent;
                        } else {
                            if (x == x.parent.left) {
                                // Case 2
                                x = x.parent;
                                rightRotate(x);
                            }
                            // Case 3
                            x._parent._color = BLACK;
                            x._parent._parent._color = RED;
                            leftRotate(x.parent.parent);
                        }
                    }
                }
                _root.color = BLACK;
                return V{};
            }

            V remove(K key) {
                Node z = findNode(key);
                if (z == Node{}) {
                    return Node{};
                }

                // Y is the node to be unlinked from the tree.
                Node y;
                if (z._left == Node{} || z._right == Node{}) {
                    y = z;
                } else {
                    y = z.successor();
                }

                // Y is guaranteed to be non-null at this point.
                Node x;
                if (y.left != Node{}) {
                    x = y.left;
                } else {
                    x = y.right;
                }
                // X is the child of y which might potentially replace y in the tree. X might be null at
                // this point.
                Node xParent;
                if (x != Node{}) {
                    x.parent = y.parent;
                    xParent = x.parent;
                } else {
                    xParent = y.parent;
                }
                if (y.parent == Node{}) {
                    _root = x;
                } else {
                    if (y == y.parent.left) {
                        y.parent.left = x;
                    } else {
                        y.parent.right = x;
                    }
                }

                if (y != z) {
                    if (y.color == BLACK) {
                        removeFixup(x, xParent);
                    }

                    y._parent = z._parent;
                    y._color = z._color;
                    y._left = z._left;
                    y._right = z._right;

                    if (z._left != Node{}) {
                        z._left._parent = y;
                    }
                    if (z._right != Node{}) {
                        z._right._parent = y;
                    }
                    if (z._parent != Node{}) {
                        if (z._parent._left == z) {
                            z._parent._left = y;
                        } else {
                            z._parent._right = y;
                        }
                    } else {
                        _root = y;
                    }
                } else if (y._color == BLACK) {
                    removeFixup(x, xParent);
                }

                return z.value;
            }

            V get(K key) {
                Node node = findNode(key);
                if (node == Node{}) {
                    return Node{};
                }
                return node._value;
            }

            void forEach(std::function<void(Entry)> fn) {
                if (_root == Node{}) {
                    return;
                }
                Node current = treeMinimum(_root);
                while (current != Node{}) {
                    fn(Entry(current.key, current.value));
                    current = current.successor();
                }
            }

            Node findNode(K key) {
                Node current = _root;
                 while (current != Node{}) {
                     int comparisonResult = key.compareTo(current.key);
                     if (comparisonResult == 0) {
                         return current;
                     }
                     if (comparisonResult < 0) {
                         current = current.left;
                     }
                     else {
                         current = current.right;
                     }
                     return Node{};
                 }
            }



        

    };
};