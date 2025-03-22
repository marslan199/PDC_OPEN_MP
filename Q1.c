#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_SIZE 2000  // Max matrix size
#define NUM_THREADS 4  // Fixed number of threads

// Function to initialize matrix
void initialize_matrix(double A[MAX_SIZE][MAX_SIZE], int N) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            A[i][j] = (i + j) * 0.01;
}

// Column-major loop interchange
void loop_interchange_c(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], int N) {
    double start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int j = 0; j < N; j++) {   // Column major
        for (int i = 0; i < N; i++) {
            A[i][j] += B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Loop Interchange Column Time for %d x %d: %.6f sec\n", N, N, end - start);
}

// Row-major loop interchange
void loop_interchange_r(double A[MAX_SIZE][MAX_SIZE], double B[MAX_SIZE][MAX_SIZE], int N) {
    double start = omp_get_wtime();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {   // Row major
        for (int j = 0; j < N; j++) {
            A[i][j] += B[i][j];
        }
    }

    double end = omp_get_wtime();
    printf("Loop Interchange Row Time for %d x %d: %.6f sec\n", N, N, end - start);
}

int main() {
    int sizes[] = {512, 1024, 2000};
    omp_set_num_threads(NUM_THREADS);  // Set fixed number of threads

    for (int i = 0; i < 3; i++) {
        int N = sizes[i];
        static double A[MAX_SIZE][MAX_SIZE], B[MAX_SIZE][MAX_SIZE];
        initialize_matrix(A, N);
        initialize_matrix(B, N);

        printf("\n--- Running with Matrix Size %d x %d, %d Threads ---\n", N, N, NUM_THREADS);

        loop_interchange_c(A, B, N);
        loop_interchange_r(A, B, N);
    }

    return 0;
}
