all: timer.o hw4

timer.o: timer.c
	gcc timer.c -c -I. -lm -g

hw4: hw4.c
	gcc hw4.c timer.o -o hw4 -lm -g

