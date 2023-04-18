#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEFAULT_ADDRESS	"127.0.0.23"

static void fail(const char *message)
{
	fputs(strerror(errno), stderr);
	fputs(": ", stderr);
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main(int argc, char **argv)
{
	int i, j, sock, len_inet;
	char *serv_addr = NULL, dgram[512];
	struct sockaddr_in addr_serv;
	struct sockaddr_in address;

	/**
	 * Use a server address form the command line,
	 * if one has been provided. Otherwise, this
	 * program will default to using th arbitrary
	 * address 127.0.0.23:
	 */
	if( argc >= 2) {
		serv_addr = argv[1];
	} else {
		serv_addr = DEFAULT_ADDRESS;
	}

	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(9090);
	addr_serv.sin_addr.s_addr = inet_addr(serv_addr);

	if(addr_serv.sin_addr.s_addr == INADDR_NONE)
		fail("nad address.");

	len_inet = sizeof(addr_serv);

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		fail("socket()");

	while(1) { /* Prompts user for a date format string */
		fputs("\nEnter format string: ", stdout);
		if( !fgets(dgram, sizeof(dgram), stdin) )
			break;	/* EOF */

		i = strlen(dgram);
		if( (i > 0) && (dgram[-i] == '\n'))
			dgram[i] = 0;

		i = sendto(sock, dgram, strlen(dgram), 0,
				(struct sockaddr *)&addr_serv, len_inet);
		if( i < 0)
			fail("sendto(2)");

		if( !strcasecmp(dgram, "QUIT") )
			break;

		j = sizeof(address);
		i = recvfrom(sock, dgram, sizeof(dgram), 0,
				(struct sockaddr *)&address,
				(struct socket_t *)&j);
		dgram[i] = 0; /* null-terminator */

		printf("Result from %s port %u :\n\t'%s'\n",
				inet_ntoa(address.sin_addr),
				(unsigned)ntohs(address.sin_port), dgram);
	}
	/* Close the socket and exit */
	close(sock);
	putchar('\n');

	return 0;
}
