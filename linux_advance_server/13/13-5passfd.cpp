#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define MAXSIZE 4096

static const int CONTROL_LEN = CMSG_LEN( sizeof(int) );

void send_fd( int fd, int fd_to_send )
{
	struct iovec iov[1];
	struct msghdr msg;
	char buf[4096]="abc";

	iov[0].iov_base = buf;
	iov[0].iov_len = strlen(buf);
	msg.msg_name    = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov     = iov;//iov数组指针
	msg.msg_iovlen = 1;//有几个iov数组里的元素

	//必须malloc申请空间
	struct cmsghdr *cmptr = (struct cmsghdr *)malloc(CONTROL_LEN);
	cmptr->cmsg_level  = SOL_SOCKET;
	cmptr->cmsg_type   = SCM_RIGHTS;
	cmptr->cmsg_len    = CONTROL_LEN;
	msg.msg_control    = cmptr;
	msg.msg_controllen = CONTROL_LEN;
	*(int *)CMSG_DATA(cmptr) = fd_to_send;
	sendmsg( fd, &msg, 0 );
}

int recv_fd( int fd )
{
	int ret;
	struct iovec iov[1];
	struct msghdr msg;
	char buf[MAXSIZE];

	iov[0].iov_base = buf;
	iov[0].iov_len  = sizeof(buf);
	msg.msg_name    = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	//必须malloc申请空间
	struct cmsghdr *cmptr= (struct cmsghdr*)malloc(CONTROL_LEN);
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROL_LEN;
	printf("recvmsg\n");
	ret = recvmsg( fd, &msg, 0 );
	printf("recvmsg() ret:%d\n",ret);
	printf("recv 普通数据:%s\n",buf);
	int fd_to_read = *(int *)CMSG_DATA( cmptr );
	printf("recv 辅助数据:%d\n", fd_to_read);
	return fd_to_read;
}

//子进程打开的文件描述符传递给父进程
//需要注意的是传递描述符并不是简单传递一个 int 型的描述符编号，
//而是在接收进程中创建一个新的描述符，并且在内核的文件表中，
//它与发送进程发送的描述符指向相同的项。(具体看apue17.4和tlpi61.13.3)
int main()
{
	int pipefd[2];
	int fd_to_pass = 0;

	int ret = socketpair( PF_UNIX, SOCK_DGRAM, 0, pipefd );
	assert( ret != -1 );

	pid_t pid = fork();
	assert( pid >= 0 );

	if ( pid == 0 )
	{
		printf("child pid:%d\n", getpid());
		close( pipefd[0] );
		fd_to_pass = open( "./test.txt", O_RDWR, 0666 );
		if (fd_to_pass==-1) {
			printf("open error\n");
			exit(1);
		}
		printf("send_fd\n");
		send_fd( pipefd[1], ( fd_to_pass > 0 ) ? fd_to_pass : 0 );
		close( fd_to_pass );
		exit( 0 );
	}
	printf("parent pid:%d\n", getpid());

	close( pipefd[1] );
	fd_to_pass = recv_fd( pipefd[0] );
	char buf[1024];
	memset( buf, '\0', 1024 );
	read( fd_to_pass, buf, 1024 );
	printf( "I got fd %d and data %s\n", fd_to_pass, buf );
	close( fd_to_pass );
}
