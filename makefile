all: main.c built_ins.h
	gcc -o shell.exe main.c built_ins.h
