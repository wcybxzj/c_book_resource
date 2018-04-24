#include <stdio.h>
int main(int argc, const char *argv[])
{
	union{
		short value;
		char union_bytes[ sizeof( short ) ];
	} test;
	test.value = 0x0102;
	printf("%d\n", test.union_bytes[0]);
	return 0;
}
