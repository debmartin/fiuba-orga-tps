#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "pgm.h"
#include "velocidad_escape.h"

#define DEFAULT_RESOLUTION_WIDTH 640
#define DEFAULT_RESOLUTION_HEIGHT 480
#define DEFAULT_CENTER_REAL 0
#define DEFAULT_CENTER_IMAG 0
#define DEFAULT_PLANE_WIDTH 4
#define DEFAULT_PLANE_HEIGHT 4
#define ARG_DEFAULT_OUT "-"


typedef struct _OutputData{
    int resolution[2];
    float center[2];
    float plane[2];
    FILE* output;
} OutputData;

void generatePGM(OutputData* data){
    PGMData pgm_image;
    pgm_image.row = data->resolution[1];
    pgm_image.col = data->resolution[0];
    pgm_image.max_gray = 255;
    pgm_image.matrix = allocate_dynamic_matrix(pgm_image.row, pgm_image.col);

    double first_real_value = data->center[0] - data->plane[0]/2;
    double first_imaginary_value = data->center[1] + data->plane[1]/2;
    double width_scale = (data->plane[0] / data->resolution[0]);
    double height_scale =  - (data->plane[1] / data->resolution[1]);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;

    for(int i = 0; i < pgm_image.row; i++){
        for(int j = 0; j < pgm_image.col; j++){
            pixel_t* pixel = crear_pixel(first_real_value + j * width_scale, first_imaginary_value + i * height_scale);
            pgm_image.matrix[i][j] = velocidad_de_escape(pixel);
            destruir_pixel(pixel);
        }
    }

    writePGM(data->output, &pgm_image);
    deallocate_dynamic_matrix(pgm_image.matrix, pgm_image.row);
}

void OutputDataInitialize(OutputData* data){
    data->resolution[0] = DEFAULT_RESOLUTION_WIDTH;
    data->resolution[1] = DEFAULT_RESOLUTION_HEIGHT;
    data->center[0] = DEFAULT_CENTER_REAL;
    data->center[1] =  DEFAULT_CENTER_IMAG;
    data->plane[0] = DEFAULT_PLANE_WIDTH;
    data->plane[1] = DEFAULT_PLANE_HEIGHT;
    data->output = stdout;
}

int terminateError(char* errorMessaje){
    perror(errorMessaje);
    return EXIT_FAILURE;
}

int main(int argc, char* argv[]){
    static struct option long_options[] =
    {
        {"resolution", required_argument, 0, 'r'},
        {"center", required_argument, 0, 'c'},
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'H'},
        {"output", required_argument, 0, 'o'}
    };

    OutputData data;
    OutputDataInitialize(&data);
    bool need_close = false;
    char option;
    int option_index;

    while ((option = getopt_long(argc, argv, "o:r:c:w:H:", long_options, &option_index)) != -1) {
        switch (option){
            case 'r':
                if (sscanf(optarg, "%d%*c%d", &data.resolution[0], &data.resolution[1]) != 2){
                    return terminateError("fatal: invalid resolution specification");
                }
                if (data.resolution[0] == 0 || data.resolution[1] == 0){
                    return terminateError("Usage:\n\ttp0 -h\n\ttp0 -V\n");
                }
                break;
            case 'c':
                if (sscanf(optarg, "%f%*c%f%*c", &data.center[0], &data.center[1]) != 2){
                    return terminateError("fatal: invalid center specification");
                }
                break;
            case 'w':
                data.plane[0] = atof(optarg);
                if (data.plane[0] == 0){
                    return terminateError("fatal: invalid width specification");
                }
                break;
            case 'H':
                data.plane[1] = atof(optarg);
                if (data.plane[1] == 0){
                    return terminateError("fatal: invalid height specification");
                }
                break;
            case 'o':
                if (strcmp(ARG_DEFAULT_OUT, optarg) != 0){
                    data.output = fopen(optarg, "w");
                    if (! data.output){
                        perror("fatal: cannot open output file");
                        exit(EXIT_FAILURE);
                    }
                    need_close = true;
                }
                break;
            default:
                perror("fatal: invalid arguments");
                exit(EXIT_FAILURE);
        }
    }
    generatePGM(&data);
    if (need_close) fclose(data.output);
    return 0;
}
