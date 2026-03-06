#include <stdio.h>
#define SIZE 40

int arr[SIZE];
int top1 = -1;
int top2 = SIZE;

// Push to Stack 1
void push1(int x) {
    if (top1 < top2 - 1) {
        arr[++top1] = x;
    } else {
        printf("Stack Overflow in Stack 1\n");
    }
}

// Push to Stack 2
void push2(int x) {
    if (top1 < top2 - 1) {
        arr[--top2] = x;
    } else {
        printf("Stack Overflow in Stack 2\n");
    }
}

// Pop from Stack 1
int pop1() {
    if (top1 >= 0) {
        return arr[top1--];
    } else {
        printf("Stack Underflow in Stack 1\n");
        return -1;
    }
}

// Pop from Stack 2
int pop2() {
    if (top2 < SIZE) {
        return arr[top2++];
    } else {
        printf("Stack Underflow in Stack 2\n");
        return -1;
    }
}

// Display contents of both stacks
void showStacks() {
    int i;

    printf("\nStack 1: ");
    if (top1 >= 0) {
        for (i = 0; i <= top1; i++) {
            printf("%d ", arr[i]);
        }
    } else {
        printf("Empty");
    }

    printf("\nStack 2: ");
    if (top2 < SIZE) {
        for (i = SIZE - 1; i >= top2; i--) {
            printf("%d ", arr[i]);
        }
    } else {
        printf("Empty");
    }

    printf("\n");
}

int main() {
    int choice, value;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Push to Stack 1\n");
        printf("2. Push to Stack 2\n");
        printf("3. Pop from Stack 1\n");
        printf("4. Pop from Stack 2\n");
        printf("5. Exit\n");
        printf("6. Show Stacks\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to push into Stack 1: ");
                scanf("%d", &value);
                push1(value);
                break;

            case 2:
                printf("Enter value to push into Stack 2: ");
                scanf("%d", &value);
                push2(value);
                break;

            case 3:
                value = pop1();
                if (value != -1)
                    printf("Popped from Stack 1: %d\n", value);
                break;

            case 4:
                value = pop2();
                if (value != -1)
                    printf("Popped from Stack 2: %d\n", value);
                break;

            case 5:
                printf("Exiting...\n");
                return 0;

            case 6:
                showStacks();
                break;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
