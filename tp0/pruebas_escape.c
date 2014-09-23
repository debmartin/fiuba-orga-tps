#include <stdio.h>
#include "velocidad_escape.h"

int main(){
	pixel_t* pixel1 = crear_pixel(0, 0);
	int vel1 = velocidad_de_escape(pixel1);
	printf("%s: %s\n", "Vel de escape de 0+0i es 255", (vel1 == 255)? "OK" : "ERROR");

	pixel_t* pixel2 = crear_pixel(10, 0);
	int vel2 = velocidad_de_escape(pixel2);
	printf("%s: %s\n", "Vel de escape de 10+0i es 0", (vel2 == 0)? "OK" : "ERROR");
	
}
