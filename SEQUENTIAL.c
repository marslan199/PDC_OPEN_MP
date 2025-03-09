#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // For clock()

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function to partition the array around a pivot
int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Choose the last element as pivot
    int i = (low - 1);  // Index of the smaller element

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);  // Return the pivot index
}

// Quick Sort function
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pi = partition(arr, low, high);

        // Recursively sort the left and right subarrays
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to print the array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Function to calculate execution time
double calculateExecutionTime(clock_t start, clock_t end) {
    // Return the elapsed time in seconds
    return ((double)(end - start)) / CLOCKS_PER_SEC;
}

int main() {
    // Example array to be sorted
    int arr[] = {64, 34, 25, 12, 22, 11, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Record the start time
    clock_t start = clock();

    // Print the original array
    printf("Original array: ");
    printArray(arr, n);

    // Call the quickSort function
    quickSort(arr, 0, n - 1);

    // Record the end time
    clock_t end = clock();

    // Print the sorted array
    printf("Sorted array: ");
    printArray(arr, n);

    // Calculate and print the execution time
    double executionTime = calculateExecutionTime(start, end);
    printf("Execution time: %f seconds\n", executionTime);

    return 0;
}
