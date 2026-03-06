#include <stdio.h>
// Function to perform Bubble Sort
void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform Selection Sort
void selectionSort(int arr[], int n) {
    int i, j, minIndex, temp;
    for (i = 0; i < n - 1; i++) {
        minIndex = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex])
                minIndex = j;
        }
        // Swap arr[i] and arr[minIndex]
        temp = arr[i];
        arr[i] = arr[minIndex];
        arr[minIndex] = temp;
    }
}

// Function to perform Insertion Sort
void insertionSort(int arr[], int n) {
    int i, j, key;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // Shift elements greater than key to one position ahead
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// Function to display the array
void display(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() 
{
    int n, choice;

    // Input number of elements
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];

    // Input array elements
    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++)
     {
        scanf("%d", &arr[i]);
    }

    // Menu for sorting methods
    printf("Choose Sorting Method:\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // Perform sorting based on choice
    switch (choice) 
    {
        case 1:
            bubbleSort(arr, n);
            printf("Sorted array using Bubble Sort: ");
            break;
        case 2:
            selectionSort(arr, n);
            printf("Sorted array using Selection Sort: ");
            break;
        case 3:
            insertionSort(arr, n);
            printf("Sorted array using Insertion Sort: ");
            break;
        default:
            printf("Invalid choice!\n");
            return 0;
    }

    // Display sorted array
    display(arr, n);

    return 0;
}