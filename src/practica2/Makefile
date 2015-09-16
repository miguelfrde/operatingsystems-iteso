CC := gcc
CFLAGS := -O0 -g -Wall -Werror -std=c99
LIB_DIR := lib

SH_SRC := sh.c lib/linkedlist.c lib/sh_input.c
SH_OBJS := $(SH_SRC:.c=.o)
SH_MAIN := sh

GETTY_SRC := getty.c
GETTY_OBJS := $(GETTY_SRC:.c=.o)
GETTY_MAIN := getty

INIT_SRC := init.c
INIT_OBJS := $(INIT_SRC:.c=.o)
INIT_MAIN := init

MAINS := $(SH_MAIN) $(GETTY_MAIN) $(INIT_MAIN)

all: $(MAINS)

$(SH_MAIN): $(SH_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(GETTY_MAIN): $(GETTY_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(INIT_MAIN): $(INIT_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o lib/*.o *~ $(MAINS)
