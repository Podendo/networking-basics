/**
 * 09_makeaddr.c:
 * Demonstrate inet_lnaof, inet_netof
 * and inet_makeaddr (3) functions;
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
	int x;
	struct sockaddr_in addr_inet;
	unsigned long netid, hostid;
	const char *addr[] = {
		"44.135.86.12",
		"127.0.0.1",
		"172.16.23.95",
		"192.168.9.1"
	};

	for(x = 0; x < 4; ++x) {
		memset(&addr_inet, 0, sizeof(addr_inet));
		addr_inet.sin_family = AF_INET;
		addr_inet.sin_port = htons(9000);
		if( !inet_aton(addr[x], &addr_inet.sin_addr))
			puts("bad address.");

		hostid = inet_lnaof(addr_inet.sin_addr);
		netid = inet_netof(addr_inet.sin_addr);

		printf("%14s: net=0x%lx; host=0x%lx\n",
				inet_ntoa(addr_inet.sin_addr), netid, hostid);

		memset(&addr_inet, 0, sizeof(addr_inet));
		addr_inet.sin_family = AF_INET;
		addr_inet.sin_port = htons(9000);

		addr_inet.sin_addr = inet_makeaddr(netid, hostid);

		printf("%14s : %s\n\n", "inet_makeaddr",
				inet_ntoa(addr_inet.sin_addr));
	}

	return 0;
}
