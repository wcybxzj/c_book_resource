#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SIZE 11
void func1(int sd){
	char str[100]="/bin/ls\r\n";
	send(sd, str, strlen(str),0);

	memset(str,'\0',sizeof(str));
	while (read(sd, str, sizeof(str))>0) {
		printf("%s\n",str);
	}

	close(sd);
}

int main(int argc, const char *argv[])
{
	int sd;
	struct sockaddr_in saddr;
	if(argc!=3){
		perror("./a.out ip port");
		exit(-1);
	}

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0) {
		perror("socket");
		exit(-2);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));
	if(inet_pton(AF_INET, argv[1], &saddr.sin_addr) != 1){
		perror("inet_pton()");
		exit(0);
	}

	if(connect(sd, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
		perror("connect()");
		exit(-2);
	}

	func1(sd);
	//func2(sd);

	return 0;
}
