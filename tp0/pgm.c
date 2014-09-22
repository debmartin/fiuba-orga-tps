// Based on http://zerocool.is-a-geek.net/simd-instructions-in-c-part-two/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xmmintrin.h>

#define HI(num)	(((num) & 0x0000FF00) >> 8)
#define LO(num)	((num) & 0x000000FF)

typedef struct _PGMData {
	int row;
	int col;
	int max_gray;
	float **matrix;
} PGMData;

float ** allocate_dynamic_matrix(int row, int col);
void deallocate_dynamic_matrix(float **matrix, int row);
void SkipComments(FILE *fp);
void writePGM(const char *filename, const PGMData *data);

int exampleForPGM(const char * modified_fileName) {
	PGMData pgm_image;
	PGMData * data = &pgm_image;

	data->col = 800;
	data->row = 600;
	data->max_gray = 255;
	data->matrix = allocate_dynamic_matrix(data->row, data->col);

	int i, j;
	// Genero como ejemplo un gradiente de grises (i es el valor de la fila)
	//data->matrix[4][4] = 255;
	for (i = 0; i < data->row; ++i) {
		for (j = 0; j < data->col; ++j) {
			data->matrix[i][j] = i;
		}
	}

	printf("Write to PGM ...\n");
	writePGM(modified_fileName, data);
	return 0;

}

float **allocate_dynamic_matrix(int row, int col) {

	float **ret_val;
	int i;

	ret_val = (float **) malloc(sizeof(float *) * row);
	if (ret_val == NULL) {
		perror("memory allocation failure");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < row; ++i) {
		ret_val[i] = (float *) malloc(sizeof(float) * col);
		if (ret_val[i] == NULL) {
			perror("memory allocation failure");
			exit(EXIT_FAILURE);
		}
	}

	return ret_val;
}

void deallocate_dynamic_matrix(float **matrix, int row) {

	int i;

	for (i = 0; i < row; ++i)
		free(matrix[i]);
	free(matrix);
}

void SkipComments(FILE *fp) {

	int ch;
	char line[100];

	while ((ch = fgetc(fp)) != EOF && isspace(ch))
		;
	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		SkipComments(fp);
	} else
		fseek(fp, -1, SEEK_CUR);

}

void writePGM(const char *filename, const PGMData *data) {

	FILE *pgmFile;
	int i, j;
	int lo;

	pgmFile = fopen(filename, "wb");
	if (pgmFile == NULL) {
		perror("cannot open file to write");
		exit(EXIT_FAILURE);
	}

	fprintf(pgmFile, "P5 ");
	printf("col %d - row %d ", data->col, data->row);
	printf(" maxgray %d ", data->max_gray);
	fprintf(pgmFile, "%d %d ", data->col, data->row);
	fprintf(pgmFile, "%d ", data->max_gray);

	for (i = 0; i < data->row; ++i)
		for (j = 0; j < data->col; ++j) {
			lo = LO((int ) data->matrix[i][j]);
			fputc(lo, pgmFile);
		}

	fclose(pgmFile);
	deallocate_dynamic_matrix(data->matrix, data->row);
}
