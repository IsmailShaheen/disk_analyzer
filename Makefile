all: diana

diana: main.c tree.h
	gcc main.c -o diana