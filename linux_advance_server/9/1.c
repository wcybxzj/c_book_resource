#include <stdio.h>
int main()
{
	char s[80];
	fgets(s, 80, stdin);
	printf("%s\n", s);
	return 1;
}
