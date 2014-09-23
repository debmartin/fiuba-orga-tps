#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <complex.h>

#include "velocidad_escape.h"

#define N 256

typedef struct num_complejo {
	double real;
	double imaginario;
} complejo;

struct pixel {
	complejo* numero_complejo;
};

complejo* crear_complejo(double real, double imaginaria) {
	complejo* num = malloc(sizeof(complejo));
	if (num == NULL)
		return NULL;
	num->real = real;
	num->imaginario = imaginaria;
	return num;
}

void destruir_complejo(complejo* num) {
	free(num);
}

pixel_t* crear_pixel(double real, double imaginaria) {
	pixel_t* pixel = malloc(sizeof(pixel));
	if (pixel == NULL)
		return NULL;
	complejo* num = crear_complejo(real, imaginaria);
	if (num == NULL) {
		free(pixel);
		return NULL;
	}
	pixel->numero_complejo = num;
	return pixel;
}

void destruir_pixel(pixel_t* pixel) {
	destruir_complejo(pixel->numero_complejo);
	free(pixel);
}

unsigned int modulo_al_2(complejo* num) {
	return (num->real * num->real) + (num->imaginario * num->imaginario);
}

complejo* sumar(complejo* num1, complejo* num2) {
	double real = num1->real + num2->real;
	double imaginario = num1->imaginario + num2->imaginario;
	return crear_complejo(real, imaginario);
}

complejo* al_2(complejo* num) {
	double real = num->real - num->imaginario;
	double imaginario = 2 * num->real * num->imaginario;
	return crear_complejo(real, imaginario);
}

int velocidad_de_escape(pixel_t* pixel) {
	complejo* num = pixel->numero_complejo;
	complejo* num2 = crear_complejo(num->real, num->imaginario);
	int i;
	for (i = 0; i < (N - 1); ++i) {
		if (modulo_al_2(num2) > 4)
			break;
		num2 = sumar(al_2(num2), num);
	}
	return i;
}
