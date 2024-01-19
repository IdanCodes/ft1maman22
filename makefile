mainmat: mainmat.o mymat.o utils.o
	gcc -ansi -Wall -pedantic -g mainmat.o mymat.o utils.o -o mainmat

mainmat.o: mainmat.c mymat.h
	gcc -c -ansi -Wall -pedantic mainmat.c -o mainmat.o

mymat.o: mymat.c mymat.h utils.h
	gcc -c -ansi -Wall -pedantic mymat.c -o mymat.o

utils.o: utils.c utils.h
	gcc -c -ansi -Wall -pedantic utils.c -o utils.o