#define FASIC_IMPLEMENTATION
#include "fasic.h"

int main (void)
{
    char *build_test[] = {"gcc", "-Wall", "-Werror", "-Wextra", "-g", "-std=c11", "-o", "test", "Test/tests.c", NULL};
    if (!build_c_file(build_test)) return 1;

    return 0;
}
