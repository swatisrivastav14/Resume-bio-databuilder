/*=========================================
    PROGRAM 7: STACK USING ARRAY
=========================================*/

#include <stdio.h>
#define MAX 5

int stack[MAX], top = -1;

// Push
void push(int val) {
    if (top == MAX - 1) {
        printf("Stack Overflow!\n");
        return;
    }
    stack[++top] = val;
}

// Pop
void pop() 
    if (top == -1) {
        printf("Stack Underflow!\n");
        return;
    }
    printf("Popped: %d\n", stack[top--]);
}

// Display
void display() {
    printf("Stack: ");
    for (int i = top; i >= 0; i--)
        printf("%d ", stack[i]);
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