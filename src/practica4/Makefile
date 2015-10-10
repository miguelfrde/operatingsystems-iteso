CC := gcc
CFLAGS := -g -Wall -Werror -std=c99 -Wno-unused-result

all: fcfs proceso

fcfs: virtual_processor.o schedulerFCFS.c
	$(CC) $(CFLAGS) -o fcfs schedulerFCFS.c virtual_processor.o

proceso: proceso.c
	$(CC) $(CFLAGS) -o proceso proceso.c

virtual_processor.o: virtual_processor.c virtual_processor.h
	$(CC) $(CFLAGS) -c virtual_processor.c

.PHONY: clean
clean:
	rm -f *.o *~ fcfs
