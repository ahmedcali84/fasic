#define FASIC_IMPLEMENTATION
#include "../fasic.h"

#define COLS 5
#define ROWS 5

int main(void)
{
    srand(time(NULL));
    Fasic_Matrix random_mat = random_matrix(ROWS, COLS);
    PRINT(random_mat);
    PRINT_SHAPE(random_mat);

    HashMap hashmap;
    hashmap_init(&hashmap);

    hashmap_insert(&hashmap, "Ahmed");
    hashmap_insert(&hashmap, "Ali");
    hashmap_insert(&hashmap, "Mohammed");
    hashmap_insert(&hashmap, "Ahmed");
    hashmap_print(&hashmap);

    hashmap_destroy(&hashmap);
    return 0;
}
