INC=../inc
SRC=../src
OBJ= main_shell.o tree.o dir_node.o

build: diana.out clean_obj

all: diana.out

main_shell.o: main_shell.c
	gcc -c main_shell.c -ggdb -o main_shell.o -I $(INC)

tree.o: tree.c
	gcc -c tree.c -ggdb -o tree.o -I $(INC)

dir_node.o: dir_node.c
	gcc -c dir_node.c -ggdb -o dir_node.o -I $(INC)

diana.out: $(OBJ)
	gcc $(OBJ) -ggdb -o diana.out

rebuild: clean build

.PHONY: clean clean_obj
clean:
	rm -f diana.out $(OBJ)

clean_obj:
	rm -f $(OBJ)