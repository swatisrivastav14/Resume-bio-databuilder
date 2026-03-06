#include <stdio.h>
// Function to perform Linear Search
// Works on both sorted and unsorted arrays
int linearSearch(int arr[], int n, int key) {
    for (int i = 0; i < n; i++) {
        if (arr[i] == key)
            return i; // return index if element is found
    }
    return -1; // return -1 if element not found
}

// Function to perform Binary Search
// Works only when array is sorted
int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1, mid;

    while (low <= high) {
        mid = (low + high) / 2;

        if (arr[mid] == key)
            return mid; // element found at index mid
        else if (arr[mid] < key)
            low = mid + 1; // search in right half
        else
            high = mid - 1; // search in left half
    }
    return -1; // return -1 if not found
}

int main() {
    int n, choice, key, result;

    // Input size of array
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n]; // array declaration

    // Input elements
    printf("Enter %d elements (sorted if you want to use Binary Search):\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Input element to be searched
    printf("Enter element to search: ");
    scanf("%d", &key);

    // Give user option to choose search method
    printf("Choose search method:\n");
    printf("1. Linear Search\n");
    printf("2. Binary Search\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    // Perform search according to choice
    if (choice == 1) {
        result = linearSearch(arr, n, key);
    } else if (choice == 2) {
        result = binarySearch(arr, n, key);
    } else {
        printf("Invalid choice!\n");
        return 0;
    }

    // Display result
    if (result != -1)
        printf("Element found at position %d\n", result + 1); // 1-based index
    else
        printf("Element not found.\n");

    return 0;
}