
/*=========================================
    PROGRAM 5: CIRCULAR LINKED LIST IN C
    Operations:
    1. Insertion
    2. Deletion
    3. Searching
    4. Reversing
=========================================*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

// Insert at end
void insert(struct Node** head, int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;

    if (*head == NULL) {
        *head = newNode;
        newNode->next = *head;
        return;
    }

    struct Node* temp = *head;
    while (temp->next != *head)
        temp = temp->next;

    temp->next = newNode;
    newNode->next = *head;
}

// Delete node
void deleteNode(struct Node** head, int val) {
    if (*head == NULL) return;

    struct Node *curr = *head, *prev = NULL;

    // If head to be deleted
    if (curr->data == val) {
        while (curr->next != *head)
            curr = curr->next;
        struct Node* del = *head;
        curr->next = (*head)->next;
        *head = (*head)->next;
        free(del);
        return;
    }

    curr = *head;
    do {
        prev = curr;
        curr = curr->next;
        if (curr->data == val) {
            prev->next = curr->next;
            free(curr);
            return;
        }
    } while (curr != *head);
}

// Search
int search(struct Node* head, int val) {
    if (head == NULL) return 0;
    struct Node* temp = head;
    do {
        if (temp->data == val) return 1;
        temp = temp->next;
    } while (temp != head);
    return 0;
}

// Reverse
void reverse(struct Node** head) {
    if (*head == NULL) return;
    struct Node *prev = NULL, *curr = *head, *next;
    do {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    } while (curr != *head);
    (*head)->next = prev;
    *head = prev;
}

// Display
void display(struct Node* head) {
    if (head == NULL) return;
    struct Node* temp = head;
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while (temp != head);
    printf("(back to head)\n");
}

int main() {
    struct Node* list = NULL;

    insert(&list, 10);
    insert(&list, 20);
    insert(&list, 30);
    display(list);

    deleteNode(&list, 20);
    display(list);

    printf("Search 30: %s\n", search(list, 30) ? "Found" : "Not Found");

    reverse(&list);
    display(list);

    return 0;
}