/**
 * Example getservent(3) program:
 *	1. Calls getservent(3) to obtain an entry from the /etc/services file.
 *	2. Prints the service name, port and protocol.
 *	3. In an internal loop, printfs all alias names, if any.
 *	4. Repeats step 1, until there are no more entries.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>		/* ntohs() */
#include <netdb.h>		/* struct servent */

int main(int argc, char **argv)
{
	int x;
	struct servent *sp;
	while(1) {
		errno = 0;
		if( !(sp = getservent()))
			break;

		printf("%s:\n\tPort: %d\n\tProtocol: %s\n\tAliases:",
				sp->s_name, ntohs(sp->s_port), sp->s_proto);

		for(x = 0; sp->s_aliases[x] != NULL; ++x)
			printf("%s ", sp->s_aliases[x]);

		putchar('\n');
	}

	if((errno != 0) && (errno != ENOENT))
		fprintf(stderr, "%s: getservent(3) %d\n",
				strerror(errno), errno);

	return 0;
}
