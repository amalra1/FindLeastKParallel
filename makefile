run:
	gcc -Wall -O3 -c chrono.c -o chrono.o
	gcc -Wall -O3 chrono.o max-heap.c -o max-heap -lpthread -lm

clean:
	rm *.o
	rm max-heap