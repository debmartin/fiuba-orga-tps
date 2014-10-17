#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_RESOLUTION_WIDTH 640
#define DEFAULT_RESOLUTION_HEIGHT 480
#define DEFAULT_CENTER_REAL 0
#define DEFAULT_CENTER_IMAG 0
#define DEFAULT_PLANE_WIDTH 4
#define DEFAULT_PLANE_HEIGHT 4
#define DEFAULT_MAX_GRAY 255
#define ARG_DEFAULT_OUT "-"

int main(int argc, char* argv[]){
    static struct option long_options[] =
    {
        {"resolution", required_argument, 0, 'r'},
        {"center", required_argument, 0, 'c'},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'H'},
        {"output", required_argument, 0, 'o'},
        {0, 0, 0, 0}
    };

    int resolution[2];
    float center[2];
    float plane[2];
	int max_gray;
    FILE* output;
	
	// La inicializo por default
	resolution[0] = DEFAULT_RESOLUTION_WIDTH;
    resolution[1] = DEFAULT_RESOLUTION_HEIGHT;
    center[0] = DEFAULT_CENTER_REAL;
    center[1] =  DEFAULT_CENTER_IMAG;
    plane[0] = DEFAULT_PLANE_WIDTH;
    plane[1] = DEFAULT_PLANE_HEIGHT;
	max_gray = DEFAULT_MAX_GRAY;
    output = stdout;
	

	// Parseo y verifico argumentos.
    bool need_close = false;
    char option, i;
    int option_index;

    while ((option = getopt_long(argc, argv, "o:r:c:w:H:", long_options, &option_index)) != -1) {
        switch (option){
            case 'r':
                if (sscanf(optarg, "%d%*c%d", &resolution[0], &resolution[1]) != 2){
                    printf("fatal: invalid resolution specification\n");
                    goto usage;
                }
                if (resolution[0] <= 0 || resolution[1] <= 0){
                    printf("fatal: invalid resolution specification\n");
                    goto usage;
                }

                break;
            case 'c':
                if (sscanf(optarg, "%f%*c%f%c", &center[0], &center[1], &i) != 3 || i!='i'){
                    printf("fatal: invalid center specification\n");
                    goto usage;
                }
                break;
            case 'w':
                plane[0] = atof(optarg);
                if (plane[0] <= 0){
                    printf("fatal: invalid width specification\n");
                    goto usage;
                }
                break;
            case 'H':
                plane[1] = atof(optarg);
                if (plane[1] <= 0){
                    printf("fatal: invalid height specification\n");
                    goto usage;
                }
                break;
            case 'o':
                if (strcmp(ARG_DEFAULT_OUT, optarg) != 0){
                    output = fopen(optarg, "w");
                    if (! output){
                        printf("fatal: cannot open output file\n");
                        goto usage;
                    }
                    need_close = true;
                }
                break;
            default:
            usage:
                printf("Usage:\n\ttp0 -H, for height of the complex plane (4 by default)\n\ttp0 -w, for width of the complex plane (4 by default)\n\ttp0 -r, for resolution of the image: AxB format with A and B natural numbers (640x480 by default)\n\ttp0 -c, for centre of the complex plane: A+Bi format with A and B numbers (0+0i by default)\n\ttp0 -o, for pgm output file (stdout by default)\n");
                return 1;
        }
    }
    
	// Calculo y escribo el archivo:
	
	// Armo escala.
	double first_real_value = center[0] - plane[0]/2;
    double first_imaginary_value = center[1] + plane[1]/2;
    double width_scale = (plane[0] / resolution[0]);
    double height_scale =  - (plane[1] / resolution[1]);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;
	
	// Escribo parámetros de la imagen.
	fprintf(output, "P2\n");
	fprintf(output, "%d\n%d\n", resolution[0], resolution[1]);
	fprintf(output, "%d\n", max_gray);

	// Escribo valor de cada pixel.
	int j, k;
	for (j = 0; j < resolution[1]; ++j){
		for (k = 0; k < resolution[0]; ++k) {
            
			double real = first_real_value + k * width_scale;
			double imag = first_imaginary_value + j * height_scale;
			
			// Calculo velocidad de escape
			int vel;
			for (vel = 0; vel < max_gray; ++vel) {
				if ((real * real + imag * imag) > 4)
					//Módulo al cuadrado > 4
					break;
				// Sino, elevo al cuadrado y le sumo a si mismo
				real = real * real - imag * imag + real;
				imag = 2 * real * imag + imag;
			}
			
			fprintf(output, "%d ", vel);
		}
		fprintf(output, "\n");
	}
	// Terminé de escribir el archivo.
	
	
    if (need_close) fclose(output);
    return 0;
}
