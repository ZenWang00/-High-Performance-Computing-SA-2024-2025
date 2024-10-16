const char* dgemm_desc = "Naive, three-loop dgemm.";

#include <omp.h>

void dgemm_naive(int n, double* A, double* B, double* C) {
    #pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}
