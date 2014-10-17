#ifndef VELOCIDAD_ESCAPE_H_INCLUDED
#define VELOCIDAD_ESCAPE_H_INCLUDED

typedef struct num_complejo complejo;

int velocidad_de_escape(complejo* num);

complejo* crear_complejo(double real, double imaginaria);

void destruir_complejo(complejo* num);

double setReal(complejo* num, double real);
double getReal(complejo* num);
double setImag(complejo* num, double imag);
double getImag(complejo* num);

#endif // VELOCIDAD_ESCAPE_H_INCLUDED
