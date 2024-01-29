CC = gcc
CFLAGS = -c -ansi -Wall -pedantic
OBJ_DIR = ./obj

mainmat: mainmat.o mymat.o inputhelper.o mathelper.o errorhelper.o
	gcc -ansi -Wall -pedantic -g mainmat.o mymat.o inputhelper.o mathelper.o errorhelper.o -o mainmat

mainmat.o: mainmat.c mymat.h inputhelper.h errorhelper.h mathelper.h
	$(CC) $(CFLAGS) mainmat.c -o mainmat.o

mymat.o: mymat.c mymat.h
	$(CC) $(CFLAGS) mymat.c -o mymat.o

inputhelper.o: inputhelper.c inputhelper.h errorhelper.h
	$(CC) $(CFLAGS) inputhelper.c -o inputhelper.o

mathelper.o: mathelper.c mathelper.h inputhelper.h
	$(CC) $(CFLAGS) mathelper.c -o mathelper.o

errorhelper.o: errorhelper.c errorhelper.h
	$(CC) $(CFLAGS) errorhelper.c -o errorhelper.o
	
