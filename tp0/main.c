#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "velocidad_escape.h"

#define DEFAULT_RESOLUTION_WIDTH 640
#define DEFAULT_RESOLUTION_HEIGHT 480
#define DEFAULT_CENTER_REAL 0
#define DEFAULT_CENTER_IMAG 0
#define DEFAULT_PLANE_WIDTH 4
#define DEFAULT_PLANE_HEIGHT 4
#define ARG_DEFAULT_OUT "-"
#define N 256


typedef struct _OutputData{
    int resolution[2];
    float center[2];
    float plane[2];
    FILE* output;
} OutputData;

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

    OutputData data;
    data.resolution[0] = DEFAULT_RESOLUTION_WIDTH;
    data.resolution[1] = DEFAULT_RESOLUTION_HEIGHT;
    data.center[0] = DEFAULT_CENTER_REAL;
    data.center[1] =  DEFAULT_CENTER_IMAG;
    data.plane[0] = DEFAULT_PLANE_WIDTH;
    data.plane[1] = DEFAULT_PLANE_HEIGHT;
    data.output = stdout;

    bool need_close = false;
    char option, i;
    int option_index;

    while ((option = getopt_long(argc, argv, "o:r:c:w:H:", long_options, &option_index)) != -1) {
        switch (option){
            case 'r':
                if (sscanf(optarg, "%d%*c%d", &data.resolution[0], &data.resolution[1]) != 2){
                    printf("fatal: invalid resolution specification\n");
                    return 1;
                }
                if (data.resolution[0] <= 0 || data.resolution[1] <= 0){
                    printf("Usage:\n\ttp0 -h\n\ttp0 -V\n");
                    return 1;
                }
                break;
            case 'c':
                if (sscanf(optarg, "%f%*c%f%c", &data.center[0], &data.center[1], &i) != 3 || i!='i'){
                    printf("fatal: invalid center specification\n");
                    return 1;
                }
                break;
            case 'w':
                data.plane[0] = atof(optarg);
                if (data.plane[0] <= 0){
                    printf("fatal: invalid width specification\n");
                    return 1;
                }
                break;
            case 'H':
                data.plane[1] = atof(optarg);
                if (data.plane[1] <= 0){
                    printf("fatal: invalid height specification\n");
                    return 1;
                }
                break;
            case 'o':
                if (strcmp(ARG_DEFAULT_OUT, optarg) != 0){
                    data.output = fopen(optarg, "w");
                    if (! data.output){
                        printf("fatal: cannot open output file\n");
                        return 1;
                    }
                    need_close = true;
                }
                break;
            default:
                printf("fatal: invalid arguments\n");
                return 1;
        }
    }

    fprintf(data.output, "P2\n");
	fprintf(data.output, "%d\n%d\n", data.resolution[0], data.resolution[1]);
	fprintf(data.output, "%d\n", 255);

    double first_real_value = data.center[0] - data.plane[0]/2;
    double first_imaginary_value = data.center[1] + data.plane[1]/2;
    double width_scale = (data.plane[0] / data.resolution[0]);
    double height_scale =  - (data.plane[1] / data.resolution[1]);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;

    int matrix[data.resolution[1]][data.resolution[0]];

    for(int i = data.resolution[1]-1; i >= 0; i--){
        for(int j = 0; j < data.resolution[0]; j++){
            double real1 = first_real_value + j * width_scale, imag1 = first_imaginary_value + i * height_scale;
            double real2 = real1, imag2 = imag1;
            complejo* num1 = crear_complejo(first_real_value + j * width_scale, first_imaginary_value + i * height_scale);
            complejo* num2 = crear_complejo(first_real_value + j * width_scale, first_imaginary_value + i * height_scale);

            int vel;
            for (vel = 0; vel < (N - 1); ++vel) {
                if (((unsigned int) (real2*real2 + imag2*imag2)) > 4)
                    break;
                num2 = sumar(al_2(num2), num1);
                real2 = getReal(num2);
                imag2 = getImag(num2);
//                real2 = real2 * real2 - imag2 * imag2 + real1;
//                imag2 = 2 * real2 * imag2 + imag1;
            }

            fprintf(data.output, "%d ", vel);
        }
        fprintf(data.output, "\n");
    }


    if (need_close) fclose(data.output);
    return 0;
}
