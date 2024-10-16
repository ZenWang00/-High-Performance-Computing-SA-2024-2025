const char* dgemm_desc = "Blocked dgemm.";

#include <omp.h>

void dgemm_blocked(int n, double* A, double* B, double* C, int block_size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i += block_size) {
        for (int j = 0; j < n; j += block_size) {
            for (int k = 0; k < n; k += block_size) {
                // Compute C[i:i+block_size][j:j+block_size]
                for (int ii = i; ii < i + block_size && ii < n; ++ii) {
                    for (int jj = j; jj < j + block_size && jj < n; ++jj) {
                        double sum = C[ii * n + jj];
                        for (int kk = k; kk < k + block_size && kk < n; ++kk) {
                            sum += A[ii * n + kk] * B[kk * n + jj];
                        }
                        C[ii * n + jj] = sum;
                    }
                }
            }
        }
    }
}
