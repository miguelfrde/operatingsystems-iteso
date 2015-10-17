CC := gcc
CFLAGS := -g -Wall -Werror -std=c99 -D_GNU_SOURCE
LDFLAGS := -lpthread

SRCS := hw_solution_processes.c hw_solution_threads.c semaphore_solution.c
MAINS := hw_solution_processes hw_solution_threads semaphore_solution

OBJS := $(SRCS:.c=.o)

all: $(MAINS)

hw_solution_processes: hw_solution_processes.o
	$(CC) $(CFLAGS) -o $@ $^

hw_solution_threads: hw_solution_threads.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

semaphore_solution: semaphore_solution.o semaphore.o
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAINS)
