/*
 * 09-loop4.c
 *
 * loop4.c: similar a loop2.c y loop3.c, pero con el bug corregido,
 * y usando modo de direccionamiento base + registro * ancho + offset.
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef N
#define N 13
#endif

int
main(int argc, char * const argv[])
{
	int i;
	int v[N];

	__asm__ volatile (
		"init:                      \n\t"
		"xorl %%ebx, %%ebx          \n\t" /* ebx: loop counter. */
		
		"loop:                      \n\t"
		"cmpl %1, %%ebx             \n\t"
		"jge  fin                   \n\t"

		"body:                      \n\t"
		"movl %%ebx, %%eax          \n\t"
		"mull %%eax                 \n\t"
		"movl %%eax, (%0, %%ebx, 4) \n\t" /* address: %0 + 4 * ebx. */

		"incr:                      \n\t"
		"incl %%ebx                 \n\t"
		"jmp  loop                  \n\t"
		
		"fin:                       \n\t"
	:
	: "g" (v), "i" (N)
	: "eax", "ebx", "ecx", "edx", "cc", "memory"
	);

	for (i = 0; i < N; ++i)
		printf("%d^2 = %d\n", i, v[i]);

	return 0;
}
