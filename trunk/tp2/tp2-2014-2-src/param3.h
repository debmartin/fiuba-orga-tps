#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float UL_re;
	float UL_im;
	float LR_re;
	float LR_im;
	float d_re;
	float d_im;

	size_t x_res;
	size_t y_res;
	size_t shades;

	size_t x0;
	size_t x1;

	uint8_t *bitmap;
} param_t;
