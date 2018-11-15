CFLAGS=-Wall -std=c11 -fpermissive
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

cc: $(OBJS)
	cc -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): cc.h

test: cc
		./test.sh

clean:
		rm -f cc *.o *~