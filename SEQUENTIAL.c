#include <stdio.h>
#include <stdlib.h>
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

    clock_t start = clock();

    mergeSort(arr, 0, SIZE - 1);

    clock_t end = clock();

    printf("Sorting completed in %f seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);

    free(arr);
    return 0;
}
