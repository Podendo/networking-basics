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

char *sock_addr(int s, char *buf, size_t bufsize)
{
	int err, len_inet;
	struct sockaddr_in addr_inet;

	len_inet = sizeof(addr_inet);

	err = getsockname(s, (struct sockaddr *)&addr_inet,
			(socklen_t *)&len_inet);
	if(err == -1)
		return NULL;

	snprintf(buf, bufsize, "%s:%u", inet_ntoa(addr_inet.sin_addr),
			(unsigned)ntohs(addr_inet.sin_port));

	return buf;
}

int main(int argc, char **argv, char **envp)
{
	int err, sock_inet, len_inet;
	struct sockaddr_in addr_inet;
	char buf[64];

	sock_inet = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_inet == -1)
		fail("socket");

	memset(&addr_inet, 0, sizeof(addr_inet));
	addr_inet.sin_family = AF_INET;
	addr_inet.sin_port = htons(9000);
	inet_aton("127.0.0.24", &addr_inet.sin_addr);
	len_inet = sizeof(addr_inet);

	err = bind(sock_inet, (struct sockaddr *)&addr_inet, len_inet);
	if(err == -1)
		fail("bind()");

	/* socket testing function */
	if( ! sock_addr(sock_inet, buf, sizeof(buf)))
		fail("sock_addr()");
	printf("Address is: '%s'\n", buf);
	close(sock_inet);

	return 0;
}
