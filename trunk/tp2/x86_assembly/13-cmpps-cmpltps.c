/*
 * 13-cmpps-cmpltps.c
 * 
 * Usamos cmpltps para comparar dos números de punto flotante,
 * pasados por línea de comando. Luego, imprimimos el resultado
 * por salida estándar.
 *
 * $Date: 2008/06/10 18:58:28 $
 */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char * const argv[])
{
	float X[4] __attribute__ ((aligned (16)));
	float Y[4] __attribute__ ((aligned (16)));
	float Z[4] __attribute__ ((aligned (16)));
	char c;

	if (argc != 3)
		exit(1);

	if (sscanf(argv[1], "%f%c", X, &c) != 1
	    || sscanf(argv[2], "%f%c", Y, &c) != 1)
		exit(1);

	X[1] = X[2] = X[3] = 0.0f;
	Y[1] = Y[2] = Y[3] = 0.0f;
	Z[0] = -1.0f;

	__asm__ volatile (
		"movaps  %1, %%xmm0 \n\t"
		"cmpltps %2, %%xmm0 \n\t"
		"movaps  %%xmm0, %0 \n\t"
	: "=m" (Z) /* Ojo: usar Z, no *Z. ¿Por qué? */
	: "m" (*X), "m" (*Y) 
	: "xmm0"
	);

	printf("%f vs. %f => 0x%08lx\n",
	       (double) X[0],
	       (double) Y[0],
	       *(unsigned long *) ((void *) Z));

	return 0;
}
