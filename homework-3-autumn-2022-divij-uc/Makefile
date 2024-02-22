all: nbody barneshut

nbody: timer.o nbody.o
	gcc -o nbody timer.o nbody.o -I. -lm -g

barneshut: timer.o barneshut.o quadtree.o
	gcc -o barneshut timer.o barneshut.o quadtree.o -I. -lm -fopenmp -g

timer.o: timer.c
	gcc -c -o timer.o timer.c -I. -lm -g

quadtree.o: quadtree.c
	gcc -c -o quadtree.o quadtree.c -I. -lm -g

nbody.o: nbody.c
	gcc -c -o nbody.o nbody.c -I. -lm -g

barneshut.o: barneshut.c
	gcc -c -o barneshut.o barneshut.c -I. -lm -fopenmp -g

clean:
	\rm -f *.o nbody *~ *#
