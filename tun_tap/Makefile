
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
	install -m 0777 restart_modem_gui.sh $(prefix)/bin
	install -m 0444 modem_filter.ftr $(prefix)/share/rfsom-box-gui
clean:
	rm -rf modemd *.o
