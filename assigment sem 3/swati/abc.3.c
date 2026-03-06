#include <stdio.h>
#include <stdlib.h>

// Node structure
struct Node {
    int data;
    struct Node* next;
};

// Insert at end
void insert(struct Node** head, int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }
    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Delete a node
void deleteNode(struct Node** head, int val) {
    if (*head == NULL) return;

    struct Node* temp = *head;

    // If head node to be deleted
    if (temp->data == val) {
        *head = temp->next;
        free(temp);
        return;
    }

    // Search for node to delete
    struct Node* prev = NULL;
    while (temp != NULL && temp->data != val) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Element not found!\n");
        return;
    }
