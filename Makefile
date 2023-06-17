SRC_CORE=	src/som/Error.cpp				\
			src/Bounce.cpp					\
			src/Run.cpp 					\
			src/Harness.cpp					\
			src/List.cpp					\
			src/Mandelbrot.cpp				\
			src/Benchmark.cpp				\
			src/Sieve.cpp					\
			src/Permute.cpp					\
			src/Queens.cpp



OBJ_CORE    =	$(SRC_CORE:.cpp=.o)

CXXFLAGS+=	-Wall -Wextra -std=c++17

NAME_CORE   =	Benchmark


$(NAME_CORE):	$(OBJ_CORE)
	g++ -o $(NAME_CORE) $(OBJ_CORE) $(CXXFLAGS) -ldl

all:    $(NAME_CORE) 

core: $(NAME_CORE)


clean:
		rm -f $(OBJ_CORE)

fclean:		clean
		rm -f $(NAME_CORE)

re:		fclean all