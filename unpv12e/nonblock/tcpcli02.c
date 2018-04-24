#include	"unp.h"

//192.168.91.11 是客户端
//192.168.9.12 是echo服务
//
//tcpdump抓的br0是因为kvm做的一个桥
// tcpdump -i br0 -w tcpd  tcp and port 7
// ./tcpcli02 192.168.91.12 < /tmp/services >out 2>diag
// tcpdump -r  tcpd -N|sort diag - > /tmp/1
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
