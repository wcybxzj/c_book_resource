#include <stdio.h>
#include <unistd.h>
static bool switch_to_user( uid_t user_id, gid_t gp_id )
{
	if ( ( user_id == 0 ) && ( gp_id == 0 ) )
	{
		return false;
	}
	gid_t gid = getgid();
	uid_t uid = getuid();
	printf( "current userid is %d, getgid is: %d\n", uid, gid);
	if ( ( ( gid != 0 ) || ( uid != 0 ) ) && ( ( gid != gp_id ) || ( uid != user_id ) ) )
	{
		return false;
	}
	if ( uid != 0 )
	{
		return true;
	}
	if ( ( setgid( gp_id ) < 0 ) || ( setuid( user_id ) < 0 ) )
	{
		return false;
	}
	return true;
}
int main(int argc, const char *argv[])
{
	switch_to_user( 501, 20);
	uid_t uid = getuid();
	uid_t gid = getgid();
	printf( "change to userid is %d, getgid is: %d\n", uid, gid);
	return 0;
}
