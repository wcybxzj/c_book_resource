#define _GNU_SOURCE                 /* Get '_sys_nerr' and '_sys_errlist'
                                       declarations from <stdio.h> */
#include <stdio.h>
#include <string.h>                 /* Get declaration of strerror() */
#include <pthread.h>
#include "tlpi_hdr.h"


#define MAX_ERROR_LEN 256           /* Maximum length of string in per-thread
                                       buffer returned by strerror() */

char *
strerror(int err)
{
    char *buf;

	buf = malloc(MAX_ERROR_LEN);
	if (buf == NULL)
		errExit("malloc");

	if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
		snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
	} else {
		strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
		buf[MAX_ERROR_LEN - 1] = '\0';          /* Ensure null termination */
	}

	return buf;
}
