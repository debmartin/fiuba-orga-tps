/*
 * 08-loop3.c
 *
 * loop3.c: similar a loop2.c, pero usando un argumento en inmediato.
 *
 * Ojo: tiene un bug. Usar gdb con ``display /i $pc'', ``stepi'' y
 * ``disassemble main''.
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
	char c;

	__asm__ volatile (
		"init:               \n\t"
		"xorl %%ebx, %%ebx   \n\t" /* ebx: loop counter. */
		
		"loop:               \n\t"
		"cmpl %1, %%ebx      \n\t"
		"jge  fin            \n\t"

		"body:               \n\t"
		"movl $4, %%eax      \n\t"
		"mull %%ebx          \n\t"
		"addl %0, %%eax      \n\t"
		"movl %%eax, %%ecx   \n\t"

		"movl %%ebx, %%eax   \n\t"
		"mull %%eax          \n\t"
		"movl %%eax, (%%ecx) \n\t"
		
		"incr:               \n\t"
		"incl %%ebx          \n\t"
		"jmp  loop           \n\t"
		
		"fin:                \n\t"
	:
	: "g" (v), "i" (N)
	: "eax", "ebx", "ecx", "cc", "memory"
	);

	for (i = 0; i < N; ++i)
		printf("%d^2 = %d\n", i, v[i]);

	return 0;
}
