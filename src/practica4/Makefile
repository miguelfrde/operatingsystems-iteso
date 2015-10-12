CC := gcc
CFLAGS := -g -std=c99 -D_POSIX_C_SOURCE=200809L
WARNINGS := -Wall -Werror

all: fcfs mfq proceso

fcfs: virtual_processor.o schedulerFCFS.c
	$(CC) $(CFLAGS) $(WARNINGS) -o fcfs schedulerFCFS.c virtual_processor.o

mfq: virtual_processor.o schedulerMFQ.c
	$(CC) $(CFLAGS) $(WARNINGS) -o mfq schedulerMFQ.c virtual_processor.o -lm

proceso: proceso.c
	$(CC) $(CFLAGS) $(WARNINGS) -o proceso proceso.c

virtual_processor.o: virtual_processor.c virtual_processor.h
	$(CC) $(CFLAGS) -c virtual_processor.c

.PHONY: clean
clean:
	rm -f *.o *~ fcfs proceso
