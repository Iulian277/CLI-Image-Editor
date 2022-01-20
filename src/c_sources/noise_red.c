#include "../headers/noise_red.h"
#include "../headers/image.h"
#include "../headers/rotate.h"

double **svd(double **A, int rows, int cols, int k)
{
    double **U = alloc_double_matrix(rows, cols);
    double **S = alloc_double_matrix(rows, cols);
    double **V = alloc_double_matrix(cols, cols);

    svd_lapack(A, rows, cols, U, S, V);

    // Keep only the first k singular values
    double **U_k = alloc_double_matrix(rows, k);
    double **S_k = alloc_double_matrix(k, k);
    double **V_k = alloc_double_matrix(cols, k);

    keep_first_k_singular(U, U_k, rows, k);
    keep_first_k_singular(S, S_k, k, k);
    keep_first_k_singular(V, V_k, cols, k);

    // Transpose 
    double **V_kt = alloc_double_matrix(k, cols);
    transpose_matrix(V_k, V_kt, cols, k);

    // printf("\nA_orig_00: %lf\n", A[0][0]);
    // print_matrix("A_orig", A, rows, cols);
	// print_matrix("U_k", U_k, rows, k);
	// print_matrix("S_k", S_k, k, k);
	// print_matrix("V_k", V, cols, k);
	// print_matrix("V_kt", V_kt, k, cols);

    // Multiply U_k * S_k * V_kt
    // U_k:  rows x k
    // S_k:  k x k
    // V_kt: k x cols

    // Multiply U_k * S_k: rows x k * k x k => rows x k
    double **US = alloc_double_matrix(rows, k);
    matrix_mult(U_k, S_k, US, rows, k, k);

    // Multiply US * V_kt: rows x k * k x cols => rows x cols
    double **A_rec = alloc_double_matrix(rows, cols);
    matrix_mult(US, V_kt, A_rec, rows, k, cols);
    // print_matrix("A_rec", A_rec, rows, cols);

    dealloc_matrix((void **) U, rows);
    dealloc_matrix((void **) S, rows);
    dealloc_matrix((void **) V, cols);
    dealloc_matrix((void **) US, rows);

    dealloc_matrix((void **) U_k, rows);
    dealloc_matrix((void **) S_k, k);
    dealloc_matrix((void **) V_k, cols);
    dealloc_matrix((void **) V_kt, k);

    // printf("A_rec_00: %lf\n", A_rec[0][0]);
    return A_rec;
}