#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

union semun
{
	int val;
	struct semid_ds* buf;
	unsigned short int* array;
	struct seminfo* __buf;
};

void pv( int sem_id, int op )
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = op;
	sem_b.sem_flg = SEM_UNDO;
	semop( sem_id, &sem_b, 1 );
}

/*
https://www.cnblogs.com/guochuanrui/p/5677570.html
SEM_UNDO的意思:
当操作信号量(semop)时，sem_flg可以设置SEM_UNDO标识；
SEM_UNDO用于将修改的信号量值在进程正常退出（调用exit退出或main执行完）或异常退出（如段异常、除0异常、收到KILL信号等）时归还给信号量。
如信号量初始值是20，进程以SEM_UNDO方式操作信号量减2，减5，加1；在进程未退出时，信号量变成20-2-5+1=14；在进程退出时，将修改的值归还给信号量，信号量变成14+2+5-1=20。
*/
int main( int argc, char* argv[] )
{
	int sem_id = semget( IPC_PRIVATE, 1, 0666 );
	union semun sem_un;
	sem_un.val = 1;
	semctl( sem_id, 0, SETVAL, sem_un );
	pid_t id = fork();
	if( id < 0 )
	{
		return 1;
	}
	else if( id == 0 )
	{
		printf( "child try to get binary sem\n" );
		pv( sem_id, -1 );
		printf( "child get the sem and would release it after 5 seconds\n" );
		sleep( 5 );
		pv( sem_id, 1 );
		exit( 0 );
	}
	else
	{
		printf( "parent try to get binary sem\n" );
		pv( sem_id, -1 );
		printf( "parent get the sem and would release it after 5 seconds\n" );
		sleep( 5 );
		pv( sem_id, 1 );
	}

	waitpid( id, NULL, 0 );
	semctl( sem_id, 0, IPC_RMID, sem_un );
	return 0;
}
