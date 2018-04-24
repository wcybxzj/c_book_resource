#include <stdio.h>
#include <libgen.h>


#define BUFFER_SIZE 4096
enum CHECK_STATE{ CHECK_STATE_REQUESTLINE = 0, CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN }
enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, FORBIDDEN_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
static const char* szret[] = { "I get a correct  rresult\n", "something wrong\n" };

LINE_STATUS parse_line( char* buffer, int& checked_index, int& read_index){
	char temp;
	for ( ; checked_index < read_index; ++checked_index ) {
		temp = buffer[ checked_index ];
		if ( temp == '\r' ) {
			if ( ( checked_index + 1 ) == read_index ) {
				return LINE_OPEN;
			} else if( buffer[ checked_index + 1] == '\n' ){
				buffer[ checked_index++ ] = '\0';
				buffer[ checked_index++ ] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}else if( temp == '\n' ) {
			if ( ( checked_index > 1 ) && buffer[ checked_index - 1 ] == '\r' ) {
				buffer[ checked_index-1 ] = '\0';
				buffer[ checked_index++ ] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
	}
	return LINE_OPEN;
}

HTTP_CODE parse_requestline( char* szTemp, CHECK_STATE& checkstate ){
	char* szURL = strpbrk( szTemp, " \t");
	if ( ! szURL ) {
		return BAD_REQUEST;
	}
	*szURL++ = '\0';
	char* szMethod = szTemp;
	if ( strcasecmp( szMethod, "GET" ) == 0 ) {
		printf("The request method is GET \n");
	}else {
		return BAD_REQUEST;
	}
	szURL += strspn( szURL, " \t" );
	char* szVersion = strpbrk( szURL, " \t");
	if ( ! szVersion ) {
		return BAD_REQUEST;
	}
	*szVersion++ = '\0';
	szVersion += strspn( szVersion, " \t" )
}

HTTP_CODE parse_content( char* buffer, int& checked_index, CHECK_STATE& checkstate, int& read_index, int& start_line ){
	LINE_STATUS linestatus = LINE_OK;
	HTTP_CODE retcode = NO_REQUEST;
	while ( ( linestatus = parse_line( buffer, checked_index, read_index ) ) == LINE_OK) {
		char* szTemp = buffer + start_line;
		start_line = checked_index;
		switch( checkstate )
		{
			case CHECK_STATE_REQUESTLINE:
			{
				retcode = parse_requestline( szTemp, checkstate );
				if ( retcode == BAD_REQUEST ){
					//TODO
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if ( argc <= 2 ) {
		printf("usage: %s ip port\n", basename( argv[0] ) );
		return 1;
	}

	const char* ip = argv[1];
	int port = atoi( argv[2] );

	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip , &address.sin_addr );
	address.sin_port = htons( port );

	int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd >= 0 );
	int ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );
	ret = listen( listenfd, 5 );
	assert( ret != -1 );

	struct sockaddr_in client_address;
	socklen_t client_addrlength = sizeof( client_address );
	int fd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
	if ( fd < 0 ) {
		printf("errno is: %d\n", errno );
	}else {
		char buffer[ BUFFER_SIZE ];
		memset( buffer, '\0', BUFFER_SIZE );
		int data_read = 0;
		int read_index = 0;
		int checked_index = 0;
		int start_line = 0;
		enum CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;
		while ( 1 ) {
			data_read = recv( fd, buffer + read_index, BUFFER_SZIE - read_index, 0 );
			if ( data_read == -1) {
				printf("reading failed\n");
				break;
			}else {
				printf("remote client has closed the connection\n");
				break;
			}
			read_index += data_read;
			HTTP_CODE result = parse_content( buffer, checked_index, checkstate, read_index, start_line );
		}
	}
	return 0;
}
