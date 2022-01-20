#include "../headers/image.h"

void print_matrix(char *var, double **mat, int row, int col)
{
	printf("\nMatrix %s, rows=%d cols=%d\n", var, row, col);
	for(int i = 0; i < row; ++i)
	{
		for(int j = 0; j < col; ++j)
			printf("%f   ", mat[i][j]);
		printf("\n");
	}
}

unsigned char **alloc_uchar_matrix(int height, int width)
{
    unsigned char **matrix = (unsigned char **) calloc(height, sizeof(unsigned char *));
    for(int i = 0; i < height; ++i)
        matrix[i] = (unsigned char *) calloc(width, sizeof(unsigned char));
    return matrix;
}

double **alloc_double_matrix(int height, int width)
{
    double **matrix = (double **) calloc(height, sizeof(double *));
    for(int i = 0; i < height; ++i)
        matrix[i] = (double *) calloc(width, sizeof(double));
    return matrix;
}


void dealloc_matrix(void **matrix, int rows)
{
    for(int i = 0; i < rows; ++i)
        free(matrix[i]);
    free(matrix);
}


Image *alloc_image(int height, int width, int channels)
{
    Image *image    = (Image *) scp(calloc(1, sizeof(Image)));

    image->red      = alloc_double_matrix(height, width);
    image->green    = alloc_double_matrix(height, width);
    image->blue     = alloc_double_matrix(height, width);
    image->alpha    = alloc_double_matrix(height, width);

    image->width    = width;
    image->height   = height;
    image->channels = channels;

    return image;
}


void dealloc_image(Image *image)
{
    dealloc_matrix((void **) image->red,   image->height);
    dealloc_matrix((void **) image->green, image->height);
    dealloc_matrix((void **) image->blue,  image->height);
    dealloc_matrix((void **) image->alpha, image->height);
    free(image);
}


void data_to_matrix(Image *image, unsigned char *data)
{
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            image->red[i][j]   = (double) data[image->channels * (i * image->width + j) + 0];
            image->green[i][j] = (double) data[image->channels * (i * image->width + j) + 1];
            image->blue[i][j]  = (double) data[image->channels * (i * image->width + j) + 2];
            if (image->channels == 4) // alpha
                image->alpha[i][j] = (double) data[image->channels * (i * image->width + j) + 3];
        }
    }
}


void matrix_to_data(Image *image, unsigned char *data)
{
    size_t k = 0;
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            data[k++] = (unsigned char) image->red[i][j];
            data[k++] = (unsigned char) image->green[i][j];
            data[k++] = (unsigned char) image->blue[i][j];
            if(image->channels == 4) // alpha
                data[k++] = (unsigned char) image->alpha[i][j];
        }
    }
}

void keep_first_k_singular(double **original, double **compr, int rows, int cols)
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            compr[i][j] = original[i][j];
}

void replace_pixels(double **old, double **new, int rows, int cols)
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            old[i][j] = new[i][j];
}