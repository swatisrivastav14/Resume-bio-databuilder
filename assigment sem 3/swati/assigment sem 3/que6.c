/*=========================================
    PROGRAM 6: STACK USING LINKED LIST
=========================================*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* top = NULL;

// Push
void push(int val) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = val;
    newNode->next = top;
    top = newNode;
}

// Pop
void pop() {
    if (top == NULL) {
        printf("Stack Underflow!\n");
        return;
    }
    struct Node* temp = top;
    printf("Popped: %d\n", temp->data);
    top = top->next;
    free(temp);
}

// Display
void display() {
    struct Node* temp = top;
    printf("Stack: ");
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main() {
    push(10);
    push(20);
    push(30);
    display();

    pop();
    display();

    return 0;
}