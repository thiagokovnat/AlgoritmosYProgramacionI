CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wconversion

compile: azkaban.c prision.c
	$(CC) $(CFLAGS) azkaban.c prision.c -o azkaban

perdonables: compile
	./azkaban perdonables perdonables.dat

liberar: compile
	./azkaban liberar perdonables.dat 201906 R

mostrar: compile
	./azkaban mostrar_liberados 201906

actualizar: compile
	./azkaban actualizar 201906
	./azkaban perdonables perdonables.dat
	./lector

test: compile tester.c
	./azkaban perdonables perdonables.dat
	./azkaban liberar perdonables.dat 201905 R
	$(CC) tester.c -o tester
	./tester

ayuda: compile
	./azkaban ayuda