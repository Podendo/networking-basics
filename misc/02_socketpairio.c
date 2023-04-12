#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char **argv)
{
	int err, sockets[2];
	char *offet;
	char buff[80];

	err = socketpair(AF_LOCAL, SOCK_STREAM, 0, sockets);
	if(err == -1) {
		fprintf(stderr, "%s: socketpair(AF_LOCAL)\n", strerror(errno));
		return 1;
	}

	err = write(sockets[1], offet="Hello?", 6);
	if( err < 0) {
		fprintf(stderr, "%s: read(socket[1])\n", strerror(errno));
		return 2;
	}

	err = read(sockets[0], buff, sizeof(buff));
	if( err < 0) {
		fprintf(stderr, "%s: write(socket[0])\n", strerror(errno));
		return 3;
	}

	buff[err] = 0; /* NULL-Terminated symbol */
	printf("Received message: '%s' from socket[0])\n", buff);

	err = write(sockets[0], offet="Go away!", 8);
	if(err < 0) {
		fprintf(stderr, "%s: write(socket[0])\n", strerror(errno));
		return 4;
	}
	printf("Wrote message: '%s' on socket[0]\n", offet);

	err = read(sockets[1], buff, sizeof(buff));
	if(err < 0) {
		fprintf(stderr, "%s: read(sockets[1])\n", strerror(errno));
		return 3;
	}

	buff[err] = 0; /* NULL-terminated symbol */
	printf("Received message '%s' from socket[1]\n", buff);

	close(sockets[0]);
	close(sockets[1]);

	puts("Done.");

	return 0;
}
