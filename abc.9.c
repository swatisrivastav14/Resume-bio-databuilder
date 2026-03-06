#include <stdio.h>
#define SIZE 10

int arr[SIZE];
int top1 = -1;
int top2 = SIZE;

// Push to Stack 1
void push1(int x) {
    if (top1 < top2 - 1) {
        top1++;
        arr[top1] = x;
    } else {
        printf("Stack Overflow in Stack 1\n");
    }
}

// Push to Stack 2
void push2(int x) {
    if (top1 < top2 - 1) {
        top2--;
        arr[top2] = x;
    } else {
        printf("Stack Overflow in Stack 2\n");
    }
}

// Pop from Stack 1
int pop1() {
    if (top1 >= 0) {
        int x = arr[top1];
        top1--;
        return x;
    } else {
        printf("Stack Underflow in Stack 1\n");
        return -1;
    }
}

// Pop from Stack 2
int pop2() {
    if (top2 < SIZE) {
        int x = arr[top2];
        top2++;
        return x;
    } else {
        printf("Stack Underflow in Stack 2\n");
        return -1;
    }
}

int main() {
    push1(10);
    push1(20);
    push2(90);
    push2(80);

    printf("Pop from Stack 1: %d\n", pop1());
    printf("Pop from Stack 2: %d\n", pop2());

    return 0;
}
