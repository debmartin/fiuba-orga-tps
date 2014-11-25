/*
 * 04-gt.c
 *
 * Muestra como hacer branchs condicionales: recibimos dos números
 * por línea de comandos, e imprime por salida estándar el resultado
 * de la comparación (i.e. mayor-menor-igual).
 *
 * $Date: 2008/06/10 18:50:01 $
 */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char * const argv[])
{
	int i;
	int j;
	int k;
	char c;

	if (argc != 3)
		exit(1);

	if (sscanf(argv[1], "%d %c", &i, &c) != 1
	    || sscanf(argv[2], "%d %c", &j, &c) != 1)
		exit(1);

	__asm__ volatile (
		"cmp  %2, %1 \n\t"
		"jl   lt     \n\t"
		"je   eq     \n\t"
		"jg   gt     \n\t"

		"lt:          \n\t"
		"movl $-1, %0 \n\t"
		"jmp  fin     \n\t"

		"eq:         \n\t"
		"movl $0, %0 \n\t"
		"jmp  fin    \n\t"

		"gt:         \n\t"
		"movl $1, %0 \n\t"
		"jmp  fin    \n\t"

		"fin:        \n\t"
	: "=g" (k)
	: "r" (i), "r" (j) 
	: "cc"
	);

#define SIGN(i)                     \
	((i) == 0 ? '='             \
	          : ((i) == 1 ? '>' \
	                     : '<'))
	printf("%d %c %d\n", i, SIGN(k), j);

	return 0;
}
