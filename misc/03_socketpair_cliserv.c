#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#ifndef SHUT_WR
#define SHUT_RD		(0)
#define SHUT_WR		(1)
#define SHUT_RDWR	(2)
#endif /* SHUT_WR */

int main(int argc, char **argv)
{
	int err, mlen, socks[2];
	char *pmsg;
	char buf[80];
	pid_t chpid;

	/* create a pair of local sockets */
	err = socketpair(AF_LOCAL, SOCK_STREAM, 0, socks);
	if(err == -1) {
		fprintf(stderr, "%s socketpair()\n", strerror(errno));
		exit(1);
	}

	/* now fork() into two processes: */
	if((chpid = fork()) == (pid_t)(-1)) {
		fprintf(stderr, "%s fork()\n", strerror(errno));
		exit(1);
	} else if(chpid == 0) {
		/* child process, client */
		char rxbuf[80];
		printf("parent PID is %ld\n", (long)getpid());

		close(socks[0]);	/* server uses socks[1] */
		socks[0] = -1;		/* forget this unit */

		/* form the message and its length */
		pmsg = "%A %d-%b-%Y %l:%M %p";
		mlen = strlen(pmsg);

		printf("child sending request '%s'\n", pmsg);
		fflush(stdout);

		/* write a request to the server: */
		err = write(socks[1], pmsg, mlen);
		if(err < 0) {
			fprintf(stderr, "%s write()\n", strerror(errno));
			exit(1);
		}

		/* now indicate that we will not be writing anything
		 * further to our socket, by shutting down the write
		 * side of the socket:
		 * */
		if( shutdown(socks[1], SHUT_WR) == -1) {
			fprintf(stderr, "%s: shutdown(2)\n", strerror(errno));
			exit(1);
		}

		/* receive the reply from the server: */
		err = read(socks[1], rxbuf, sizeof(rxbuf));
		if(err < 0) {
			fprintf(stderr, "%s: read(2)\n", strerror(errno));
			exit(1);
		}

		rxbuf[err] = 0; /* NULL-terminator for our buffer */

		/* report the results: */
		printf("Server returned '%s'\n", rxbuf);
		fflush(stdout);
		close(socks[1]); /* close out end now */
	} else {
		/* This is the parent process (server) */
		int status;	/* child termination status */
		char txbuf[80];	/* reply buffer */
		time_t td;	/* current day & time */
		printf("Child PID is %ld\n", (long)chpid);
		fflush(stdout);

		close(socks[1]);	/* client uses socks[1] */
		socks[1] = -1;		/* forget this descriptor */

		/* wait for a request from the client: */
		err = read(socks[0], buf, sizeof(buf));
		if(err < 0) {
			fprintf(stderr, "%s: read(2)\n", strerror(errno));
			exit(1);
		}

		buf[err] = 0; /* NULL-terminator */

		/* server function on the receiving message */
		time(&td);	/* get current time */

		strftime(txbuf, sizeof(txbuf), buf, localtime(&td));

		err = write(socks[0], txbuf, strlen(txbuf));
		if( err < 0) {
			fprintf(stderr, "%s: write(2)\n", strerror(errno));
			exit(1);
		}

		/* close our end of the socket */
		close(socks[0]);

		/* wait for the child process to exit */
		waitpid(chpid, &status, 0);
	}

	return 0;
}
