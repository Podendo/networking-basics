#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void fail(const char *message)
{
	perror(message);
	exit(1);
	return;
}

int main(void)
{
	int err, sock_inet, len_inet;
	const unsigned char ipno[] = { 127, 0, 0, 23 };
	struct sockaddr_in addr_inet;

	/* create an ipv4 internet socket */
	sock_inet = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_inet == -1)
		fail("sock() - creation failed!");

	/* create an AF_INET address */
	memset(&addr_inet, 0, sizeof(addr_inet));
	addr_inet.sin_family = AF_INET;
	addr_inet.sin_port = htons(9000);
	memcpy(&addr_inet.sin_addr.s_addr, ipno, 4);
	len_inet = sizeof(addr_inet);

	/* bind the address to the socket */
	err = bind(sock_inet, (struct sockaddr *)&addr_inet, len_inet);
	if( err == -1)
		fail("bind() - socket binding failed!");

	/* dispay all of our bound sockets */
	system("netstat -pa --tcp");
	close(sock_inet);

	return 0;
}
