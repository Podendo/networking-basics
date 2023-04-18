/**
 * Classify an IP address
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[])
{
	unsigned char msb;
	int index, len_inet;
	char class, *netmask;
	struct sockaddr_in addr_inet;
	static struct {
		unsigned char ip[4]
	} addresses[] = {
		{ { 44, 135, 86, 12 } },
		{ { 127, 0, 0, 1 } },
		{ { 172, 16, 23, 95 } },
		{ { 192, 168, 9, 1 } }
	};

	for( index = 0; index < 4; ++index ) {
		/* setting up th socket address to classify it */
		memset(&addr_inet, 0, sizeof(addr_inet));
		addr_inet.sin_family = AF_INET;
		addr_inet.sin_port = htons(9000);
		memcpy(&addr_inet.sin_addr.s_addr, addresses[index].ip, 4);
		len_inet = sizeof(addr_inet);

		msb = *(unsigned char *)&addr_inet.sin_addr.s_addr;

		if((msb & 0x80) == 0x00) {
			class = 'A';
			netmask = "255.0.0.0";
		} else if((msb & 0xC0) == 0x80) {
			class = 'B';
			netmask = "255.255.0.0";
		} else if((msb & 0xE0) == 0xC0) {
			class = 'C';
			netmask = "255.255.255.0";
		} else if((msb & 0xF0) == 0xE0) {
			class = 'D';
			netmask = "255.255.255.255";
		} else {
			class = 'E';
			netmask = "255.255.255.255";
		}
		printf("Addresses %u.%u.%u.%u is class %c; netmask %s\n",
				addresses[index].ip[0], addresses[index].ip[1],
				addresses[index].ip[2], addresses[index].ip[3],
				class, netmask);
	}

	return 0;
}
