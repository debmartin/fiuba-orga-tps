/*
 * $Date: 2007/05/24 15:16:41 $
 */

#ifndef _XMALLOC_H_INCLUDED_
#define _XMALLOC_H_INCLUDED_

extern void *xmalloc(size_t);
extern void *xrealloc(void *, size_t);
extern void xfree(void *);

#endif
