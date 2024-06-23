#ifndef MATRIX_H
#define MATRIX_H

#include <complex.h>

#define M 30
#define N 40

void transpose(double complex A[M][N], double complex At[N][M]);
void multiply_mn_nm(double complex A[M][N], double complex B[N][M], double complex C[M][M]);
void multiply_nm_mm(double complex A[N][M], double complex B[M][M], double complex C[N][M]);
void identity(double complex Id[M][M]);
void inverse(double complex A[M][M], double complex A_inv[M][M]);
void calculate_pseudo_inverse(double complex A[M][N], double complex A_pinv[N][M]);

#endif
