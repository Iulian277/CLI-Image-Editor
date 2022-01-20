#include "image.h"
#include "commons.h"

#ifndef _ROTATE_IMG_
#define _ROTATE_IMG_

void transpose_matrix(double **original, double **transp, int rows, int cols);
Image *rotate_image(Image *image, int angle); // angles in degree
void svd_lapack(double **a, int row, int col, double **u, double **s, double **v);
void matrix_mult(double **A, double **B, double **C, int rowsA, int colsA, int colsB);

#endif // _ROTATE_IMG_