CC := gcc
CFLAGS := -g -Wall -Werror -std=c99 -D_GNU_SOURCE
LDFLAGS := -lpthread

SRCS := hw_solution_processes.c hw_solution_threads
MAINS := hw_solution_processes hw_solution_threads

OBJS := $(SRCS:.c=.o)

all: $(MAINS)

hw_solution_processes: hw_solution_processes.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

hw_solution_threads: hw_solution_threads.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lpthread

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAINS)
