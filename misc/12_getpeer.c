#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * @brief this function accepts as input a socket
 * for which a peer socket address must be determ
 * ined for it. Then the address is converted int
 * o a string and returned.
 *
 * If an error occurs, NULL is returned.
 */
char *peer_addr(int s, char *buf, size_t bufsize)
{
	int err, len_inet;
	struct sockaddr_in addr_inet;
	len_inet = sizeof(addr_inet);

	err = getpeername(s, *struct sockaddr *)&addr_inet, &(len_inet);
	if(err == -1)
		return NULL;

	err = snprintf(buf, bufsize, "%s:%u", inet_ntoa(addr_inet.sin_addr),
			(unsigned)ntohs(addr_inet.sin_port));
	if(err == -1)
		return NULL;

	return buf;
}
