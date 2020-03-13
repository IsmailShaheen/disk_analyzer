all: disk_analyzer.out

disk_analyzer.out: main.c
	gcc main.c -o disk_analyzer.out