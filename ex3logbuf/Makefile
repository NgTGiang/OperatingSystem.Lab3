CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: logger

logger: main.o logger.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c logger.h
	$(CC) $(CFLAGS) -c main.c

logger.o: logger.c logger.h
	$(CC) $(CFLAGS) -c logger.c

clean:
	rm -f *.o logger