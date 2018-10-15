
CFLAGS=-std=c99 -pedantic -Wall -D_POSIX_SOURCE
DEPS = mac.h
OBJ_MODEMD = modemd.o mac.o
OBJ_IP_REG = ip_reg.o
OBJ_IP_MONITOR = ip_monitor.o
prefix=/usr/local

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
	install -m 0755 modemd $(prefix)/bin
	install -m 0755 ip_reg $(prefix)/bin
	install -m 0755 ip_monitor $(prefix)/bin
	install -m 0777 restart_modem_gui.sh $(prefix)/bin
	install -m 0777 ip_reg_default.sh $(prefix)/bin
	install -m 0444 modem_filter.ftr $(prefix)/share/rfsom-box-gui

clean:
	rm -rf modemd ip_reg *.o ip_monitor
