CC := gcc
CFLAGS := -O0 -g -Wall -Werror -std=c99
LIB_DIR := lib

SH_SRC := sh.c lib/linkedlist.c lib/sh_input.c
SH_OBJS := $(SH_SRC:.c=.o)
SH_MAIN := sh

all: sh

$(SH_MAIN): $(SH_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o lib/*.o *~ sh
