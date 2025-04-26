#define FASIC_IMPLEMENTATION
#include "../fasic.h"

#define COLS 5
#define ROWS 5

typedef struct _matrix_data {
    Fasic_Matrix *mat;
    const char *key;
} Matrix_Data;

int main(void)
{
    srand(time(NULL));
    Fasic_Matrix random_mat = random_matrix(ROWS, COLS);
    PRINT(random_mat);
    PRINT_SHAPE(random_mat);

    Matrix_Data mat_data = {.mat = &random_mat, .key = "random_matri"};
    HashMap hashmap;
    hashmap_init(&hashmap);

    hashmap_insert(&hashmap, mat_data);
    hashmap_destroy(&hashmap);
    return 0;
}
