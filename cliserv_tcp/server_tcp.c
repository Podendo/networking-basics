/**
 * daytime service server example
 * 1. Set the server network address for the server
 * 2. Set a server port number (default port is 9099)
 * 3. Create a socket and the server socket address
 * 4. Bind the socket address, mark this socket as a listening socket
 * 5. Start the client service loop
 * 6. Accept a client connection
 * 7. Generate a date and time string and write it back to the client
 * 8. Close the client connection
 *
 * usage: $ make server
 *	  $ ./server &
 *	  telnet 127.0.0.1 9099
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_ADDRESS		"127.0.0.1"
#define DEFAULT_QUEUELEN	(10)

static void fail(const char *message)
{
	if(errno != 0) {
		fputs(strerror(errno), stderr);
		fputs(": ", stderr);
	}

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char **argv)
{
	int err, sock_srv, sock_cli, nbytes, len_inet;
	char dtbuf[128], *srv_addr = NULL, *srv_port = "9099";
	struct sockaddr_in addr_srv, addr_cli;
	time_t timedate;

	/*
	 * Use a server address from the command line, if one has been provided.
	 * Otherwise, this program will default to using the arbitrary address
	 * 127.0.0.1:
	 */
	if(argc >= 2) {
		srv_addr = argv[1];
	} else {
		srv_addr = DEFAULT_ADDRESS;
	}
	/*
	 * If there is a second argment on the command line, use it as the port:
	 */
	if(argc >= 3)
		srv_port = argv[2];

	/* Create a TCP/IP server (listening) socket to use: */
	sock_srv = socket(PF_INET, SOCK_STREAM, 0);
	if( sock_srv == -1)
		fail("socket()");

	/* Create a server socket address: */
	memset(&addr_srv, 0, sizeof(addr_srv));
	addr_srv.sin_family = AF_INET;
	addr_srv.sin_port = htons(atoi(srv_port));
	if(strcmp(srv_addr, "*") != 0) {
		/* Normal address */
		addr_srv.sin_addr.s_addr = inet_addr(srv_addr);
		if(addr_srv.sin_addr.s_addr == INADDR_NONE)
			fail("bad address.");
	} else {
		/*
		 * Wild address. A server address cannot completely wild.
		 * However, with a specified port number, the IP number
		 * portion of a TCP/IP address can be wild (INADDR_ANY).
		 * This allows the server to accept connections from any
		 * valid IP interface on  that host.
		 */
		addr_srv.sin_addr.s_addr = INADDR_ANY;
	}

	/* Bind the server address: */
	len_inet = sizeof(addr_srv);
	err = bind(sock_srv, (struct sockaddr *)&addr_srv, len_inet);
	if( err == -1 )
		fail("bind(2)");

	/* Make it a listening socket: */
	err = listen(sock_srv, DEFAULT_QUEUELEN);
	if( err == -1 )
		fail("listen(2)");

	/* This is the start of the server loop: */
	while(1) {
		/* Wait for a connection: */
		len_inet = sizeof(addr_cli);
		sock_cli = accept(sock_srv,
				(struct sockaddr *)&addr_cli,
				(struct socklen_t *)&len_inet);
		if(sock_cli == -1)
			fail("accept(2)");

		/* Generate a timestamp: */
		time(&timedate);
		nbytes = (int)strftime(dtbuf, sizeof(dtbuf),
				"%A %b %H:%M%S %Y\n", localtime(&timedate));
		/*
		 * Write result back to the client:
		 */
		err = write(sock_cli, dtbuf, nbytes);
		if( err == -1 )
			fail("write(2)");
		/*
		 * Close this client's connection.
		 */
		close(sock_cli);
	}
	/* Control never gets here */

	return 0;
}
