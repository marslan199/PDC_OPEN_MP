#include <stdio.h>
#include <stdlib.h>
#include <omp.h>    // For OpenMP
#include <time.h>   // For time measurement

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
    return (i + 1);  // Returns the pivot index
}

// Parallel Quick Sort function using dynamic scheduling and #pragma omp for
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Partition the array and get the pivot index
        int pi = partition(arr, low, high);

        // Use parallel for loop for dynamic scheduling
        #pragma omp parallel for schedule(static)  // Dynamic scheduling
        for (int i = 0; i < 2; i++) {
            // We have two recursive calls, one for each subarray
            if (i == 0) {
                quickSort(arr, low, pi - 1);  // Left subarray (elements smaller than pivot)
            } else {
                quickSort(arr, pi + 1, high);  // Right subarray (elements larger than pivot)
            }
        }
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
    return ((double)(end - start)) / CLOCKS_PER_SEC;  // Return elapsed time in seconds
}

int main() {
    // Example array to be sorted
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 12, 34, 66, 88, 77, 44, 33, 55, 65, 7, 6, 89, 99, 12, 22, 24, 54, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    // Set the number of threads (you can adjust this value)
    omp_set_num_threads(4);  // Specify the number of threads to use

    // Record the start time
    clock_t start = clock();

    // Print the original array
    printf("Original array: ");
    printArray(arr, n);

    // Call the parallel quickSort function
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
