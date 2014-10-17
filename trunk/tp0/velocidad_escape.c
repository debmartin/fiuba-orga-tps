#include <stdlib.h>
#include <stddef.h>

#include "velocidad_escape.h"

#define N 256

struct num_complejo {
	double real;
	double imaginario;
};

double getReal(complejo* num){
    return num->real;
}
double getImag(complejo* num){
    return num->imaginario;
}
double setImag(complejo* num, double imag){
    num->imaginario = imag;
}
double setReal(complejo* num, double real){
    num->real = real;
}
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

unsigned int modulo_al_2(complejo* num) {
	return (num->real * num->real) + (num->imaginario * num->imaginario);
}

complejo* sumar(complejo* num1, complejo* num2) {
	double real = num1->real + num2->real;
	double imaginario = num1->imaginario + num2->imaginario;
	return crear_complejo(real, imaginario);
}

complejo* al_2(complejo* num) {
	double real = num->real * num->real - num->imaginario * num->imaginario;
	double imaginario = 2 * num->real * num->imaginario;
	return crear_complejo(real, imaginario);
}

int velocidad_de_escape(complejo* num) {
	complejo* num2 = crear_complejo(num->real, num->imaginario);
	int i;
	for (i = 0; i < (N - 1); ++i) {
		if (modulo_al_2(num2) > 4)
			break;
		num2 = sumar(al_2(num2), num);
	}
	return i;
}
