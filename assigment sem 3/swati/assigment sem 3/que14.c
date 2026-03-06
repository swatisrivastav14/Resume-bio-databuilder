/*=========================================
    PROGRAM 14: SPARSE MATRIX
    Represent using 3-Tuple (Row, Col, Value)
=========================================*/

#include <stdio.h>

#define MAX 100

// Function to convert matrix into 3-tuple
void sparseMatrix(int rows, int cols, int matrix[rows][cols]) {
    int tuple[MAX][3];
    int i, j, k = 1, count = 0;

    // Count non-zero elements
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (matrix[i][j] != 0)
                count++;
        }
    }

    // First row of tuple: [rows, cols, total non-zero]
    tuple[0][0] = rows;
    tuple[0][1] = cols;
    tuple[0][2] = count;

    // Fill tuple with non-zero elements
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (matrix[i][j] != 0) {
                tuple[k][0] = i;
                tuple[k][1] = j;
                tuple[k][2] = matrix[i][j];
                k++;
            }
        }
    }

    // Display Tuple Representation
    printf("\n3-Tuple Representation:\n");
    printf("Row Col Value\n");
    for (i = 0; i <= count; i++) {
        printf("%3d %3d %3d\n", tuple[i][0], tuple[i][1], tuple[i][2]);
    }
}

int main() {
    int matrix[4][5] = {
        {0, 0, 3, 0, 4},
        {0, 0, 5, 7, 0},
        {0, 0, 0, 0, 0},
        {0, 2, 6, 0, 0}
    };

    printf("Original Matrix:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    sparseMatrix(4, 5, matrix);

    return 0;
}