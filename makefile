run:
	gcc -Wall -c chrono.c -o chrono.o
	gcc -Wall chrono.o max-heap.c -o max-heap -lpthread -lm

clean:
	rm *.o
	rm a.out
	rm max-heap
	resultado*