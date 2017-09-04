
CFLAGS=-std=c99 -pedantic -Wall -D_POSIX_SOURCE
DEPS = mac.h
OBJ = modemd.o mac.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

modemd: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lm

clean:
	rm -rf modemd *.o
