/*
 * $Date: 2007/05/24 15:16:41 $
 */

#include <stdlib.h>
#include <stdio.h>
#include <die.h>

void
die(const char *msg)
{
	fprintf(stderr, "fatal: %s\n", msg);
	exit(1);
}
