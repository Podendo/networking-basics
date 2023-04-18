#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/x25.h>


static void fail(const char *message)
{
	perror(message);
	exit(1);
	return;
}

int main(void)
{
	int err, sock_x25, len_x25;
	const char x25_host[] = "79400900";
	struct sockaddr_x25 addr_x25;

	/* create an AF_X25 socket */
	sock_x25 = socket(AF_X25, SOCK_SEQPACKET, 0);
	if(sock_x25 == -1)
		fail("socket()");

	/* form an AF_X25 address */
	addr_x25.sx25_family = AF_X25;
	strcpy(addr_x25.sx25_addr.x25_addr, x25_host);
	len_x25 = sizeof(addr_x25);

	/* bind the address to the socket */
	err = bind(sock_x25, (struct sockaddr *)&addr_x25, len_x25);
	if( err == -1)
		fail("bind()");

	puts("X.25 SOCKETS:");
	system("cat /proc/net/x25");

	return 0;
}
