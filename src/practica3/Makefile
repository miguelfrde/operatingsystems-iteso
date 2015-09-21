CC := gcc
CFLAGS := -O3 -g -Wall -Werror -std=c99
LDFLAGS := -pthread
LIB_DIR := lib

SRCS := main.c

OBJS := $(SRCS:.c=.o)
MAIN := main

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o lib/*.o *~ $(MAIN)
