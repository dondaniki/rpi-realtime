C:: 
	gcc -c dispositivos.c; 
	gcc -c periodic_tasks.c;
	gcc -c ceiling.c -D_GNU_SOURCE; 
	#gcc -Wall -std=c99 -pg -o ceiling ceiling.o periodic_tasks.o dispositivos.o -lrt -lwiringPi -lpthread
	gcc -o ceiling ceiling.o periodic_tasks.o dispositivos.o -lrt -lwiringPi -lpthread

clean::
	rm *.o ceiling

all:: C
