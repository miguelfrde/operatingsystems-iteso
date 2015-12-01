CC := gcc
CFLAGS := -g -Wall -Werror -std=c99

DEPENDENCIES := vdisk.o
SRCS := dumpsec.c dumpseclog.c fileio.c shell.c vdfdisk.c vdformat.c vdlib.c
MAINS := dumpsec dumpseclog vdfdisk shell vdformat
OBJS := $(SRCS:.c=.o)

all: $(MAINS)

shell: shell.o fileio.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

vdfdisk: vdfdisk.o vdlib.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

dumpsec: dumpsec.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

dumpseclog: dumpseclog.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

createvd:
	./createvd $(vdnum)

vdformat: vdformat.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) *~ $(MAINS)
