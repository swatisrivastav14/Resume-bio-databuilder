/*=========================================
    PROGRAM 20: SHELL SORT
    Sorting an array using Shell Sort algorithm
=========================================*/

#include <stdio.h>

// Function to perform Shell Sort
void shellSort(int arr[], int n) {
    // Start with a big gap, then reduce the gap
    for (int gap = n/2; gap > 0; gap /= 2) {
        // Perform insertion sort for this gap size
        for (int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            // Shift earlier gap-sorted elements up until the correct location for arr[i] is found
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
                arr[j] = arr[j - gap];
            arr[j] = temp;
        }
    }
}

// Function to display array
void display(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int arr[] = {12, 34, 54, 2, 3};
    int n = sizeof(arr)/sizeof(arr[0]);

    printf("Original Array: ");
    display(arr, n);

    // Sorting the array using Shell Sort
    shellSort(arr, n);

    printf("Sorted Array: ");
    display(arr, n);

    return 0;
}