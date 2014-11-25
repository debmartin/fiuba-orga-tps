/*
 * 07-loop2.c
 *
 * Carga los cuadrados de los n enteros (empezando desde cero) 
 * en un arreglo pasado por parámetro, y muestra el resultado
 * por salida estándar.
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <xmalloc.h>

int
main(int argc, char * const argv[])
{
	int i;
	int n;
	int *v;
	char c;

	if (argc != 2)
		exit(1);

	if (sscanf(argv[1], "%d%c", &n, &c) != 1
	    || n <= 0)
		exit(1);

	/* Allocate memory. */
	v = xmalloc(n * sizeof(int));

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
		"movl %%eax, %%ecx   \n\t" /* ecx: destination pointer. */

		"movl %%ebx, %%eax   \n\t"
		"mull %%eax          \n\t" /* Ojo: modifica edx:eax. */
		"movl %%eax, (%%ecx) \n\t"
		
		"incr:               \n\t"
		"incl %%ebx          \n\t"
		"jmp  loop           \n\t"
		
		"fin:                \n\t"
	:
	: "g" (v), "g" (n)
	: "eax", "ebx", "ecx", "edx", "cc", "memory"
	);

	for (i = 0; i < n; ++i)
		printf("%d^2 = %d\n", i, v[i]);

	/* Clean up. */
	xfree(v);

	return 0;
}
