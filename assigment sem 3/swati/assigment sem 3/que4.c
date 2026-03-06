#include <stdio.h>
#include <stdlib.h>

// Node structure
struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

// Insert at end
void insert(struct Node** head, int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    struct Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    newNode->prev = temp;
}

// Delete a node
void deleteNode(struct Node** head, int val) {
    if (*head == NULL) {
        printf("List is empty!\n");
        return;
    }

    struct Node* temp = *head;

    // If head is to be deleted
    if (temp->data == val) {
        *head = temp->next;
        if (*head != NULL)
            (*head)->prev = NULL;
        free(temp);
        return;
    }

    while (temp != NULL && temp->data != val)
        temp = temp->next;

    if (temp == NULL) {
        printf("Element not found!\n");
        return;
    }

    if (temp->next != NULL)
        temp->next->prev = temp->prev;
    if (temp->prev != NULL)
        temp->prev->next = temp->next;

    free(temp);
}

// Search element
int search(struct Node* head, int val) {
    struct Node* temp = head;
    while (temp != NULL) {
        if (temp->data == val)
            return 1;
        temp = temp->next;
    }
    return 0;
}

// Reverse the list
void reverse(struct Node** head) {
    struct Node* curr = *head;
    struct Node* temp = NULL;

    while (curr != NULL) {
        // Swap prev and next
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev; 
    }

    if (temp != NULL)
        *head = temp->prev;
}

// Display list
void display(struct Node* head) {
    struct Node* temp = head;
    printf("NULL <-> ");
    while (temp != NULL) {
        printf("%d <-> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// Main function
int main() {
    struct Node* list = NULL;

    printf("--- Insertion ---\n");
    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 30);
    display(list);

    printf("--- Deletion of 20 ---\n");
    deleteNode(&list, 20);
    display(list);

    printf("--- Searching 30 ---\n");
    if (search(list, 30))
        printf("Found\n");
    else
        printf("Not Found\n");

    printf("--- Reversing the list ---\n");
    reverse(&list);
    display(list);

    return 0;
}
