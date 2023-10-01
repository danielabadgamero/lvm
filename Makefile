SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

EXE = lvm

FLAGS = -Wall -Wextra -Werror -g

$(EXE): $(OBJ)
	gcc $^ -o $@ $(FLAGS)

obj/%.o: src/%.c
	gcc -c -Iinclude $^ -o $@ $(FLAGS)
