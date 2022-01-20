#include "../headers/commons.h"
#include "../headers/image.h"
#include "../headers/rotate.h"
#include "../headers/noise_red.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb/stb_image_write.h"

void replace_pixels(double **old, double **new, int rows, int cols)
{
    for(int i = 0; i < rows; ++i)
        for(int j = 0; j < cols; ++j)
            old[i][j] = new[i][j];
}

int main(void)
{
    int width, height, comp;
    unsigned char *pixels = stbi_load("./images/img_before_compression.jpg", &width, &height, &comp, STBI_rgb);

    Image *image = alloc_image(height, width, STBI_rgb);

    data_to_matrix(image, pixels);
    double **A_rec_red   = svd(image->red,   height, width, MIN(width, height));
    double **A_rec_green = svd(image->green, height, width, MIN(width, height));
    double **A_rec_blue  = svd(image->blue,  height, width, MIN(width, height));

    // int idx = 5;
    // printf("%f %f %f\n", image->red[idx][idx], image->green[idx][idx], image->blue[idx][idx]);
    // printf("%f %f %f\n", A_rec_red[idx][idx], A_rec_green[idx][idx], A_rec_blue[idx][idx]);
    
    replace_pixels(image->red,   A_rec_red,   height, width);
    replace_pixels(image->green, A_rec_green, height, width);
    replace_pixels(image->blue,  A_rec_blue,  height, width);


    // image->red[idx][idx] = 0;
    // image->green[idx][idx] = 0;
    // image->blue[idx][idx] = 0;
    matrix_to_data(image, pixels); // overwrite the original 'data' array of pixels
    stbi_write_png("./images/png4_out.png", width, height, comp, pixels, width * comp);




    dealloc_image(image);
    free(pixels);
    
    return 0;
}

    // https://stackoverflow.com/questions/15830913/lapack-matrix-multiplication-with-c
    // https://scicomp.stackexchange.com/questions/20021/blas-lapack-or-atlas-for-matrix-multiplication-in-c
    // http://www.netlib.org/lapack/explore-html/d1/d54/group__double__blas__level3_gaeda3cbd99c8fb834a60a6412878226e1.html



    // int rows = 5;
    // // int cols = 6;
    // // double **A = alloc_double_matrix(rows, cols);
    // // double **U = alloc_double_matrix(rows, cols);
    // // double **S = alloc_double_matrix(rows, cols);
    // // double **V = alloc_double_matrix(cols, cols);

    // // int idx = 1;
    // // for(int i = 0; i < rows; ++i)
    // //     for(int j = 0; j < cols; ++j)
    // //         A[i][j] = idx++;

    // // svd_lapack(A, rows, cols, U, S, V);
    
    // // Keep only the first k singular values
    // int k = 2;
    // double **U_k = alloc_double_matrix(rows, k);
    // double **S_k = alloc_double_matrix(k, k);
    // double **V_k = alloc_double_matrix(cols, k);

    // keep_first_k_singular(U, U_k, rows, k);
    // keep_first_k_singular(S, S_k, k, k);
    // keep_first_k_singular(V, V_k, cols, k);

    // // Transpose 
    // double **V_kt = alloc_double_matrix(k, cols);
    // transpose_matrix(V_k, V_kt, cols, k);

    // print_matrix("A_orig", A, rows, cols);
	// // print_matrix("U_k", U_k, rows, k);
	// // print_matrix("S_k", S_k, k, k);
	// // print_matrix("V_k", V, cols, k);
	// // print_matrix("V_kt", V_kt, k, cols);

    // // Multiply U_k * S_k * V_kt
    // // U_k:  rows x k
    // // S_k:  k x k
    // // V_kt: k x cols

    // // Multiply U_k * S_k: rows x k * k x k => rows x k
    // double **US = alloc_double_matrix(rows, k);
    // matrix_mult(U_k, S_k, US, rows, k,k);
    // // print_matrix("US", US, rows, k);

    // // // Multiply US * V_kt: rows x k * k x cols => rows x cols
    // double **A_rec = alloc_double_matrix(rows, cols);
    // matrix_mult(US, V_kt, A_rec, rows, k, cols);
    // print_matrix("A_rec", A_rec, rows, cols);