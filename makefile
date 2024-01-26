CC = gcc
CFLAGS = -c -ansi -Wall -pedantic
OBJ_DIR = ./obj

mainmat: $(OBJ_DIR)/mainmat.o $(OBJ_DIR)/mymat.o $(OBJ_DIR)/inputhelper.o $(OBJ_DIR)/mathelper.o $(OBJ_DIR)/errorhelper.o
	gcc -ansi -Wall -pedantic -g "$(OBJ_DIR)/mainmat.o" "$(OBJ_DIR)/mymat.o" "$(OBJ_DIR)/inputhelper.o" "$(OBJ_DIR)/mathelper.o" "$(OBJ_DIR)/errorhelper.o" -o mainmat

$(OBJ_DIR)/mainmat.o: mainmat.c mymat.h inputhelper.h errorhelper.h mathelper.h
	$(CC) $(CFLAGS) mainmat.c -o "$(OBJ_DIR)/mainmat.o"

$(OBJ_DIR)/mymat.o: mymat.c mymat.h
	$(CC) $(CFLAGS) mymat.c -o "$(OBJ_DIR)/mymat.o"

$(OBJ_DIR)/inputhelper.o: inputhelper.c inputhelper.h errorhelper.h
	$(CC) $(CFLAGS) inputhelper.c -o "$(OBJ_DIR)/inputhelper.o"

$(OBJ_DIR)/mathelper.o: mathelper.c mathelper.h inputhelper.h
	$(CC) $(CFLAGS) mathelper.c -o "$(OBJ_DIR)/mathelper.o"

$(OBJ_DIR)/errorhelper.o: errorhelper.c errorhelper.h
	$(CC) $(CFLAGS) errorhelper.c -o "$(OBJ_DIR)/errorhelper.o"
