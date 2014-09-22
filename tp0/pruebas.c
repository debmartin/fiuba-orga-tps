#include <stdio.h>
#include "pgm.h"

int main(int argc, const char* argv[]) {
	puts("!!!Prueba!!!");

	printf("arg %d: %s\n", 1, argv[1]);

	double complex dc1 = 3 + 2 * I;
	double complex dc2 = -4 + 3 * I;
	double complex result;

	result = dc1 + dc2;

	printf("%g + %g i\n", result);

	complejo* num = crear_complejo(1, 5);
	complejo* num2 = crear_complejo(2, 4);
	num = sumar(num, num2);
	printf("real %g \t %g", num->real, num->imaginario);

	// Ejemplo de como crear pgm
	exampleForPGM("hola.pgm");
	return EXIT_SUCCESS;
}
