/*
 * 12-addps.c
 * 
 * Incrementa e imprime los 4 argumentos de punto flotante, usando
 * instrucciones SIMD SSE para hacer las 4 operaciones con una sola
 * instrucción.
 *
 * $Date: 2008/06/10 19:02:47 $
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

	if (argc != 5)
		exit(1);

	X[0] = X[1] = X[2] = X[3] = 0.0f;
	Y[0] = Y[1] = Y[2] = Y[3] = 1.0f;

	if (sscanf(argv[1], "%f %c", X + 0, &c) != 1
	    || sscanf(argv[2], "%f %c", X + 1, &c) != 1
	    || sscanf(argv[3], "%f %c", X + 2, &c) != 1
	    || sscanf(argv[4], "%f %c", X + 3, &c) != 1)
		exit(1);

	__asm__ volatile (
		"movaps  %1, %%xmm0 \n\t"
		"addps   %2, %%xmm0 \n\t"
		"movaps  %%xmm0, %0 \n\t"
	: "=m" (Z) /* Ojo: usar Z, no *Z. ¿Por qué? */
	: "m" (*X), "m" (*Y) 
	: "xmm0"
	);

	printf("%f %f %f %f\n", Z[0], Z[1], Z[2], Z[3]);

	return 0;
}
