#include <stdio.h>
#include "mymat.h"

int main(void) {
    mat m1, m2, m3;
    double values1[] = { 1, 1, 1, 1, 2, 2, 2, 2, -1, 1, 1, 1, 2, 2, 2, 2 };
    read_mat(&m1, values1, 16);

    double values2[] = { 1, 0, 1, 0, 2, 2, 0, 0, 1, 1, 0, 1, 1, 0, 2, 3 };
    read_mat(&m2, values2, 16);

    trans_mat(m1, &m2);

    printf("m1:\n");
    print_mat(m1);

    printf("\nm1^T:\n");
    print_mat(m2);

    return 0;
}
