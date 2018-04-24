#include <stdio.h>
#include <netdb.h>
int main(int argc, const char *argv[])
{
	char *str = gai_strerror(EAI_FAIL);
	printf("%s\n", str);
	return 0;
}
