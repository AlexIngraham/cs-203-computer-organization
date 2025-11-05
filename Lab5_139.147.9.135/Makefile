CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = fish_simulator
OBJS = main.o bit_manip.o game.o file.o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o : main.c bit_manip.h game.h file.h
	$(CC) $(CFLAGS) -c main.c

bit_manip.o : bit_manip.c bit_manip.h
	$(CC) $(CFLAGS) -c bit_manip.c

game.o : game.c game.h bit_manip.h
	$(CC) $(CFLAGS) -c game.c

file.o : file.c file.h game.h
	$(CC) $(CFLAGS) -c file.c

clean :
	rm -f $(TARGET) $(OBJS)

.PHONY : all clean