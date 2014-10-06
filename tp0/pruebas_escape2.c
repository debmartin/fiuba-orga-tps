#include <stdio.h>

int main(){
        double real, imag;
        int vel;
        
        real = 0;
        imag = 0;
        
        for (vel = 0; vel < max_gray; ++vel) {
			if ((real * real + imag * imag) > 4)
            //Módulo al cuadrado > 4
				break;
            // Sino, elevo al cuadrado y le sumo a si mismo
            real = real * real - imag * imag + real;
            imag = 2 * real * imag + imag;
        }
        
        printf("%s: %s\n", "Vel de escape de 0+0i es 255", (vel == 255)? "OK" : "ERROR");

        real = 10;
        imag = 0;
        
        for (vel = 0; vel < max_gray; ++vel) {
			if ((real * real + imag * imag) > 4)
            //Módulo al cuadrado > 4
				break;
            // Sino, elevo al cuadrado y le sumo a si mismo
            real = real * real - imag * imag + real;
            imag = 2 * real * imag + imag;
        }

        printf("%s: %s\n", "Vel de escape de 10+0i es 0", (vel == 0)? "OK" : "ERROR");
       
        real = -1;
        imag = 0;
        
        for (vel = 0; vel < max_gray; ++vel) {
			if ((real * real + imag * imag) > 4)
            //Módulo al cuadrado > 4
				break;
            // Sino, elevo al cuadrado y le sumo a si mismo
            real = real * real - imag * imag + real;
            imag = 2 * real * imag + imag;
        }

        printf("%s: %s\n", "Vel de escape de -1+0i es 255", (vel == 255)? "OK" : "ERROR");
       
        real = 0.5;
        imag = 0;
        
        for (vel = 0; vel < max_gray; ++vel) {
			if ((real * real + imag * imag) > 4)
            //Módulo al cuadrado > 4
				break;
            // Sino, elevo al cuadrado y le sumo a si mismo
            real = real * real - imag * imag + real;
            imag = 2 * real * imag + imag;
        }


        printf("%s: %s\n", "Vel de escape de 0,5+0i es 4", (vel == 4)? "OK" : "ERROR");
       
        real = 1;
        imag = 0;
        
        for (vel = 0; vel < max_gray; ++vel) {
			if ((real * real + imag * imag) > 4)
            //Módulo al cuadrado > 4
				break;
            // Sino, elevo al cuadrado y le sumo a si mismo
            real = real * real - imag * imag + real;
            imag = 2 * real * imag + imag;
        }


        printf("%s: %s\n", "Vel de escape de 1+0i es 2", (vel == 2)? "OK" : "ERROR");
}
