CC = gcc
CFLAGS = -I. -std=c99 -Wall

DEPS = qtypes.h
OBJS = main.o

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

qlearn: $(OBJS)
	$(CC) -g -o $@ $^ $(CFLAGS) -lm

clean:
	rm -f qlearn *.o
