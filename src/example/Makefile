CC := gcc
CFLAGS := -O3 -g -Wall -Werror -D_USE_MATH_DEFINE

all: main

main: main.o
	$(CC) $(CFLAGS) -o main main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

.PHONY: clean
clean:
	rm -f *.o *~ main
