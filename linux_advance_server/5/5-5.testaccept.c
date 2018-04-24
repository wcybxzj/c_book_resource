#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>
#include <assert.h>


int main(int argc, char *argv[])
{
	if (argc <= 2) {
		printf("usage: %s ip port\n",basename( argv[0] ));
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert( sock >= 0 );

	int ret = bind( sock, (struct sockaddr*)&address, sizeof( address ));
	assert( ret >= 0 );

	ret = listen( sock, 5);
	assert( ret >= 0 );

	sleep(30);//等待客户端掉线或者退出
	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof( client );
	int connfd =accept( sock, ( struct sockaddr* )&client, &client_addrlength );
	if (connfd < 0 ) {
		printf("errno is %d\n", errno);
	}else{
		char remote[INET_ADDRSTRLEN];
		printf("connected with ip: %s and port %d\n",
				inet_ntop( AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN ),ntohs( client.sin_port ) );
		close( connfd );
	}

	return 0;
}
