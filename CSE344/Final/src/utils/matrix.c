#include "utils/matrix.h"

void transpose(double complex A[M][N], double complex At[N][M]) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            At[j][i] = conj(A[i][j]);
        }
    }
}

void multiply_mn_nm(double complex A[M][N], double complex B[N][M], double complex C[M][M]) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiply_nm_mm(double complex A[N][M], double complex B[M][M], double complex C[N][M]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < M; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void identity(double complex Id[M][M]) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            Id[i][j] = (i == j) ? 1 : 0;
        }
    }
}

void inverse(double complex A[M][M], double complex A_inv[M][M]) {
    double complex Id[M][M];
    identity(Id);

    // This is a simplified method for inverting a matrix using Gauss-Jordan elimination
    for (int i = 0; i < M; ++i) {
        double complex diag = A[i][i];
        for (int j = 0; j < M; ++j) {
            A[i][j] /= diag;
            Id[i][j] /= diag;
        }
        for (int k = 0; k < M; ++k) {
            if (k != i) {
                double complex factor = A[k][i];
                for (int j = 0; j < M; ++j) {
                    A[k][j] -= factor * A[i][j];
                    Id[k][j] -= factor * Id[i][j];
                }
            }
        }
    }
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            A_inv[i][j] = Id[i][j];
        }
    }
}

void calculate_pseudo_inverse(double complex A[M][N], double complex A_pinv[N][M]) {
    double complex At[N][M];
    double complex AtA[M][M];
    double complex AtA_inv[M][M];

    transpose(A, At);
    multiply_mn_nm(A, At, AtA);
    inverse(AtA, AtA_inv);
    multiply_nm_mm(At, AtA_inv, A_pinv);
}
