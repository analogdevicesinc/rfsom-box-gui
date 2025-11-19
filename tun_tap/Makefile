PREFIX ?= /usr/local
DESTDIR=

CFLAGS=-std=c99 -pedantic -Wall -D_POSIX_SOURCE
DEPS = mac.h
OBJ_MODEMD = modemd.o mac.o
OBJ_IP_REG = ip_reg.o
OBJ_IP_MONITOR = ip_monitor.o

all: modemd ip_reg ip_monitor

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

modemd: $(OBJ_MODEMD)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lm
	
ip_reg: $(OBJ_IP_REG)
	$(CC) $(CFLAGS) -pthread -o $@ $^ -lm

ip_monitor: $(OBJ_IP_MONITOR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

install:
	install -m 0755 modemd $(DESTDIR)$(PREFIX)/bin
	install -m 0755 ip_reg $(DESTDIR)$(PREFIX)/bin
	install -m 0755 ip_monitor $(DESTDIR)$(PREFIX)/bin
	install -m 0777 restart_modem_gui.sh $(DESTDIR)$(PREFIX)/bin
	install -m 0777 ip_reg_default.sh $(DESTDIR)$(PREFIX)/bin
	install -m 0444 modem_filter.ftr $(DESTDIR)$(PREFIX)/share/rfsom-box-gui

clean:
	rm -rf modemd ip_reg *.o ip_monitor

