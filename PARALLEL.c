#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define SIZE 5000000  // 5 million elements
#define CHUNK_SIZE 500000  // 500,000 elements per chunk

// Merge two sorted subarrays into one sorted array
void merge(int arr[], int temp[], int left, int mid, int right) {
    int i = left;       // Starting index for left subarray
    int j = mid + 1;    // Starting index for right subarray
    int k = left;       // Starting index for temporary array

    // Merge the two subarrays into the temporary array
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    // Copy remaining elements of the left subarray (if any)
    while (i <= mid)
        temp[k++] = arr[i++];

    // Copy remaining elements of the right subarray (if any)
    while (j <= right)
        temp[k++] = arr[j++];

    // Copy the merged elements back into the original array
    for (i = left; i <= right; i++)
        arr[i] = temp[i];
}

// Sequential merge sort
void mergeSort(int arr[], int temp[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);
        merge(arr, temp, left, mid, right);
    }
}

// Parallel merge sort using OpenMP tasks
void parallelMergeSort(int arr[], int temp[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if ((right - left) > 1000) { // Only parallelize for large subarrays
            #pragma omp task
            parallelMergeSort(arr, temp, left, mid);

            #pragma omp task
            parallelMergeSort(arr, temp, mid + 1, right);

            #pragma omp taskwait
        } else {
            mergeSort(arr, temp, left, right);
        }

        merge(arr, temp, left, mid, right);
    }
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int));
    int *temp = (int *)malloc(SIZE * sizeof(int)); // Temporary array for merging

    if (!arr || !temp) {
        printf("Memory allocation failed in main!\n");
        return 1;
    }

    // Initialize the array with random numbers
    double init_start = omp_get_wtime();
    srand(time(NULL));
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100000; // Fill with random numbers
    double init_end = omp_get_wtime();
    printf("Array initialization completed in %f seconds.\n", init_end - init_start);

    printf("Sorting an array of %d elements...\n", SIZE);

    double start = omp_get_wtime();

    omp_set_num_threads(omp_get_max_threads()); // Use max available threads

    // Calculate the number of chunks
    int num_chunks = SIZE / CHUNK_SIZE;
    if (SIZE % CHUNK_SIZE != 0) {
        num_chunks++;
    }

    // Parallel sorting of chunks
    double parallel_start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = 0; i < num_chunks; i++) {
                int left = i * CHUNK_SIZE;
                int right = (left + CHUNK_SIZE - 1 < SIZE) ? left + CHUNK_SIZE - 1 : SIZE - 1;
                printf("Chunk %d: left = %d, right = %d\n", i, left, right); // Debug print
                #pragma omp task
                parallelMergeSort(arr, temp, left, right);
            }
        }
    }
    double parallel_end = omp_get_wtime();
    printf("Parallel sorting of chunks completed in %f seconds.\n", parallel_end - parallel_start);

    // Final merge step to combine sorted chunks
    double final_merge_start = omp_get_wtime();
    for (int step = CHUNK_SIZE; step < SIZE; step *= 2) {
        for (int i = 0; i < SIZE; i += 2 * step) {
            int mid = i + step - 1;
            int right = (i + 2 * step - 1 < SIZE) ? i + 2 * step - 1 : SIZE - 1;
            printf("Final merge: i = %d, mid = %d, right = %d\n", i, mid, right); // Debug print
            merge(arr, temp, i, mid, right);
        }
    }
    double final_merge_end = omp_get_wtime();
    printf("Final merge step completed in %f seconds.\n", final_merge_end - final_merge_start);

    double end = omp_get_wtime();

    printf("Total sorting completed in %f seconds.\n", end - start);

    free(arr);
    free(temp);
    return 0;
}