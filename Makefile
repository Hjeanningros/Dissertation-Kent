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
			src/nbody/Body.cpp						\
			src/nbody/NBodySystem.cpp				\
			src/NBody.cpp
			src/json/JsonArray.cpp					\
			src/json/JsonLiteral.cpp				\
			src/json/JsonNumber.cpp					\
			src/json/JsonObject.cpp					\
			src/json/JsonPureStringParser.cpp		\
			src/json/JsonString.cpp					\
			src/json/JsonValue.cpp					\
			src/json/ParseException.cpp				\
			src/Json.cpp									






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