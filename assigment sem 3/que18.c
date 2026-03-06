#include <stdio.h>
#include <stdlib.h>

#define SIZE 10 // Size of hash table

// Node structure for Separate Chaining
struct Node {
    int key;
    struct Node* next;
};

// Primary hash function
int hash(int key) {
    return key % SIZE; // Simple modulo-based hash
}

/* ================= Linear Probing ================== */
void linearProbing() {
    int table[SIZE];
    // Initialize table with -1 (indicates empty slot)
    for (int i = 0; i < SIZE; i++) table[i] = -1;

    int keys[] = {10, 20, 30, 25, 35};
    int n = 5;

    // Insert keys
    for (int i = 0; i < n; i++) {
        int index = hash(keys[i]);
        // If collision, move to next slot linearly
        while (table[index] != -1)
            index = (index + 1) % SIZE;
        table[index] = keys[i];
    }

    // Display table
    printf("Hash Table (Linear Probing):\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d: %d\n", i, table[i]);
}

/* ================= Quadratic Probing ================== */
void quadraticProbing() {
    int table[SIZE];
    for (int i = 0; i < SIZE; i++) table[i] = -1;

    int keys[] = {10, 20, 30, 25, 35};
    int n = 5;

    for (int i = 0; i < n; i++) {
        int index = hash(keys[i]);
        int j = 0;
        int newIndex = index;
        // Resolve collision using quadratic formula: (index + j^2) % SIZE
        while (table[newIndex] != -1) {
            j++;
            newIndex = (index + j * j) % SIZE;
        }
        table[newIndex] = keys[i];
    }

    printf("\nHash Table (Quadratic Probing):\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d: %d\n", i, table[i]);
}

/* ================= Double Hashing ================== */
// Secondary hash function
int hash2(int key) {
    return 7 - (key % 7); // Step size for double hashing
}

void doubleHashing() {
    int table[SIZE];
    for (int i = 0; i < SIZE; i++) table[i] = -1;

    int keys[] = {10, 20, 30, 25, 35};
    int n = 5;

    for (int i = 0; i < n; i++) {
        int index = hash(keys[i]);
        int step = hash2(keys[i]);
        int j = 0;
        int newIndex = index;
        // Resolve collision using double hashing formula: (index + j * step) % SIZE
        while (table[newIndex] != -1) {
            j++;
            newIndex = (index + j * step) % SIZE;
        }
        table[newIndex] = keys[i];
    }

    printf("\nHash Table (Double Hashing):\n");
    for (int i = 0; i < SIZE; i++)
        printf("%d: %d\n", i, table[i]);
}

/* ================= Separate Chaining ================== */
void separateChaining() {
    struct Node* table[SIZE];
    // Initialize all buckets as NULL
    for (int i = 0; i < SIZE; i++) table[i] = NULL;

    int keys[] = {10, 20, 30, 25, 35};
    int n = 5;

    for (int i = 0; i < n; i++) {
        int index = hash(keys[i]);
        // Create new node for chaining
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->key = keys[i];
        // Insert at beginning of linked list
        newNode->next = table[index];
        table[index] = newNode;
    }

    printf("\nHash Table (Separate Chaining):\n");
    // Print each bucket
    for (int i = 0; i < SIZE; i++) {
        printf("%d: ", i);
        struct Node* curr = table[i];
        while (curr) {
            printf("%d -> ", curr->key);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}

int main() {
    // Linear Probing example
    linearProbing();

    // Quadratic Probing example
    quadraticProbing();

    // Double Hashing example
    doubleHashing();

    // Separate Chaining example
    separateChaining();

    return 0;
}
