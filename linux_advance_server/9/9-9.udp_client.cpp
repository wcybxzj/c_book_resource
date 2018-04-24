#include <sys/socket.h>
#include <stdio.h>
#include <libgen.h>
#include <assert.h>
#include <arpa/inet.h>
#include <string.h>

#define UDP_BUFFER_SIZE 80

int main(int argc, char *argv[]) {
	if( argc <= 2 ) {
		printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );
	struct sockaddr_in client_address;
	bzero( &client_address, sizeof( client_address ) );
	client_address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &client_address.sin_addr );
	client_address.sin_port = htons( port );
	int sockfd = socket( PF_INET, SOCK_DGRAM, 0 );
	assert( sockfd >= 0 );
	socklen_t client_addrlength = sizeof( client_address );
	int ret;
	char buf[UDP_BUFFER_SIZE];
	while ( fgets( buf, 80, stdin) ) {
		ret = sendto( sockfd, buf, UDP_BUFFER_SIZE-1, 0, ( struct sockaddr* )&client_address, client_addrlength );
		printf( "sendto %d\n", ret );
		if( ret > 0 ) {
			ret = recvfrom( sockfd, buf, UDP_BUFFER_SIZE-1, 0, ( struct sockaddr* )&client_address, &client_addrlength );
			printf( "recvfrom %d\n", ret );
			printf( "%s\n", buf );
		}
	}
}
