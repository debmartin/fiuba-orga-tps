#include <ctype.h>
#include "pgm.h"

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

	fprintf(pgmFile, "P2\n");
	fprintf(pgmFile, "%d\n%d\n", data->col, data->row);
	fprintf(pgmFile, "%d\n", data->max_gray);

	for (i = 0; i < data->row; ++i){
		for (j = 0; j < data->col; ++j) {
            fprintf(pgmFile, "%d ", data->matrix[data->row - i-1][j]);
		}
		fprintf(pgmFile, "\n");
	}
}
