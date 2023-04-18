/**
 * daytime service client example
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_ADDRESS	"127.0.0.1"

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
	int err, len_inet, sock;
	char dtbuf[128];
	char *srvr_addr = NULL;
	char *srvr_port = "9099";
	struct sockaddr_in addr_srvr;

	/**
	 * Use a server address from the command line,
	 * if one has been provided. Otherwise,  this
	 * application will default to using the arb-
	 * itrary address 127.0.0.1 (localhost):
	 */
	if(argc >= 2) {
		srvr_addr = argv[1];
	} else {
		srvr_addr = DEFAULT_ADDRESS;
	}
	if(argc >= 3)
		srvr_port = argv[2];

	/* Create a TCP/IP socket to use: */
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if( sock == -1 )
		fail("socket()");

	/* Create a server socket address: */
	memset(&addr_srvr, 0, sizeof(addr_srvr));
	addr_srvr.sin_family = AF_INET;
	addr_srvr.sin_port = htons(atoi(srvr_port));
	addr_srvr.sin_addr.s_addr = inet_addr(srvr_addr);

	if(addr_srvr.sin_addr.s_addr == INADDR_NONE)
		fail("bad address.");

	/* Connect to the server: */
	len_inet = sizeof(addr_srvr);

	err = connect(sock, &addr_srvr, len_inet);
	if(err == -1)
		fail("connect(2)");

	/* Read the daytime info: */
	err = read(sock, &dtbuf, (sizeof(dtbuf) - 1));
	if(err == -1)
		fail("read(2)");

	/* Report the Date & TIme: */
	dtbuf[err] = 0; /* NULL-terminate string */
	printf("Date and Time is %s\n", dtbuf);

	/* Close the socket and exit: */
	close(sock);
	putchar('\n');

	return 0;
}
/**
 * HINT: the connect() function (line :69) can be used with UDP
 * sockets (SOCK DGRAM for protocol falimy PF_INET). This does
 * not actually imply a connection, however, but instead imposes
 * some restrictions upon the UDP socket will be used. This func
 * tion call should be made:
 * 1. After the bind(2) function is called,if used.
 * 2. Before any datagrams are read or written to the socket.
 * It is used form receiving UDP datagrams form th same server.
 * This prevents stray datagrams from being received from other
 * sockets, which is itherwise possible with UDP.
 * Also, it permits the caller to use the read(2) and write(2)
 * functions instead of the sendto(2) recvfrom(2). This saves
 * the programmer from having to proved socket address structs
 * and lengths for each I/O operation.
 */
