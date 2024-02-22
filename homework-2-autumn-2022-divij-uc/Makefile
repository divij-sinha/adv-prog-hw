CC=gcc
CFLAGS=-I.
DEPS = timer.h
OBJ = timer.o nbody.o 
OBJPT = timer.o nbody_pt.o 
OBJOMP = timer.o nbody_omp.o 

all: nbody nbody_pt nbody_omp

nbody: timer.o nbody.o
	gcc -o nbody timer.o nbody.o -I. -lm

nbody_pt: timer.o nbody_pt.o
	gcc -o nbody_pt timer.o nbody_pt.o -I. -lm -pthread

nbody_omp: timer.o nbody_omp.o
	gcc -o nbody_omp timer.o nbody_omp.o -I. -lm -fopenmp

timer.o: timer.c
	gcc -c -o timer.o timer.c -I. -lm

nbody.o: nbody.c
	gcc -c -o nbody.o nbody.c -I. -lm 

nbody_pt.o: nbody_pt.c
	gcc -c -o nbody_pt.o nbody_pt.c -I. -lm -pthread

nbody_omp.o: nbody_omp.c
	gcc -c -o nbody_omp.o nbody_omp.c -I. -lm -fopenmp

clean:
	\rm -f *.o nbody *~ *#
