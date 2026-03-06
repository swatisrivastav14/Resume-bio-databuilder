/*=========================================
    PROGRAM 11: FIBONACCI SERIES
    (i) Using Recursion
    (ii) Using Iteration
=========================================*/

#include <stdio.h>

// Recursion
int fibRec(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fibRec(n - 1) + fibRec(n - 2);
}

// Iteration
void fibIter(int n) {
    int a = 0, b = 1, c;
    printf("Fibonacci (Iteration): ");
    for (int i = 0; i < n; i++) {
        printf("%d ", a);
        c = a + b;
        a = b;
        b = c;
    }
    printf("\n");
}

int main() {
    int n = 10;

    printf("Fibonacci (Recursion): ");
    for (int i = 0; i < n; i++)
        printf("%d ", fibRec(i));
    printf("\n");

    fibIter(n);

    return 0;
}