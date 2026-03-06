/*=========================================
    PROGRAM 9: DOUBLE ENDED QUEUE (DEQUE)
=========================================*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* front = NULL;
struct Node* rear = NULL;

// Insert at front
void insertFront(int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->prev = NULL;
    newNode->next = front;
    if (front != NULL) front->prev = newNode;
    else rear = newNode;
    front = newNode;
}

// Insert at rear
void insertRear(int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->next = NULL;
    newNode->prev = rear;
    if (rear != NULL) rear->next = newNode;
    else front = newNode;
    rear = newNode;
}

// Delete from front
void deleteFront() {
    if (front == NULL) {
        printf("Deque Underflow!\n");
        return;
    }
    struct Node* temp = front;
    printf("Deleted: %d\n", temp->data);
    front = front->next;
    if (front != NULL) front->prev = NULL;
    else rear = NULL;
    free(temp);
}

// Delete from rear
void deleteRear() {
    if (rear == NULL) {
        printf("Deque Underflow!\n");
        return;
    }
    struct Node* temp = rear;
    printf("Deleted: %d\n", temp->data);
    rear = rear->prev;
    if (rear != NULL) rear->next = NULL;
    else front = NULL;
    free(temp);
}

// Display
void display() {
    struct Node* temp = front;
    printf("Deque: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    insertRear(10);
    insertRear(20);
    insertFront(5);
    display();

    deleteFront();
    display();

    deleteRear();
    display();

    return 0;
}