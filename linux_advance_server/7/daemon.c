#include <unistd.h>
#include <stdio.h>
 
int do_sth()
{
    //Add what u want
    return 0;
}
int main()
{
    daemon(0,0);
    while ( 1 )
    {
        do_sth();
        sleep(1);
    }
}
