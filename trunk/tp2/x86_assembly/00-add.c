/*
 * 00-add.txt
 *
 * Recibe dos números por línea de comando, hace la suma, e imprime
 * el resultado de la operación por salida estándar. Compilar con -g.
 *
 * $Date: 2008/06/10 18:50:00 $
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

	if (sscanf(argv[1], "%d%c", &i, &c) != 1
	    || sscanf(argv[2], "%d%c", &j, &c) != 1)
		exit(1);

	/*
	 * Pasaje de los argumentos de entrada por registros.
	 */
	__asm__ volatile (
		"movl %1, %0\n\t"
		"addl %2, %0\n\t"
	: "=g" (k)
	: "r" (i), "r" (j) 
	);

	printf("(1) %d + %d = %d\n", i, j, k);

	/*
	 * Observar que, en este caso, el pasaje de argumentos
	 * se hace usando operandos en memoria. Ver el código
	 * assembler resultante.
	 */
	__asm__ volatile (
		"movl %1, %0\n\t"
		"addl %2, %0\n\t"
	: "=r" (k)
	: "m" (i), "m" (j) 
	);

	printf("(2) %d + %d = %d\n", i, j, k);

	return 0;
}
