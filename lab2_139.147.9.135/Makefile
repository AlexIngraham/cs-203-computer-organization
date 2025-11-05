CC = gcc
CFLAGS = -Wall -g

all: prog1 prog2 prog3

prog1: prog1.c
	$(CC) $(CFLAGS) -o prog1 prog1.c

prog2: prog2.c
	$(CC) $(CFLAGS) -o prog2 prog2.c

prog3: prog3.c
	$(CC) $(CFLAGS) -o prog3 prog3.c

clean:
	rm -f prog1 prog2 prog3

.PHONY: all clean
