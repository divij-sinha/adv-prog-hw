all: timer.o djikstra

timer.o: timer.c
	gcc timer.c -c -I. -lm -g

djikstra: djikstra.c
	gcc djikstra.c timer.o -o djikstra -lm -g -fopenmp