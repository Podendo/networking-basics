obj-srv:=server_tcp
obj-cli:=client_tcp
obj-clinew:=client_tcp_upd

CC:=gcc
DEFINES:=-D_GNU_SOURCE
CCFLAGS:=-Wall -Wextra
CCOPT:=-O0

all : server client newclient

client:
	@$(CC) -c $(obj-cli).c $(DEFINES) $(CCFLAGS)
	@$(CC) $(obj-cli).o -o $@

newclient:
	@$(CC) -c $(obj-clinew).c $(DEFINES) $(CCFLAGS)
	@$(CC) $(obj-clinew).o -o $@

server:
	@$(CC) -c $(obj-srv).c $(DEFINES) $(CCFLAGS)
	@$(CC) $(obj-srv).o -o $@

.PHONY: clean
clean:
	@rm -rf *o client server newclient
