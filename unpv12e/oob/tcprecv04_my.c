#include	"unp.h"
int connfd;

void sig_urg(int signo);

//模仿tcprecv04唯一的区别是没使用 SO_OOBINLINE 让带外数据在带内存储
//而是不设置SO_OOBINLINE 让带外数据在带外存储
int
main(int argc, char **argv)
{
	int		listenfd,n;
	char	buff[100];

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], NULL);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], NULL);
	else
		err_quit("usage: tcprecv04 [ <host> ] <port#>");

	//Setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on));

	connfd = Accept(listenfd, NULL, NULL);
	Signal(SIGURG, sig_urg);
	Fcntl(connfd, F_SETOWN, getpid());
	sleep(5);

	for ( ; ; ) {
		if (Sockatmark(connfd))
			printf("at OOB mark\n");

		if ( (n = Read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
}

void
sig_urg(int signo)
{
	int		n;
	char	buff[100];

	printf("SIGURG received\n");
	n = Recv(connfd, buff, sizeof(buff)-1, MSG_OOB);
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}
