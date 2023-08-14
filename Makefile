SRC_CORE=	src/som/Error.cpp						\
			src/som/Random.cpp						\
			src/som/Dictionary.cpp					\
			src/som/IdentityDictionary.cpp			\
			src/som/Vector.cpp						\
			src/Bounce.cpp							\
			src/Run.cpp 							\
			src/Harness.cpp							\
			src/List.cpp							\
			src/Mandelbrot.cpp						\
			src/Benchmark.cpp						\
			src/Sieve.cpp							\
			src/Permute.cpp							\
			src/Towers.cpp							\
			src/Queens.cpp							\
			src/Storage.cpp							\
			src/nbody/Body.cpp						\
			src/nbody/NBodySystem.cpp				\
			src/NBody.cpp							\
			src/json/JsonArray.cpp					\
			src/json/JsonLiteral.cpp				\
			src/json/JsonNumber.cpp					\
			src/json/JsonObject.cpp					\
			src/json/JsonPureStringParser.cpp		\
			src/json/JsonString.cpp					\
			src/json/JsonValue.cpp					\
			src/json/ParseException.cpp				\
			src/Json.cpp							\
			src/havlak/BasicBlock.cpp				\
			src/havlak/BasicBlockEdge.cpp			\
			src/havlak/ControlFlowGraph.cpp			\
			src/havlak/HavlakLoopFinder.cpp			\
			src/havlak/LoopTesterApp.cpp			\
			src/havlak/SimpleLoop.cpp				\
			src/havlak/LoopStructureGraph.cpp		\
			src/havlak/UnionFindNode.cpp			\
			src/Havlak.cpp							\
			src/deltablue/Strength.cpp				\
			src/deltablue/ScaleConstraint.cpp		\
			src/deltablue/AbstractConstraint.cpp	\
			src/deltablue/BinaryConstraint.cpp		\
			src/deltablue/EditConstraint.cpp		\
			src/deltablue/EqualityConstraint.cpp	\
			src/deltablue/Plan.cpp					\
			src/deltablue/Planner.cpp				\
			src/deltablue/StayConstraint.cpp		\
			src/deltablue/UnaryConstraint.cpp		\
			src/deltablue/Variable.cpp				\
			src/richards/DeviceTaskDataRecord.cpp	\
			src/richards/HandlerTaskDataRecord.cpp	\
			src/richards/IdleTaskDataRecord.cpp		\
			src/richards/Packet.cpp					\
			src/richards/RBObject.cpp				\
			src/richards/Scheduler.cpp				\
			src/richards/TaskControlBlock.cpp		\
			src/richards/TaskState.cpp				\
			src/richards/WorkerTaskDataRecord.cpp	\
			src/Richards.cpp						\
			src/cd/RedBlackTree.cpp					\
			src/cd/Aircraft.cpp						\
			src/cd/CallSign.cpp						\
			src/cd/Collision.cpp					\
			src/cd/CollisionDetector.cpp			\
			src/cd/Motion.cpp						\
			src/cd/Simulator.cpp					\
			src/cd/Vector2D.cpp						\
			src/cd/Vector3D.cpp						\
			src/CD.cpp
			






OBJ_CORE    =	$(SRC_CORE:.cpp=.o)

CXXFLAGS+=	-Wall -Wextra -std=c++17 -O2

NAME_CORE   =	Harness


$(NAME_CORE):	$(OBJ_CORE)
	g++ -o $(NAME_CORE) $(OBJ_CORE) $(CXXFLAGS) -ldl 

all:	$(NAME_CORE) clean

core:	$(NAME_CORE) clean


clean:
		rm -f $(OBJ_CORE)

fclean:		clean
		rm -f $(NAME_CORE)

re:		fclean all