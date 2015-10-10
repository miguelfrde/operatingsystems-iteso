## all: fcfs rr proceso
all: fcfs proceso 

fcfs: virtual_processor.o schedulerFCFS.c
	gcc -o fcfs schedulerFCFS.c virtual_processor.o 

proceso: proceso.c
	gcc -o proceso proceso.c

virtual_processor.o: virtual_processor.c virtual_processor.h
	gcc -c virtual_processor.c

# rr: virtual_processor.o schedulerRR.c
#	gcc -o rr schedulerRR.c virtual_processor.o


clean:
	rm fcfs
#	rm rr
