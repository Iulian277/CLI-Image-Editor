#include "../headers/rotate.h"

Image *rotate_image(Image *image, int angle)
{
    Image *rotated = alloc_image(1 * image->height, 1 * image->width, image->channels);

    double sinx = SIND(angle);
    double cosx = COSD(angle);

    int x_center = image->height;
    int y_center = image->width;

    for(size_t x = 0; x < image->height; ++x)
    {
        int xt = x - x_center;
        double xt_cosx = xt * cosx;
        double xt_sinx = xt * sinx;

        for(size_t y = 0; y < image->width; ++y)
        {
            int yt = y - y_center;
            long x_rotate = lround(xt_cosx - (yt * sinx)) + x_center;
            long y_rotate = lround((yt * cosx) + xt_sinx) + y_center;

            // check if alpha channel
            if((x_rotate >= 0) && (x_rotate < image->height) && 
               (y_rotate >= 0) && (y_rotate < image->width))
            {
                rotated->red[x][y]   = image->red[x_rotate][y_rotate];
                rotated->green[x][y] = image->green[x_rotate][y_rotate];
                rotated->blue[x][y]  = image->blue[x_rotate][y_rotate];
            }
            else
            {   // black pixel
                rotated->red[x][y]   = 0;
                rotated->green[x][y] = 0;
                rotated->blue[x][y]  = 0;
            }
        }
    }
    
    return rotated;
}

void transpose_matrix(double **original, double **transp, int rows, int cols) // rows and cols for the original matrix
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            transp[j][i] = original[i][j];
}

// void transpose_tile(int row, int col, int size, int **matrix) {
//     if (size > 4) {
//         transpose_tile(row, col, size / 2, matrix);
//         transpose_tile(row, col + size / 2, size / 2, matrix);
//         transpose_tile(row + size / 2, col, size / 2, matrix);
//         transpose_tile(row + size / 2, col + size / 2, size / 2, matrix);
//     } else {
//         for (int a = 0; a < size; a++) {
//             for (int b = 0; b < size; b++) {
//                 int tmp = matrix[row + a][col + b];
//                 matrix[row + a][col + b] = matrix[col + b][row + a];
//                 matrix[col + b][row + a] = tmp;
//             }
//         }
//     }
// }

// void transpose_tile_diag(int pos, int size, double **matrix) {
//     if (size > 4) {
//         transpose_tile_diag(pos, size / 2, matrix);
//         transpose_tile(pos, pos + size / 2, size / 2, matrix);
//         transpose_tile_diag(pos + size / 2, size / 2, matrix);
//     } else {
//         /* swap values on either side of the first diagonal */
//         for (int a = 1; a < size; a++) {
//             /* stop the inner loop when b == a */
//             for (int b = 0; b < a; b++) {
//                 int tmp = matrix[pos + a][pos + b];
//                 matrix[pos + a][pos + b] = matrix[pos + b][pos + a];
//                 matrix[pos + b][pos + a] = tmp;
//             }
//         }
//     }
// }


void svd_lapack(double **A, int row, int col, double **U, double **S, double **V)
{
	double *tmpa = (double *) calloc(row * col, sizeof(double));
	double *tmpu = (double *) calloc(row * row, sizeof(double));
	double *tmpv = (double *) calloc(col * col, sizeof(double));
	double *tmps = (double *) calloc(col	  , sizeof(double));

	// Flatten the 2d matrix
	int ioff = 0;
	for(int i = 0; i < col; ++i)
		for(int j = 0; j < row; ++j)
			tmpa[ioff++] = A[j][i];

	// Query and allocate the optimal workspace
	int info, lwork = -1;
	double wkopt;
	double *work;
	dgesvd_("All", "All", &row, &col, tmpa, &row, tmps, tmpu, &row, tmpv, &col, &wkopt, &lwork, &info);

	lwork = (int) wkopt;
	work = (double *) calloc(lwork, sizeof(double));

	// Compute SVD
	dgesvd_("All", "All", &row, &col, tmpa, &row, tmps, tmpu, &row, tmpv, &col, work, &lwork, &info);

	// Check for convergence
	if(info > 0) {
		printf("The algorithm computing SVD failed to converge.\n");
		exit(1);
	}
	// tmpU (1D) -> U (2D)
	ioff = 0;
	for(int i = 0; i < row; ++i)
		for(int j = 0; j < row; ++j)
			U[j][i] = tmpu[ioff++];

	// tmpV (1D) -> V (2D)
	ioff = 0;
	for(int i=0; i < col; ++i)
		for(int j = 0; j < col; ++j)
			V[i][j] = tmpv[ioff++];
	

	// tmpS (1D) -> S (2D)
	for(int i = 0; i < MIN(row, col); ++i)
		S[i][i] = tmps[i];

	// Free allocated memory
	free(work);
	free(tmpa);
	free(tmpu);
	free(tmpv);
	free(tmps);
}

void matrix_mult(double **A, double **B, double **C, int rowsA, int colsA, int colsB)
{
	for(int i = 0; i < rowsA; ++i)
	{
		for(int j = 0; j < colsB; ++j)
		{
			C[i][j] = 0;
			for(int k = 0; k < colsA; ++k)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
}