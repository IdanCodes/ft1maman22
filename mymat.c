#include <stdio.h>
#include "mymat.h"
#include "utils.h"

/* read_mat: set the given matrix's first #count cells' values to the matching given values; the rest will be set to 0.
 * Assuming count <= values' length */
void read_mat(mat *pm, const double values[], int count) {
    int i;

    count = (int)min(count, MAT_WIDTH * MAT_HEIGHT);    /* clamp count */

    for (i = 0; i < count; i++)
        pm->cells[i / 4][i % 4] = values[i];    /* mapping the values array's index to the cells' index */

    for (; i < MAT_WIDTH * MAT_HEIGHT; i++)
        pm->cells[i / 4][i % 4] = 0;      /* reset remaining values to 0 */
}

/* print_mat: print the given matrix */
void print_mat(mat matrix) {
    int i, j;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++)
            printf("%7.2lf ", matrix.cells[i][j]);
        putchar('\n');
    }
}

/* add_mat: add the matrices mat1 and mat2, store the result into dest */
void add_mat(mat mat1, mat mat2, mat *dest) {
    int i, j;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++)
            dest->cells[i][j] = mat1.cells[i][j] + mat2.cells[i][j];
    }
}

/* sub_mat: subtract the matrices mat1 and mat2, store the result into dest */
void sub_mat(mat mat1, mat mat2, mat *dest) {
    int i, j;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++)
            dest->cells[i][j] = mat1.cells[i][j] - mat2.cells[i][j];
    }
}

/* mul_mat: multiply the matrices mat1 and mat2, store the result into dest */
void mul_mat(mat mat1, mat mat2, mat *dest) {
    int i, j, k;
    double sum;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++) {
            for (k = 0, sum = 0; k < MAT_WIDTH; k++)
                sum += mat1.cells[i][k] * mat2.cells[k][j];

            dest->cells[i][j] = sum;
        }
    }
}

/* mul_scalar: multiply the given matrix by the given scalar; store the result into dest */
void mul_scalar(mat matrix, double scalar, mat *dest) {
    int i, j;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++)
            dest->cells[i][j] = matrix.cells[i][j] * scalar;
    }
}

/* trans_mat: transpose the given matrix and store the result into dest */
void trans_mat(mat matrix, mat *dest) {
    int i, j;

    for (i = 0; i < MAT_HEIGHT; i++) {
        for (j = 0; j < MAT_WIDTH; j++)
            dest->cells[i][j] = matrix.cells[j][i];
    }
}

/* reset_mat: reset the given matrix's cells' values to 0 */
void reset_mat(mat *pm) {
    const double empty_values[] = { 0 };
    read_mat(pm, empty_values, 0);
}
