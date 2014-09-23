#include <stdio.h>
#include "velocidad_escape.h"

int main(){
	complejo* complejo1 = crear_complejo(0, 0);
	int vel1 = velocidad_de_escape(complejo1);
	printf("%s: %s\n", "Vel de escape de 0+0i es 255", (vel1 == 255)? "OK" : "ERROR");

	complejo* complejo2 = crear_complejo(10, 0);
	int vel2 = velocidad_de_escape(complejo2);
	printf("%s: %s\n", "Vel de escape de 10+0i es 0", (vel2 == 0)? "OK" : "ERROR");
	
   complejo* complejo3 = crear_complejo(-1, 0);
	int vel3 = velocidad_de_escape(complejo3);
	printf("%s: %s\n", "Vel de escape de -1+0i es 255", (vel3 == 255)? "OK" : "ERROR");
	
	complejo* complejo4 = crear_complejo(0.5, 0);
	int vel4 = velocidad_de_escape(complejo4);
	printf("%s: %s\n", "Vel de escape de 0,5+0i es 4", (vel4 == 4)? "OK" : "ERROR");
	
	complejo* complejo5 = crear_complejo(1, 0);
	int vel5 = velocidad_de_escape(complejo5);
	printf("%s: %s\n", "Vel de escape de 1+0i es 2", (vel5 == 2)? "OK" : "ERROR");
}
