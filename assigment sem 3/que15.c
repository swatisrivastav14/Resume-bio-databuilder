#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLVL 3      // Maximum level of skip list
#define PROB 0.3      // Probability for random level

// Node structure for skip list
struct Node
{
    int key;                // Value stored in node
    struct Node* forward[MAXLVL + 1]; // Array of forward pointers for each level
};

// Create a new node
struct Node* createNode(int key) 
{
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->key = key;
    for (int i = 0; i <= MAXLVL; i++)
        node->forward[i] = NULL; // Initialize forward pointers as NULL
    return node;
}

// Generate random level for node
int randomLevel()
{
    int lvl = 0;
    while (((double)rand() / RAND_MAX) < PROB && lvl < MAXLVL)
        lvl++;
    return lvl;
}

// Insert a key into skip list
void insert(struct Node* header, int key) 
{
    struct Node* update[MAXLVL + 1];
    struct Node* curr = header;

    // Find place to insert
    for (int i = MAXLVL; i >= 0; i--)
     {
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
        update[i] = curr;
    }

    curr = curr->forward[0];

    if (!curr || curr->key != key) {
        int lvl = randomLevel();          // Random level for new node
        struct Node* newNode = createNode(key);
        for (int i = 0; i <= lvl; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }
}

// Search for a key in skip list
int search(struct Node* header, int key) {
    struct Node* curr = header;
    for (int i = MAXLVL; i >= 0; i--) {
        while (curr->forward[i] && curr->forward[i]->key < key)
            curr = curr->forward[i];
    }
    curr = curr->forward[0];
    return (curr && curr->key == key); // Return 1 if found, 0 otherwise
}

// Display skip list
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

int main() {
    srand(time(0));
    struct Node* header = createNode(-1); // Header node with dummy key

    // Insert elements
    insert(header, 10);
    insert(header, 20);
    insert(header, 30);
    insert(header, 15);
    insert(header, 25);
    insert(header, 5);

    display(header);  // Display skip list

    int key = 20;     // Search for a key
    if (search(header, key)) printf("\nFound %d\n", key);
    else printf("\nNot Found!\n");

    return 0;
}