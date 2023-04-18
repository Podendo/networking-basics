/**
 * Choosing  a socket type:
 * PF_LOCAL - PF_UNIX - local UNIX socket protocol family (domain)
 * PF_INET - Internet family protocols (domain)
 *
 * SOCK_STREAM (normal use)
 * SOCK_DGRAM (normal use)
 * SOCK_SEQPACKET (non-internet protocols e.x. AX.25, X.25)
 * SOCK_RAW - raw interface to the socket (proto-packet-dependent)
 *
 * the fird argument is a protocol, 0 - for auto, see protocols(5)
 *
 * Domain	Socket type	Protocol	Description
 * PF_LOCAL	SOCK_STREAM	0		Local stream socket
 * PF_LOCAL	SOCK_DGRAM	0		Local datagram socket
 * PF_INET	SOCK_STREAM	IPPROTO_TCP	TCP/IP stream socket
 * PF_INET	SOCK_DGRAM	IPPROTO_UDP	UDP datagram socket
 *
 * Demonstrating the bind(2) function by establishing
 * a Specific AF_INET socket address
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

static void fail(const char *message)
{
	perror(message);
	exit(1);
	return;
}

int main(int argc, char **argv, char **envp)
{
	int err, sock_inet, len_inet;
	struct sockaddr_in addr_inet;

	sock_inet = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_inet == -1)
		fail("socket()");

	memset(&addr_inet, 0, sizeof(addr_inet));
	addr_inet.sin_family = AF_INET;
	addr_inet.sin_port = htons(9000);

	inet_aton("127.0.0.24", &addr_inet.sin_addr);
	len_inet = sizeof(addr_inet);

	err = bind(sock_inet, (struct sockaddr *)&addr_inet, len_inet);
	if( err == -1)
		fail("bind()");

	system("netstat -pa --tcp");

	close(sock_inet);
	return 0;
}
