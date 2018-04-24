#include <stdio.h>
#include <unistd.h>
int main(int argc, const char *argv[])
{
	pid_t pid = getpid();
	printf( "%d\n", pid );
	pid_t pgid = getpgid( pid );
	printf( "%d\n", pgid );
	return 0;
}
