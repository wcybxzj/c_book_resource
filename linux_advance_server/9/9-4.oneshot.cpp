#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024
const char* oneshot;
struct fds{
	int epollfd;
	int sockfd;
};
int setnonblocking( int fd ){
	int old_option = fcntl( fd, F_GETFL );
	int new_option = old_option | O_NONBLOCK;
	fcntl( fd, F_SETFL, new_option );
	return old_option;
}
void addfd( int epollfd, int fd, int oneshot ){
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	if ( oneshot ) {
		event.events |= EPOLLONESHOT;
	}
	epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
	setnonblocking( fd );
}
void reset_oneshot( int epollfd, int fd ){
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}
void* worker( void* arg ){
	int sockfd = ( (fds*)arg )->sockfd;
	int epollfd = ( (fds*)arg )->epollfd;
	printf("start new thread to receive data on fd: %d\n", sockfd );
	char buf[ BUFFER_SIZE ];
	memset( buf, '\0', BUFFER_SIZE );
	while ( 1 ) {
		printf( "006:block recv\n" );
		int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0 );
		printf( "007:unblock recv\n" );
		if ( ret == 0 ) {
			close( sockfd );
			printf( "client closed the connection\n" );
			break;
		}else if ( ret < 0 ) {
			if ( errno == EAGAIN ) {
				printf("EAGAIN\n");
				if ( strncmp( "oneshot", oneshot,2 ) == 0 ) {
					reset_oneshot( epollfd, sockfd );
				}
				printf("read later\n");
				break;
			}
		}else {
			pthread_t   tid;
			tid = pthread_self();
			printf("thread id is %u\n", (unsigned int)tid);
			printf("get content: %s\n", buf);
			sleep( 15 );
		}
	}
}
int main(int argc, char *argv[])
{
	if ( argc <= 3 ) {
		printf("useage: %s ip port is_oneshot\n", basename( argv[0] ) );
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );
	oneshot = argv[3];
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );
	int ret = 0;
	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd >= 0 );
    int reuse = 1;
    setsockopt( listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
	ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );
	ret = listen( listenfd, 5 );
	assert( ret != -1 );
	epoll_event events[ MAX_EVENT_NUMBER ];
	int epollfd = epoll_create( 5 );
	assert( epollfd != -1 );
	addfd( epollfd, listenfd, 0 );
	while ( 1 ) {
		printf( "001:blocked epoll_wait\n" );
		int ret = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
		printf( "002:unblocked epoll_wait\n" );
		if ( ret < 0 ) {
			printf("epoll failure\n");
			break;
		}
		printf( "003:ret %d\n", ret );
		for (int i = 0; i < ret; i++) {
			int sockfd = events[i].data.fd;
			if ( sockfd == listenfd ) {
				printf( "004: sockfd == listenfd  \n" );
				struct sockaddr_in client_address;
				socklen_t client_addrlength = sizeof( client_address );
				int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
				if ( strncmp( "oneshot", oneshot,2 ) == 0 ) {
					addfd( epollfd, connfd, 1 );
				}else {
					addfd( epollfd, connfd, 0 );
				}
			}else if ( events[i].events & EPOLLIN ) {
				printf( "005:events[i].events & EPOLLIN %d\n", i );
				pthread_t thread;
				fds fds_for_new_worker;
				fds_for_new_worker.epollfd = epollfd;
				fds_for_new_worker.sockfd =sockfd;
				pthread_create( &thread, NULL, worker, ( void* )&fds_for_new_worker );
			}else {
				printf("somethig else happened\n");
			}
		}
	}
	close( listenfd );
	return 0;
}
