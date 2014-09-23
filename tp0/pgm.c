// Based on http://zerocool.is-a-geek.net/simd-instructions-in-c-part-two/

#include "pgm.h"


#define HI(num)	(((num) & 0x0000FF00) >> 8)
#define LO(num)	((num) & 0x000000FF)



//int exampleForPGM(const char * modified_fileName) {
//	PGMData pgm_image;
//	PGMData * data = &pgm_image;
//
//	data->col = 800;
//	data->row = 600;
//	data->max_gray = 255;
//	data->matrix = allocate_dynamic_matrix(data->row, data->col);
//
//	int i, j;
//	// Genero como ejemplo un gradiente de grises (i es el valor de la fila)
//	//data->matrix[4][4] = 255;
//	for (i = 0; i < data->row; ++i) {
//		for (j = 0; j < data->col; ++j) {
//			data->matrix[i][j] = i;
//		}
//	}
//
//	printf("Write to PGM ...\n");
//	writePGM(modified_fileName, data);
//	return 0;
//
//}

int **allocate_dynamic_matrix(int row, int col) {

	int **ret_val;
	int i;

	ret_val = (int **) malloc(sizeof(int *) * row);
	if (ret_val == NULL) {
		perror("memory allocation failure");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < row; ++i) {
		ret_val[i] = (int *) malloc(sizeof(int) * col);
		if (ret_val[i] == NULL) {
			perror("memory allocation failure");
			exit(EXIT_FAILURE);
		}
	}

	return ret_val;
}

void deallocate_dynamic_matrix(int **matrix, int row) {

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

void writePGM(FILE *pgmFile, const PGMData *data) {
	int i, j;
	int lo;

	fprintf(pgmFile, "P2\n");
	fprintf(pgmFile, "%d\n%d\n", data->col, data->row);
	fprintf(pgmFile, "%d\n", data->max_gray);

	for (i = 0; i < data->row; ++i){
		for (j = 0; j < data->col; ++j) {
            fprintf(pgmFile, "%d ", data->matrix[i][j]);
		}
		fprintf(pgmFile, "\n");
	}
}
