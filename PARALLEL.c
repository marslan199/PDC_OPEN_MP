#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 5000000  // 5 million elements

// Merge two sorted subarrays into one sorted array
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

// Sequential merge sort
void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel merge sort using OpenMP tasks
void parallelMergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if ((right - left) > 1000) { // Only parallelize for large subarrays
            #pragma omp parallel
            {
                #pragma omp single nowait
                {
                    #pragma omp task
                    parallelMergeSort(arr, left, mid);

                    #pragma omp task
                    parallelMergeSort(arr, mid + 1, right);
                }
            }
        } else {
            mergeSort(arr, left, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int));
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100000; // Fill with random numbers

    printf("Sorting an array of %d elements...\n", SIZE);

    double start = omp_get_wtime();

    omp_set_num_threads(omp_get_max_threads()); // Use max available threads

    // Parallel for loop with dynamic scheduling for chunked sorting
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < SIZE; i += SIZE / 4) { // Split into 4 chunks
        parallelMergeSort(arr, i, i + (SIZE / 4) - 1);
    }

    // Final merge step to combine sorted chunks
    for (int step = SIZE / 4; step < SIZE; step *= 2) {
        for (int i = 0; i < SIZE; i += 2 * step) {
            int mid = i + step - 1;
            int right = (i + 2 * step - 1 < SIZE) ? i + 2 * step - 1 : SIZE - 1;
            merge(arr, i, mid, right);
        }
    }

    double end = omp_get_wtime();

    printf("Sorting completed in %f seconds.\n", end - start);

    free(arr);
    return 0;
}
