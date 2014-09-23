#include <stdio.h>
#include "velocidad_escape.h"

int main(){
	pixel_t* pixel1 = crear_pixel(0, 0);
	int vel1 = velocidad_de_escape(pixel1);
	printf("%s: %s\n", "Vel de escape de 0+0i es 255", (vel1 == 255)? "OK" : "ERROR");

	pixel_t* pixel2 = crear_pixel(10, 0);
	int vel2 = velocidad_de_escape(pixel2);
	printf("%s: %s\n", "Vel de escape de 10+0i es 0", (vel2 == 0)? "OK" : "ERROR");
	
   pixel_t* pixel3 = crear_pixel(-1, 0);
	int vel3 = velocidad_de_escape(pixel3);
	printf("%s: %s\n", "Vel de escape de -1+0i es 255", (vel3 == 255)? "OK" : "ERROR");
	
	pixel_t* pixel4 = crear_pixel(0.5, 0);
	int vel4 = velocidad_de_escape(pixel4);
	printf("%s: %s\n", "Vel de escape de 0,5+0i es 4", (vel4 == 4)? "OK" : "ERROR");
	
	pixel_t* pixel5 = crear_pixel(1, 0);
	int vel5 = velocidad_de_escape(pixel5);
	printf("%s: %s\n", "Vel de escape de 1+0i es 2", (vel5 == 2)? "OK" : "ERROR");
}
