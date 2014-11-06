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

    int resolutionX = DEFAULT_RESOLUTION_WIDTH;
    int resolutionY = DEFAULT_RESOLUTION_HEIGHT;
    float centerX = DEFAULT_CENTER_REAL;
    float centerY =  DEFAULT_CENTER_IMAG;
    float planeX = DEFAULT_PLANE_WIDTH;
    float planeY = DEFAULT_PLANE_HEIGHT;
    FILE* output = stdout;

    bool need_close = false;
    char option, i_str, mas, por;
    int option_index;

    while ((option = getopt_long(argc, argv, "o:r:c:w:H:", long_options, &option_index)) != -1) {
        switch (option){
            case 'r':
                if (sscanf(optarg, "%d%c%d", &resolutionX, &por, &resolutionY) != 2 || por!='x'){
                    printf("fatal: invalid resolution specification\n");
                    goto usage;
                }
                if (resolutionX <= 0 || resolutionY <= 0){
                    printf("fatal: invalid resolution specification\n");
                    goto usage;
                }

                break;
            case 'c':
                if (sscanf(optarg, "%f%c%f%c", &centerX, &mas, &centerY, &i_str) != 3 || i_str!='i' || mas!='+'){
                    printf("fatal: invalid center specification\n");
                    goto usage;
                }
                break;
            case 'w':
                planeX = atof(optarg);
                if (planeX <= 0){
                    printf("fatal: invalid width specification\n");
                    goto usage;
                }
                break;
            case 'H':
                planeY = atof(optarg);
                if (planeY <= 0){
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

    fprintf(output, "P2\n");
    fprintf(output, "%d\n%d\n", resolutionX, resolutionY);
    fprintf(output, "%d\n", 255);

    float first_real_value = centerX - planeX/2;
    float first_imaginary_value = centerY + planeY/2;
    float width_scale = (planeX / resolutionX);
    float height_scale =  - (planeY / resolutionY);
    first_real_value += width_scale/2;
    first_imaginary_value += height_scale/2;


    for(int i = resolutionY-1; i >= 0; i--){
        for(int j = 0; j < resolutionX; j++){
            float real1 = first_real_value + j * width_scale, imag1 = first_imaginary_value + i * height_scale;
            float real2 = real1, imag2 = imag1;

            int vel;
            for (vel = 0; vel < (N - 1); ++vel) {
                if (((unsigned int) (real2*real2 + imag2*imag2)) > 4)
                    break;
                float aux = real2 * real2 - imag2 * imag2 + real1;
                imag2 = 2 * real2 * imag2 + imag1;
                real2 = aux;
            }

            fprintf(output, "%d ", vel);
        }
        fprintf(output, "\n");
    }


    if (need_close) fclose(output);
    return 0;
}
