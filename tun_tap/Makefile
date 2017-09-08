
CFLAGS=-std=c99 -pedantic -Wall -D_POSIX_SOURCE
DEPS = mac.h
OBJ = modemd.o mac.o
prefix=/usr/local

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

modemd: $(OBJ)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lm
install:
	install -m 0755 modemd $(prefix)/bin	
clean:
	rm -rf modemd *.o
