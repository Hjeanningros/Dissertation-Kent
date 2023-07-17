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
			src/deltablue/Variable.cpp






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