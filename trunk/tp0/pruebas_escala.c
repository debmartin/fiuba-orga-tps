#include <stdio.h>
#include "velocidad_escape.h"

void imprimir_complejos_del_plano(double centro_real, double centro_imag, double ancho_plano, double alto_plano, int ancho_res, int alto_res, double* v){
	
	double first_real_value = centro_real - ((float)ancho_plano)/2;
    double first_imaginary_value = centro_imag + ((float)alto_plano)/2;
    double width_scale = (((float) ancho_plano) / ancho_res);
    double height_scale =  - (((float) alto_plano) / alto_res);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;
    
	int contador = 0;
    for(int i = 0; i < alto_res; i++){
        for(int j = 0; j < ancho_res; j++){
			double real = first_real_value + j * width_scale;
			double imag = first_imaginary_value + i * height_scale;
			
			printf("%f + %f i\n", real, imag);
			
			v[contador] = real;
			contador++;
			v[contador] = imag;
			contador++;
		}
	}

}

int main(){

	printf("Complejos para -c 0+0i -r 2x2 -w 4 -H 4:\n");
    double v1[8];
    imprimir_complejos_del_plano(0, 0, 4, 4, 2, 2, v1);
	printf("El primero es correcto: %s\n", ((v1[0] == -1) & (v1[1] == 1))? "OK" : "ERROR");
	printf("El segundo es correcto: %s\n", ((v1[2] == 1) & (v1[3] == 1))? "OK" : "ERROR");
	printf("El tercero es correcto: %s\n", ((v1[4] == -1) & (v1[5] == -1))? "OK" : "ERROR");
	printf("El cuarto es correcto: %s\n", ((v1[6] == 1) & (v1[7] == -1))? "OK" : "ERROR");
	
	printf("Complejos para -c 10+0i -r 1x1 -w 4 -H 4:\n");
    double v2[2];
    imprimir_complejos_del_plano(10, 0, 4, 4, 1, 1, v2);
	printf("El primero es correcto: %s\n", ((v2[0] == 10) & (v2[1] == 0))? "OK" : "ERROR");
	
	printf("Complejos para -c -3.5+1i -r 1x1 -w 4 -H 4:\n");
    double v3[2];
    imprimir_complejos_del_plano(-3.5, 1, 4, 4, 1, 1, v3);
	printf("El primero es correcto: %s\n", ((v3[0] == - 3.5) & (v3[1] == 1))? "OK" : "ERROR");
	
	printf("Complejos para -c 1-3i -r 2x1 -w 2 -H 3:\n");
    double v4[4];
    imprimir_complejos_del_plano(1, -3, 2, 3, 2, 1, v4);
	printf("El primero es correcto: %s\n", ((v4[0] == 0.5) & (v4[1] == -3))? "OK" : "ERROR");
	printf("El segundo es correcto: %s\n", ((v4[2] == 1.5) & (v4[3] == -3))? "OK" : "ERROR");
	
	return 0;
}
