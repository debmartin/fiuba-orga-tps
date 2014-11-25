/*
 * 05-loop1a.c
 *
 * Calcula e imprime los diez primeros cuadrados, empezando desde 0.
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>

int
main(int argc, char * const argv[])
{
	int i;
	int j;

	for (i = 0; i < 10; ++i) {
		__asm__ volatile (
			"movl %1, %%eax\n\t"
			"mull %1       \n\t"
			"movl %%eax, %0\n\t"
		: "=g" (j)
		: "g" (i)
		: "cc", "eax", "edx" /* Ojo: mull usa %edx:%eax. */
		);
		printf("%d^2 = %d\n", i, j);
	}

	return 0;
}
