/*=========================================
    PROGRAM 8: QUEUE USING CIRCULAR ARRAY
=========================================*/

#include <stdio.h>
#define MAX 5

int queue[MAX];
int front = -1, rear = -1;

// Enqueue
void enqueue(int val) {
    if ((rear + 1) % MAX == front) {
        printf("Queue Overflow!\n");
        return;
    }
    if (front == -1) front = 0;
    rear = (rear + 1) % MAX;
    queue[rear] = val;
}

// Dequeue
void dequeue() {
    if (front == -1) {
        printf("Queue Underflow!\n");
        return;
    }
    printf("Dequeued: %d\n", queue[front]);
    if (front == rear)
        front = rear = -1;
    else
        front = (front + 1) % MAX;
}

// Display
void display() {
    if (front == -1) {
        printf("Queue is Empty!\n");
        return;
    }
    printf("Queue: ");
    int i = front;
    while (1) {
        printf("%d ", queue[i]);
        if (i == rear) break;
        i = (i + 1) % MAX;
    }
    printf("\n");
}

int main() {
    enqueue(10);
    enqueue(20);
    enqueue(30);
    display();

    dequeue();
    display();

    return 0;
}