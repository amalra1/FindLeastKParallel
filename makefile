CC = gcc 
CFLAGS = -std=c99 -Wall
LIBS = -lm -lpthread
NAME = max-heap
OBJ = max-heap.o chrono.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS) $(LIBS) 

max-heap.o: max-heap.c chrono.h
	$(CC) $(CFLAGS) -c max-heap.c

chrono.o: chrono.c chrono.h 
	$(CC) $(CFLAGS) -c chrono.c

clean:
	rm -f *.o

purge:
	rm -f *.o
	rm -f $(NAME)