CC := gcc
CFLAGS := -O3 -g -Wall -Werror -std=c99 -Wno-unused-result
LDFLAGS := -lpthread
LIB_DIR := lib

SRCS := main.c main_pthreads.c main_clone.c main_just_cache.c
MAINS := main main_pthreads main_clone main_just_cache

OBJS := $(SRCS:.c=.o)

all: $(MAINS)

main: main.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

main_pthreads: main_pthreads.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

main_clone: main_clone.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

main_just_cache: main_just_cache.o
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o lib/*.o *~ $(MAINS)
