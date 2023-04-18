obj-srv:=server_dgram
obj-cli:=client_dgram

CC:=gcc
DEFINES:=-D_GNU_SOURCE
CCFLAGS:=-Wall -Wextra
CCOPT:=-O0

all : server client

client:
	$(CC) -c $(obj-cli).c $(DEFINES) $(CCFLAGS)
	$(CC) $(obj-cli).o -o $@

server:
	$(CC) -c $(obj-srv).c $(DEFINES) $(CCFLAGS)
	$(CC) $(obj-srv).o -o $@

.PHONY: clean
clean:
	rm -rf *o client server
