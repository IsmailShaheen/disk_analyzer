build: diana.out

diana.out: main.c tree.h
	gcc -ggdb main.c -o diana.out

rebuild: clean build

.PHONY: clean run
clean:
	rm -f diana.out

run:
	./diana.out