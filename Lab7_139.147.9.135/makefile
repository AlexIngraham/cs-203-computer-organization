CC = gcc
CFLAGS = -Wall -Wextra -g -std=c99
TARGET = ssam_vm

SRCS = sim.c controller.c memory.c
OBJS = $(SRCS:.c=.o)
HEADERS = controller.h memory.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) log_file.txt dump_log.txt

test: $(TARGET)
	./$(TARGET) inclassMon_function.bin

.PHONY: all clean test