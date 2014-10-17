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

    fprintf(data.output, "P2\n");
	fprintf(data.output, "%d\n%d\n", data.resolution[0], data.resolution[1]);
	fprintf(data.output, "%d\n", 255);

    double first_real_value = data.center[0] - data.plane[0]/2;
    double first_imaginary_value = data.center[1] + data.plane[1]/2;
    double width_scale = (data.plane[0] / data.resolution[0]);
    double height_scale =  - (data.plane[1] / data.resolution[1]);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;


    for(int i = data.resolution[1]-1; i >= 0; i--){
        for(int j = 0; j < data.resolution[0]; j++){
            double real1 = first_real_value + j * width_scale, imag1 = first_imaginary_value + i * height_scale;
            double real2 = real1, imag2 = imag1;

            int vel;
            for (vel = 0; vel < (N - 1); ++vel) {
                if (((unsigned int) (real2*real2 + imag2*imag2)) > 4)
                    break;
                double aux = real2 * real2 - imag2 * imag2 + real1;
                imag2 = 2 * real2 * imag2 + imag1;
                real2 = aux;
            }

            fprintf(data.output, "%d ", vel);
        }
        fprintf(data.output, "\n");
    }


    if (need_close) fclose(data.output);
    return 0;
}
