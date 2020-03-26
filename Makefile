all: diana.out

diana.out: main.c tree.h
	gcc -ggdb main.c -o diana.out

.PHONY: clean
clean:
	rm -f diana.out

run:
	./diana.out