/**
 * 1. Create a socket using socket(2)
 * 2. Establish a server address and bind(2) it.
 * 3. Wait for a datagram by calling recvfrom(2).
 * 4. Process the request (format a date string).
 * 5. Send the result back to the clinet using sendto(2).
 * 6. Until it is time to exit, repeat loop 3 - 6.
 */
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
	return;
}

int main(int argc, char **argv)
{
	int err, len_inet, sock;
	char dgram[512], dtfmt[512], *srvr_addr = NULL;
	struct sockaddr_in addr_inet, addr_client;
	struct tm tm;
	time_t td;

	if (argc >= 2) {
		srvr_addr = argv[1];
	} else {
		/* this default address will work locally, even
		 * if you do not have any network cards installed.
		 */
		srvr_addr = DEFAULT_ADDRESS;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		fail("socket()");

	memset(&addr_inet, 0, sizeof(addr_inet));
	addr_inet.sin_family = AF_INET;
	addr_inet.sin_port = htons(9090);
	addr_inet.sin_addr.s_addr = inet_addr(srvr_addr);

	if(addr_inet.sin_addr.s_addr == INADDR_NONE)
		fail("bad address.");

	len_inet = sizeof(addr_inet);

	err = bind(sock, (struct sockaddr *)&addr_inet, len_inet);
	if(err == -1)
		fail("bind()");

	while(1) { /* wait for requests */
		/**
		 * Block until the program receives a
		 * datagram at our address and port:
		 */
		len_inet = sizeof(addr_client);

		/* This function call will block until the server receives
		 * a datagram. Upon return from this function, buffer dgram
		 * will contain the datagram, err will contain the size of it.
		 */
		err = recvfrom(sock, dgram, sizeof(dgram), 0,
				(struct sockaddr *)&addr_client,
				(struct socklen_t *)&len_inet);
		if(err < 0)
			fail("recvfrom(2)");

		/* Process the request: */
		dgram[err] = 0;	/* NULL-terminate */
		if( !strcasecmp(dgram, "QUIT"))
			break;

		/* get the current date and time: */
		time(&td);
		tm = *localtime(&td);
		strftime(dtfmt, sizeof(dtfmt), dgram, &tm);

		/* Send the formatted result back to the client program: */
		err = sendto(sock, dtfmt, strlen(dtfmt), 0,
				(struct sockaddr *)&addr_client, len_inet);
		if( err < 0)
			fail("sendto(2)");
	}

	/* close the socket and exit: */
	close(sock);

	return 0;
}
