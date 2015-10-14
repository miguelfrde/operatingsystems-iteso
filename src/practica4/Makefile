CC := gcc
CFLAGS := -g -std=c99 -D_POSIX_C_SOURCE=200809L
WARNINGS := -Wall -Werror

all: fcfs proceso schedulerRR

schedulerRR: virtual_processor.o schedulerRR.c
	$(CC) $(CFLAGS) $(WARNINGS) -o schedulerRR schedulerRR.c virtual_processor.o

fcfs: virtual_processor.o schedulerFCFS.c
	$(CC) $(CFLAGS) $(WARNINGS) -o fcfs schedulerFCFS.c virtual_processor.o

proceso: proceso.c
	$(CC) $(CFLAGS) $(WARNINGS) -o proceso proceso.c

virtual_processor.o: virtual_processor.c virtual_processor.h
	$(CC) $(CFLAGS) -c virtual_processor.c

.PHONY: clean
clean:
	rm -f *.o *~ fcfs proceso
