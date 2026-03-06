#include <stdio.h>// For printf
#include <stdlib.h>// For malloc, free, rand
#include <time.h>// For random number generation

#define MAXLVL 3   // Maximum levels
#define PROB 0.5   // Probability for level increase

// Node structure
typedef struct Node // Define Node structure
    {
    int key;                     // Value
    struct Node* forward[MAXLVL+1]; // Forward pointers
} Node;

// Create a new node
Node* createNode(int key, int level)// Create a new node with given key and level
 {
    Node* n = (Node*)malloc(sizeof(Node));// Allocate memory for new node
    n->key = key;
    for (int i = 0; i <= level; i++)// Initialize forward pointers to NULL
        n->forward[i] = NULL;// Set forward pointer at level i to NULL
    return n;
}

// Generate random level for node
int randomLevel() {
    int lvl = 0;
    while (((double)rand() / RAND_MAX) < PROB && lvl < MAXLVL)// Increase level with probability PROB
        lvl++;
    return lvl;
}

// Insert a key
void insert(Node* header, int key)// Insert a new key into the skip list
 {
    Node* update[MAXLVL+1];// Array to hold nodes at each level
    Node* curr = header;// Start from header node

    // Find position for new node
    for (int i = MAXLVL; i >= 0; i--) 
    {
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
        update[i] = curr;
    }

    curr = curr->forward[0];

    if (!curr || curr->key != key) {
        int lvl = randomLevel();
        Node* newNode = createNode(key, lvl);

        for (int i = 0; i <= lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        printf("Inserted: %d at level %d\n", key, lvl);
    }
}

// Search a key
int search(Node* header, int key) {
    Node* curr = header;
    for (int i = MAXLVL; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
    }
    curr = curr->forward[0];
    return (curr && curr->key == key);
}

// Delete a key
void delete(Node* header, int key) {
    Node* update[MAXLVL+1];
    Node* curr = header;

    for (int i = MAXLVL; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
        update[i] = curr;
    }

    curr = curr->forward[0];

    if (curr && curr->key == key) {
        for (int i = 0; i <= MAXLVL; i++) {
            if (update[i]->forward[i] != curr)
                break;
            update[i]->forward[i] = curr->forward[i];
        }
        free(curr);
        printf("Deleted: %d\n", key);
    } else {
        printf("Key %d not found!\n", key);
    }
}

// Display skip list
void display(Node* header) {
    printf("\nSkip List:\n");
    for (int i = 0; i <= MAXLVL; i++) {
        Node* node = header->forward[i];
        printf("Level %d: ", i);
        while (node) {
            printf("%d -> ", node->key);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
}

// Main function
int main() {
    srand(time(0));
    Node* header = createNode(-1, MAXLVL); // Dummy header

    insert(header, 10);
    insert(header, 20);
    insert(header, 30);
    insert(header, 15);

    display(header);

    int key = 20;
    if (search(header, key))
        printf("\nFound %d\n", key);
    else
        printf("\n%d Not Found\n", key);

    delete(header, 15);
    display(header);

    delete(header, 100); // Trying to delete non-existing key

    return 0;
}
