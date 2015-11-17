CC := gcc
IGNORE_WARNINGS := -Wno-implicit-function-declaration -Wno-unused-but-set-variable -Wno-pointer-to-int-cast
CFLAGS := -O3 -g -Wall -Werror -std=c99 $(IGNORE_WARNINGS)

SRCS := pagefault.c

PROCESSES_MAIN := processes
PROCESSES_SRC := processes.c pagefault.c
PROCESSES_OBJS := $(PROCESSES_SRC:.c=.o) mmu.o

PROCESSES_SWAP_MAIN := processes_swap
PROCESSES_SWAP_SRC := processes.c pagefault_with_swap.c
PROCESSES_SWAP_OBJS := $(PROCESSES_SWAP_SRC:.c=.o) mmu.o

CREATESWAP_MAIN := createswap
CREATESWAP_SRC := createswap.c
CREATESWAP_OBJS := $(CREATESWAP_SRC:.c=.o)

MAINS := $(PROCESSES_MAIN) $(CREATESWAP_MAIN) $(PROCESSES_SWAP_MAIN)
OBJS := $(PROCESSES_SRC:.c=.o) $(PROCESSES_SWAP_SRC:.c=.o) $(CREATESWAP_OBJS)

all: $(MAINS)

$(PROCESSES_MAIN): $(PROCESSES_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(PROCESSES_SWAP_MAIN): $(PROCESSES_SWAP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(CREATESWAP_MAIN): $(CREATESWAP_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

swap: $(CREATESWAP_MAIN)
	./$(CREATESWAP_MAIN)

.PHONY: clean
clean:
	rm -f $(OBJS) *~ $(MAINS) swap
