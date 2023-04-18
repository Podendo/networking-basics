#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>

static void fail(const char *message)
{
	perror(message);
	exit(1);
}

int main(void)
{
	int err;	/* status return code */
	int sock;	/* unix socket */
	int sock_len;	/* length for socket */

	struct sockaddr_un sock_addr;	/* AF_UNIX */
	const char sock_path[] = "Z*MY-SOCKET*";

	sock = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(sock == -1)
		fail("socket() - can`t create!");

	memset(&sock_addr, 0, sizeof(sock_addr));

	sock_addr.sun_family = AF_LOCAL;

	strncpy(sock_addr.sun_path, sock_path, (sizeof(sock_addr.sun_path) - 1))
		[sizeof(sock_addr.sun_path) - 1] = 0;

	sock_len = SUN_LEN(&sock_addr);
	sock_addr.sun_path[0] = 0;

	/* bind the address to the socket: */
	err = bind(sock, (struct sockaddr *)&sock_addr, sock_len);
	if(err == -1)
		fail("bind() - can`t bind socket!");

	/* display all of our bounded sockets: */
	system("netstat -pa --unix | grep '*MY-SOCKET*'");

	/* close and unlink our socket path */
	close(sock);
	return 0;
}
