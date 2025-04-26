#define FASIC_IMPLEMENTATION
#include "../fasic.h"

#define COLS 5
#define ROWS 5

int main(void)
{
    Fasic_Matrix random_mat = random_matrix(ROWS, COLS);
    PRINT(random_mat);
    PRINT_SHAPE(random_mat);
    return 0;
}
