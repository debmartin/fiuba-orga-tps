/*
 * $Date: 2007/05/24 15:16:41 $
 */

#include <stdlib.h>
#include <xmalloc.h>
#include <die.h>

void *
xmalloc(size_t size)
{
	void *p;
	if ((p = malloc(size)) == 0)
		die("out of memory.");
	return p;
}

void *
xrealloc(void *ptr, size_t size)
{
	void *p;
	if ((p = realloc(ptr, size)) == 0)
		die("out of memory.");
	return p;
}

void
xfree(void *ptr)
{
	free(ptr);
}
