/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2016.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 34-4 */

/* disc_SIGHUP.c

   This program demonstrates that when a "terminal disconnect" occurs, SIGHUP
   is sent to all the members of the foreground process group for this terminal.

   Try using the following command to run this program in an X-window, and then
   closing the window:

        exec disc_SIGHUP > sig.log 2>&1

   (Since the above will replace the shell with this program, it will be the
   controlling process for the terminal.)
*/
#define _GNU_SOURCE     /* Get strsignal() declaration from <string.h> */
#include <string.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void             /* Handler for SIGHUP */
handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(),
            sig, strsignal(sig));
                        /* UNSAFE (see Section 21.1.2) */
}

/*
实验:
断开终端->父进程接受SIGHUP,然后进程终止->给前台进程组发SIGHUP

echo $$
8350

exec ./disc_SIGHUP d s s >/tmp/2.txt

断开终端

cat /tmp/2.txt
PID of parent process is:       8350(linux exec让当前进程替换bash PID不变)
Foreground process group ID is: 8350(当前前台进程)
PID=8443 PGID=8443 第一个子进程，自己创建了一个新租
PID=8350 PGID=8350 父进程
PID=8444 PGID=8350 第二个子进程和父进程同组
PID=8445 PGID=8350 第三个子进程和父进程同组
PID 8444: caught signal  1 (Hangup)
PID 8445: caught signal  1 (Hangup)

*/
int
main(int argc, char *argv[])
{
    pid_t parentPid, childPid;
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);

    setbuf(stdout, NULL);               /* Make stdout unbuffered */

    parentPid = getpid();
    printf("PID of parent process is:       %ld\n", (long) parentPid);
    printf("Foreground process group ID is: %ld\n",
            (long) tcgetpgrp(STDIN_FILENO));

    for (j = 1; j < argc; j++) {        /* Create child processes */
        childPid = fork();
        if (childPid == -1)
            errExit("fork");

        if (childPid == 0) {            /* If child... */
            if (argv[j][0] == 'd')      /* 'd' --> to different pgrp */
                if (setpgid(0, 0) == -1)
                    errExit("setpgid");

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1)
                errExit("sigaction");
            break;                      /* Child exits loop */
        }
    }

    /* All processes fall through to here */

    alarm(60);      /* Ensure each process eventually terminates */

    printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());
    for (;;)
        pause();        /* Wait for signals */
}
