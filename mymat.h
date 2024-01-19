#define MAT_WIDTH   4
#define MAT_HEIGHT  4

typedef struct {
    double cells[MAT_HEIGHT][MAT_WIDTH];
} mat;

void read_mat(mat *pm, const double values[], int count);
void print_mat(mat matrix);
void add_mat(mat mat1, mat mat2, mat *dest);
void sub_mat(mat mat1, mat mat2, mat *dest);
void mul_mat(mat mat1, mat mat2, mat *dest);
void mul_scalar(mat matrix, double scalar, mat *dest);
void trans_mat(mat matrix, mat *dest);
void reset_mat(mat *pm);
