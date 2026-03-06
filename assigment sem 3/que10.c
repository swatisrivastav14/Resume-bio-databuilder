/*=========================================
    PROGRAM 10: FACTORIAL & FACTORS
    (i) Using Recursion
    (ii) Using Iteration
=========================================*/

#include <stdio.h>

// Factorial (Recursion)
int factRec(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factRec(n - 1);
}

// Factorial (Iteration)
int factIter(int n) {
    int result = 1;
    for (int i = 1; i <= n; i++)
        result *= i;
    return result;
}

// Factors (Recursion)
void factorsRec(int n, int i) {
    if (i > n) return;
    if (n % i == 0) printf("%d ", i);
    factorsRec(n, i + 1);
}

// Factors (Iteration)
void factorsIter(int n) {
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) printf("%d ", i);
    }
}

int main() {
    int num = 6;

    printf("Factorial (Recursion): %d\n", factRec(num));
    printf("Factorial (Iteration): %d\n", factIter(num));

    printf("Factors (Recursion): ");
    factorsRec(num, 1);
    printf("\n");

    printf("Factors (Iteration): ");
    factorsIter(num);
    printf("\n");

    return 0;
}