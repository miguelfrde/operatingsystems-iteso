CC := gcc
CFLAGS := -g -Wall -Werror -std=c99 -Wno-format-contains-nul

DEPENDENCIES := vdisk.o
SRCS := global.c dumpsec.c dumpseclog.c fileio.c shell.c vdfdisk.c vdformat.c vdlib.c createvd.c
MAINS := dumpsec dumpseclog shell vdformat createvd
OBJS := $(SRCS:.c=.o)

all: $(MAINS)

shell: shell.o fileio.o vdlib.o global.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

vdfdisk: vdfdisk.o vdlib.o global.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

dumpsec: dumpsec.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

dumpseclog: dumpseclog.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

createvd: createvd.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

vdformat: vdformat.o
	$(CC) $(CFLAGS) -o $@ $^ $(DEPENDENCIES)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) *~ $(MAINS)
