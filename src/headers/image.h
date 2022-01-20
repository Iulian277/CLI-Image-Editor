#include "commons.h"

#ifndef _IMAGE_
#define _IMAGE_

typedef struct Image
{
    unsigned int width;
    unsigned int height;
    unsigned int channels;

    double **red;
    double **green;
    double **blue;
    double **alpha;
} Image;

void print_matrix(char *var, double **mat, int row, int col);

unsigned char **alloc_uchar_matrix(int height, int width);
double **alloc_double_matrix(int height, int width);

// void alloc_matrix(unsigned char ***matrix, int height, int width);
void dealloc_matrix(void **matrix, int rows);

Image *alloc_image(int height, int width, int channels);
void dealloc_image(Image *image);

void data_to_matrix(Image *image, unsigned char *data);
void matrix_to_data(Image *image, unsigned char *data);

void keep_first_k_singular(double **original, double **compr, int rows, int cols);

#endif // _IMAGE_
