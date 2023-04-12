#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	int err, socks[2];

	err = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
	if(err == -1) {
		fprintf(stderr, "%s: socketpair local\n", strerror(errno));
		return 1;
	}
	/* returned file descriptors: */
	printf("sock[0] = %d;\n", socks[0]);
	printf("sock[1] = %d;\n", socks[1]);

	system("netstat --unix -p | grep socketpair");

	return 0;
}
