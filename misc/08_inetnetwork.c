/**
 * Example using inet_network(3):
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	int x;
	unsigned long net_addr;
	const char *addr[] = {
		"44.135.86.12",
		"127.0.0.1",
		"172.16.23.95",
		"192.168.9.1"
	};

	for(x = 0; x < 4; ++x) {
		net_addr = inet_network(addr[x]);
		printf("%14s = 0x%lx net 0x%lx\n",
				addr[x], net_addr, (unsigned long)htonl(net_addr));
	}

	return 0;
}
