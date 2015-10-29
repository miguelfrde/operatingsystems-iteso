CC := gcc
CFLAGS := -g -Wall -Werror -std=c99 -D_GNU_SOURCE
LDFLAGS := -lpthread

SRCS := semaphore_solution.c msg_solution.c
MAINS := semaphore_solution msg_solution

OBJS := $(SRCS:.c=.o)

all: $(MAINS)

semaphore_solution: semaphore_solution.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

msg_solution: msg_solution.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAINS)
