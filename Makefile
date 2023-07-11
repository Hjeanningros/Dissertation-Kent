SRC_CORE=	src/som/Error.cpp						\
			src/som/Random.cpp						\
			src/Bounce.cpp							\
			src/Run.cpp 							\
			src/Harness.cpp							\
			src/List.cpp							\
			src/Mandelbrot.cpp						\
			src/Benchmark.cpp						\
			src/Sieve.cpp							\
			src/Permute.cpp							\
			src/Queens.cpp							\
			src/Richards.cpp						\
			src/richards/DeviceTaskDataRecord.cpp	\
			src/richards/HandlerTaskDataRecord.cpp	\
			src/richards/IdleTaskDataRecord.cpp		\
			src/richards/Packet.cpp					\
			src/richards/RBObject.cpp				\
			src/richards/TaskControlBlock.cpp		\
			src/richards/TaskState.cpp				\
			src/richards/WorkerTaskDataRecord.cpp	\
			src/richards/Scheduler.cpp	




OBJ_CORE    =	$(SRC_CORE:.cpp=.o)

CXXFLAGS+=	-Wall -Wextra -std=c++17 -O2

NAME_CORE   =	Harness


$(NAME_CORE):	$(OBJ_CORE)
	g++ -o $(NAME_CORE) $(OBJ_CORE) $(CXXFLAGS) -ldl

all:    $(NAME_CORE) 

core: $(NAME_CORE)


clean:
		rm -f $(OBJ_CORE)

fclean:		clean
		rm -f $(NAME_CORE)

re:		fclean all