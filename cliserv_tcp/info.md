# TCP/IP sockets

using TCP/IP for a connected pair of sockets requires that a slightly different
procedure be used from the one was used when  using the UDP protocol. From  the
client program's point of view, you must perform the following general steps:

1. Create a socket
2. Optionally bind the socket ( to restrict which interface will be used, or to
	explicitly indicate a wild socket address)
3. Connect to the remote socket (client connects to the server) (TCP diff)
4. Communicate with reads and writes
5. Shut down or close the socket.


# Connection-oriented protocols for servers

The basic steps of the server can now be summarized as follows:

1. Create a socket
2. Bind a socket address (mandatory)
	The server cannot be contacted by clients unless the client has an addr-
	ess for connectiong. On the other hand, the clinet bind(2) is optional.
	In order for a server to be contacted, then, a server address cannot be
	completely wild.
3. Listen for a connection
	The server must express its internet in accepting connections to the ker
	nel for its socket.
4. Accept a connection
	Once a client has connected to the server, the server must then accept
	the connection.
5. Communicate with the client program
6. Close the socket

Briefly, the server differs from the client in the following ways:
	* The server must bind a server address to the socket
	* The server listens for a connection
	* The server accepts connections

## The listen(2) function call

```
#include <sys/socket.h>

int listen(int s, int backlog)
/* there s - socket to use as the listening socket */
/* backlog - specifies the connect queue length in entries */
```

## The accept(2) function call

```
#include <sys/types.h>
#include <sys/socket.h>

int accept(int s, struct sockaddr *addr, int *addrlen)
/* there input socket s is listening for connections */
/* the pointer addr to a socket address structure,
	which will receive the client socket address */
/* the pointer to the maximum length (addrlen) of the
	receiving socket address buffer - (addr). The
	int value that this pointer is pointing to is
	both an input and output value. Upon input, it
	specifies the maximum length of addr. After the
	function returns, this value contains the actual
	length of the address that was returned. */

To summarize, there are two types of sockets used by the server program:
	* Sockets that are being used for listening. No reading or writing
		of data to these sockets is permitted.
	* Sockets that have been returned by accept(2). These are connected
		to a client process, and can be used to read and write data.
So, the input socket to the accept(2) call must be a listening socket. Any atte-
mpt to use a non-listening socket will cause the function to return an error.
