CC := cc
CFLAGS := -g -Wall -std=c99 -D_GNU_SOURCE

SRCS := dumpseclog.c vdfdisk.c
MAINS := dumpseclog vdfdisk
DEPENDENCIES := vdisk.o

OBJS := $(SRCS:.c=.o)

all: $(MAINS)

dumpseclog: dumpseclog.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

vdfdisk: vdfdisk.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *~ $(MAINS) && cp lib/vdisk.o vdisk.o
