SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,obj/%.o,$(SRC))

FLAGS = -Wall -Wextra -Werror -std=c++20 -g

bin: $(OBJ)
	g++ $^ -o $@ $(FLAGS)

obj/%.o: src/%.cpp
	g++ -c -Iinclude $^ -o $@ $(FLAGS)
