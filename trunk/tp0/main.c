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

    double first_real_value = data->center[0] - ((float)data->plane[0])/2;
    double first_imaginary_value = data->center[1] + ((float)data->plane[1])/2;
    double width_scale = (((float) data->plane[0]) / data->resolution[0]);
    double height_scale =  - (((float) data->plane[1]) / data->resolution[1]);
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
                sscanf(optarg, "%d%*c%d", &data.resolution[0], &data.resolution[1]);
                break;
            case 'c':
                sscanf(optarg, "%f%*c%f%*c", &data.center[0], &data.center[1]);
                break;
            case 'w':
                data.plane[0] = atoi(optarg);
                break;
            case 'H':
                data.plane[1] = atoi(optarg);
                break;
            case 'o':
                if (strcmp(ARG_DEFAULT_OUT, optarg) != 0)
                    data.output = fopen(optarg, "w");
                    if (! data.output){
                        perror("cannot open file to write");
                        exit(EXIT_FAILURE);
                    }
                    need_close = true;
        }
    }
    generatePGM(&data);
    if (need_close) fclose(data.output);
    return 0;
}
