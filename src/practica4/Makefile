CC := gcc
CFLAGS := -g -std=c99 -D_POSIX_C_SOURCE=200809L
WARNINGS := -Wall -Werror

all: fcfs mfq sjf proceso rr

rr: virtual_processor.o schedulerRR.c
	$(CC) $(CFLAGS) $(WARNINGS) -o rr schedulerRR.c virtual_processor.o

fcfs: virtual_processor.o schedulerFCFS.c
	$(CC) $(CFLAGS) $(WARNINGS) -o fcfs schedulerFCFS.c virtual_processor.o

sjf: virtual_processor.o schedulerSJF.c
	$(CC) $(CFLAGS) $(WARNINGS) -o sjf schedulerSJF.c virtual_processor.o

mfq: virtual_processor.o schedulerMFQ.c
	$(CC) $(CFLAGS) $(WARNINGS) -o mfq schedulerMFQ.c virtual_processor.o -lm

proceso: proceso.c
	$(CC) $(CFLAGS) $(WARNINGS) -o proceso proceso.c

virtual_processor.o: virtual_processor.c virtual_processor.h
	$(CC) $(CFLAGS) -c virtual_processor.c

.PHONY: clean
clean:
	rm -f *.o *~ fcfs mfq sjf proceso rr
