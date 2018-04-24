#include "processpool.h"


class cgi_conn
{
	public:
		cgi_conn(){}
		~cgi_conn(){}
		void init(int epollfd, int sockfd, const sockaddr_in& client_addr){
			m_epollfd = epollfd;
			m_sockfd = sockfd;
			m_address = client_addr;
			memset(m_buf, '\0', sizeof(m_buf));
			m_read_idx = 0;
		}

		void process(){
			int idx;
			int ret = -1;
			while (true) {
				idx = m_read_idx;
				ret = recv(m_sockfd, m_buf, BUFFER_SIZE-1-idx,0);
				if (ret <0) {
					if (errno!=EAGAIN) {
						removefd(m_epollfd, m_sockfd);
					}
					break;
				}else if(ret == 0){
					removefd(m_epollfd, m_sockfd);
					break;
				}else{
					m_read_idx += ret;
					printf("用户请求内容为:%s\n",m_buf);
					//如果遇到\r\n 开始进行处理"
					for (;idx < m_read_idx;idx++) {
						if ((idx>=1)&& (m_buf[idx-1]=='\r') &&(m_buf[idx]=='\n') ) {
							break;
						}
					}
					//请求中没有\r\n,需要继续读取数据
					if (idx==m_read_idx) {
						continue;
					}

					//开始处理用户请求
					m_buf[idx-1]= '\0';
					char *filename= m_buf;
					if (access(filename, F_OK)==-1) {
						removefd(m_epollfd, m_sockfd);
						break;
					}
					ret = fork();
					if (ret==-1) {
						removefd(m_epollfd, m_sockfd);
						break;
					}else if(ret > 0)  {
						removefd(m_epollfd, m_sockfd);
						close(m_sockfd);
						break;
					}else{
						close(STDOUT_FILENO);
						dup(m_sockfd);
						execl(m_buf,m_buf, 0);
						exit(0);
					}
				}
			}//end of while
		}

	private:
		static const int BUFFER_SIZE =1024;
		static int m_epollfd;
		int m_sockfd;
		sockaddr_in m_address;
		char m_buf[BUFFER_SIZE];
		int m_read_idx;//标记读入缓冲中 已经读入客户数据的最后一个字节的下一个位置
};

int cgi_conn::m_epollfd = -1;


/*
客户端:
[root@web11 15-2.poll_cgi]# ./client 127.0.0.1 1234
processpool
processpool.cpp
processpool.h

服务端:
[root@web11 15-1.processpool]# ./processpool 127.0.0.1 1234
*/
int main(int argc, const char *argv[])
{
	int listenfd;
	if( argc <= 2 )
	{
		printf( "usage: %s ip_address port_number\n", basename( argv[0] ) );
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi( argv[2] );

	int ret = 0;
	struct sockaddr_in address;
	bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
	inet_pton( AF_INET, ip, &address.sin_addr );
	address.sin_port = htons( port );

	listenfd = socket( PF_INET, SOCK_STREAM, 0 );
	assert( listenfd >= 0 );

	int val=1;
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,\
				&val, sizeof(val)) < 0){
		perror("setsockopt()");
		exit(0);
	}


	ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );

	ret = listen( listenfd, 5 );
	assert( ret != -1 );

	//processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd);
	processpool<cgi_conn> *pool = processpool<cgi_conn>::create(listenfd,2);//只创建2个子进程方便测试

	if (pool) {
		pool->run();
		delete pool;
	}
	close(listenfd);
	return 0;
}
