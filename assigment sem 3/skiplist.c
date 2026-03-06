#include <stdio.h>// For printf
#include <stdlib.h> // For malloc, free, rand
#include <time.h> // For random number generation

#define MAXLVL 5   // Maximum level (0 to 5)
#define PROB 0.5      // Probability for increasing node height

// Node structure of skip list
struct Node // Represents each node in skip list
 {
    int key;                               // Data stored in node
    struct Node* forward[MAXLVL + 1];      // Forward pointers for each level
};

// Function to create a new node with given key
struct Node* createNode(int key)
{ 
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));// Allocate memory for new node
    node->key = key;// Set node's key

    // Set all forward pointers to NULL initially
    for (int i = 0; i <= MAXLVL; i++)// Initialize forward pointers
        node->forward[i] = NULL;// Set forward pointer at level i to NULL

    return node;// Return the created node
}
// Function to generate a random level height for a new node
int randomLevel()// Generates random level for new node
 {
    int lvl = 0;

    // Increase level while random value < PROB (0.5) and lvl < MAXLVL
    while (((double)rand() / RAND_MAX) < PROB && lvl < MAXLVL)// Randomly decide level
        lvl++;

    return lvl;      // Return final level
}

// Insert a key into skip list
void insert(struct Node* header, int key)
 {

    struct Node* update[MAXLVL + 1];   // Stores last visited node at each level
    struct Node* curr = header;// Start from header node

    // STEP 1: Search the correct position to insert
    for (int i = MAXLVL; i >= 0; i--) 
    {
        // Move forward in the same level while key is larger
        while (curr->forward[i] && curr->forward[i]->key < key)// Move forward while next node key is less than insert key
            curr = curr->forward[i];

        update[i] = curr;  // Save current position for insertion level-wise
    }

    curr = curr->forward[0]; // Go to next node at level 0

    // STEP 2: Insert only if key is NOT already present
    if (!curr || curr->key != key) {

        int lvl = randomLevel();      // Randomly choose node height
        struct Node* newNode = createNode(key);

        // STEP 3: Insert node at all levels from 0 to lvl
        for (int i = 0; i <= lvl; i++) {

            // Set new node's forward pointer
            newNode->forward[i] = update[i]->forward[i];

            // Connect previous node to new node
            update[i]->forward[i] = newNode;
        }
    }
}

// Function to search a key in skip list
int search(struct Node* header, int key)
 {
    struct Node* curr = header;// Start from header node

    // Start from highest level and move down
    for (int i = MAXLVL; i >= 0; i--) {

        // Move forward while next node key is less than search key
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
    }

    // Now at level 0 — check the next node
    curr = curr->forward[0];

    // If matched, return found
    return (curr && curr->key == key);
}

// Function to display skip list level-wise
void display(struct Node* header) {
    printf("\nSkip List:\n");

    for (int i = 0; i <= MAXLVL; i++) {

        struct Node* node = header->forward[i];
        printf("Level %d: ", i);

        while (node) {
            printf("%d -> ", node->key);
            node = node->forward[i];
        }
        printf("NULL\n");
    }
}

// Main function
int main()
 {


    srand(time(0));                    // Seed random generator

    struct Node* header = createNode(-1);   // Create header node (dummy key)

    // Insert data into skip list
    insert(header, 10);
    insert(header, 20);
    insert(header, 30);
    insert(header, 15);
    insert(header, 25);
    insert(header, 5);
    insert(header, 35);

    display(header);  // Show skip list

    // Search for key 20
    int key = 20;

    if (search(header, key))// If found 
        printf("\nFound %d\n", key);// Print found message
    else
        printf("\nNot Found!\n");

    return 0;
}
