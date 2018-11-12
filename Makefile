CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

cc: $(OBJS)

$(OBJS): cc.h

test: cc
		./test.sh

clean:
		rm -f cc *.o *~