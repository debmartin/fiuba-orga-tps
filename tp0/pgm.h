#ifndef PGM_H_INCLUDED
#define PGM_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <xmmintrin.h>

typedef struct _PGMData {
	int row;
	int col;
	int max_gray;
	int **matrix;
} PGMData;

int ** allocate_dynamic_matrix(int row, int col);

void deallocate_dynamic_matrix(int **matrix, int row);

void SkipComments(FILE *fp);

void writePGM(FILE *filename, const PGMData *data);


#endif // PGM_H_INCLUDED
