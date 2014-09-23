#include <stdio.h>
#include "pgm.h"
#include "velocidad_escape.h"
void pruebas_verificacion_de_argumentos(){
	
	//Probar: -H --height, y no -h o -height y así con todos.
	//Probar: H, w, r con números negativos o ceros.
	//Probar centro con formato no a+bi. Sólo a, sólo bi, cólo a+i, sólo i.
	//Probar comandos vacíos.
	//Probar archivo de salida no.pgm.
	//Probar valores por defecto.
	
}

void pruebas_conjunto_de_Mandelbrot(){
	
	// Probar los dos del ejemplo, y alguno de mis pruebas.
	
}

int main(int argc, const char* argv[]) {
	
	pruebas_verificacion_de_argumentos();
	pruebas_conjunto_de_Mandelbrot();

        // Ejemplo de como crear pgm
        exampleForPGM("hola.pgm");
        return EXIT_SUCCESS;
}
