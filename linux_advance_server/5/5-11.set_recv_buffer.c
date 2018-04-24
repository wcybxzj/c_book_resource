#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	if ( argc <= 3 ) {
		printf("useage: %s ip port recv buffer size\n", basename( argv[0] ) );
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi( argv[2] );

	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr);
	address.sin_port = htons( port );

	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );
	int recvbuf = atoi( argv[3] );
	int len = sizeof( recvbuf );
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf , len );
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recvbuf , ( socklen_t* )&len );
	printf("the receive buffer size after setting is  %d", recvbuf);

	int ret = bind( sock, ( struct sockaddr* )&address ,sizeof( address )  );
	assert( ret != 1 );

	ret = listen( sock ,5 );
	assert( ret != 1 );

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	int connfd = accept( sock, ( struct sockaddr * )&client, &client_addrlength );
	if ( connfd < 0 ) {
		printf("errno is: %d\n", errno);
	}else {
		char buffer[ BUFFER_SIZE ];
		memset( buffer, '\0', BUFFER_SIZE );
		while ( recv( connfd, buffer, BUFFER_SIZE-1, 0 ) > 0 ) { }
		close( connfd );
	}

	close( sock );

	return 0;
}
