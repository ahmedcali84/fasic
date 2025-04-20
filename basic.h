/*
 
This Header Only Library is a collection of libraries I find myself using often.

 */


#ifndef BASIC_H_
#define BASIC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <assert.h>
#include <time.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

typedef enum {
    INFO,
    ERROR,
    DEBUG,
    WARN,
    COUNT_LEVELS,
} Log_Levels;

void Logging(FILE *stream, Log_Levels level, const char *fmt, ...);

#define Log_Out(level, fmt, ...) Logging(stdout, level, fmt, ##__VA_ARGS__)
#define Log_File(stream, level, fmt, ...) Logging(stream, level, fmt, ##__VA_ARGS__)

// Matrix Structure and Information
typedef struct _basic_matrix {
    float *A;     // Pointer to the array of matrix elements
    size_t nrows; // Number of rows in the matrix
    size_t ncols; // Number of columns in the matrix
} Basic_Matrix;

typedef struct _basic_shape {
    size_t rows;
    size_t cols;
} Basic_Shape;

/*
************** MATRIX OPERATIONS FUNCTIONS DECLARATIONS *******************
*/

// Core Functions
float get_element(Basic_Matrix C, size_t row, size_t col);                      // get specific element from the Basic_Matrix
void set_element(Basic_Matrix C, size_t row, size_t col, float new_value);      // set a specific element from the Basic_Matrix to a new value
Basic_Matrix create_matrix(size_t rows, size_t cols);                           // creates an empty Basic_Matrix with specified parameters
void unload(Basic_Matrix *b);                                                   // Function to free Allocated Memory

// Utility Functions
Basic_Matrix random_matrix(size_t nrows, size_t ncols);                         // generates random Basic_Matrix(nrows x ncols)
void print_matrix(const Basic_Matrix b, const char *name);                      // prints a Basic_Matrix
Basic_Shape matrix_shape(Basic_Matrix A);                                             // returns shape of the Basic_Matrix
Basic_Matrix fill(size_t nrows, size_t ncols, float fill_value);                // creates a Basic_Matrix filled with a specific value

// Operation Functions
Basic_Matrix matrix_add(Basic_Matrix *a, Basic_Matrix *b);                                  // adds two matrices
Basic_Matrix matrix_subtract(Basic_Matrix *a, Basic_Matrix *b);                             // subtracts two matrices
Basic_Matrix hadamard_product(Basic_Matrix *a, Basic_Matrix *b);                            // computes element-wise product of two matrices
Basic_Matrix dot_product(Basic_Matrix *a, Basic_Matrix *b);                                 // computes the dot-product of two matrices
Basic_Matrix transpose(Basic_Matrix *a);                                              // return transpose of Basic_Matrix a
bool test_matrix_equal(Basic_Matrix a, Basic_Matrix b);                               // test matrices for equality


// Special Macro Functions
#define PRINT(B) print_matrix(B, #B)                                /* Macro definition of a special print function */
#define ALLOCATION_FAILED "Memory Allocation Failed.\n"             /* Return Error String */

/* Macro function For Printing the shape of a Basic_Matrix */
#define PRINT_SHAPE(A)\
    do {\
        Basic_Shape shape = matrix_shape(*A);\
        printf("Basic_Shape: [%zu, %zu]\n", shape.rows, shape.cols);\
    } while (0)

#ifdef __cplusplus
}
#endif

#define BUCKETS_SIZE 100000

typedef struct Dictionary {
    char *key;
    unsigned int value;
} Dictionary;

typedef struct HashMap {
	Dictionary *buckets;
	unsigned int count;
} HashMap;

unsigned int hash_function(char *key);
bool hashmap_init(HashMap *hashmap);
bool hashmap_insert(HashMap *hashmap, char *key);
bool hashmap_delete(HashMap *hashmap, char *key);
unsigned int hashmap_lookup(HashMap *hashmap, char *key);
void hashmap_print(HashMap *hashmap, unsigned int iteration);
void hashmap_destroy(HashMap *hashmap);

#ifdef BASIC_IMPLEMENTATION

/* CUSTOM LOGGING FUNCTIONS IMPLEMENTATION BEGIN */

const char * Levels[COUNT_LEVELS] = {
    [INFO]  = "INFO",
    [ERROR] = "ERROR",
    [DEBUG] = "DEBUG",
    [WARN]  = "WARN",
};

void Logging(FILE *stream, Log_Levels level, const char *fmt, ...)
{
    fprintf(stream, "[%s]: " , Levels[level]);

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);
}

/* CUSTOM LOGGING FUNCTIONS IMPLEMENTATION ENDS */

/*
************** MATRIX OPERATIONS FUNCTIONS IMPLEMENTATIONS *******************
*/

float get_element(Basic_Matrix C, size_t row, size_t col) {
    // check for valid indices
    if (row >= C.nrows || col >= C.ncols) {
        Log_File(stderr, ERROR, "index out of bounds: [%zu, %zu] for Basic_Matrix size [%zu, %zu]\n",
                row, col, C.nrows, C.ncols);
        exit(EXIT_FAILURE); // handle the error, exit or return an error value
    }
    
    return C.A[row * C.ncols + col];
}

void set_element(Basic_Matrix C, size_t row, size_t col, float new_value) {
    // set the element at the specified row and column to a new value
    C.A[row * C.ncols + col] = new_value;
}

Basic_Matrix create_matrix(size_t rows, size_t cols) {
    Basic_Matrix create;
    create.nrows = rows;
    create.ncols = cols;
    create.A = calloc(sizeof(float), (rows * cols));
    if (create.A == NULL) {
        Log_File(stderr, ERROR, ALLOCATION_FAILED);
        exit(EXIT_FAILURE);
    }

    return create;
}

Basic_Matrix random_matrix(size_t nrows, size_t ncols) {
    // initialize the Basic_Matrix
    Basic_Matrix random = create_matrix(nrows, ncols);

    // fill the Basic_Matrix with random values
    for (size_t i = 0; i < random.nrows; ++i) {
        for (size_t j = 0; j < random.ncols; ++j) {
            float v = ((float)rand() / RAND_MAX) * 1000 - 500;
            set_element(random, i , j , v);
        }
    }

    return random; // return the generated Basic_Matrix
}

void print_matrix(const Basic_Matrix b, const char *name) {
    // print the Basic_Matrix with its name
    printf("%s = [\n", name);

    for (size_t i = 0; i < b.nrows; ++i) {
        for (size_t j = 0; j < b.ncols; ++j) {
            float value = get_element(b, i , j);
            printf(" %.2f ", value);
        }
        printf("\n");
    }
    printf("]"); // end of Basic_Matrix
    printf("\n\n");
}

Basic_Shape matrix_shape(Basic_Matrix A) {
    // shape of the Basic_Matrix (rows, columns)
    Basic_Shape shape = {.rows = A.nrows, .cols = A.ncols};
    return shape;
}

Basic_Matrix matrix_add(Basic_Matrix *A, Basic_Matrix *B) {
    // check if the dimensions of the matrices are the same
    assert((A->ncols == B->ncols && B->nrows == A->nrows) && "addition failed, matrices shape mismatch");

    // initialize a new Basic_Matrix for the result
    Basic_Matrix C = create_matrix(A->nrows, A->ncols);
    
    for (size_t i = 0; i < A->nrows; ++i) {
        for (size_t j = 0; j < A->ncols; ++j) {
            float a = get_element(*A, i , j);
            float b = get_element(*B, i , j);
            float c = a + b;
            set_element(C, i , j , c);
        }
    }

    return C; // return the result Basic_Matrix
}

Basic_Matrix matrix_subtract(Basic_Matrix *A, Basic_Matrix *B) {
    // check if the dimensions of the matrices are the same
    assert((A->ncols == B->ncols && B->nrows == A->nrows) && "subtraction failed, matrices shape mismatch");

    // initialize a new Basic_Matrix for the result
    Basic_Matrix C = create_matrix(A->nrows, A->ncols);

    for (size_t i = 0; i < A->nrows; ++i) {
        for (size_t j = 0; j < A->ncols; ++j) {
            float a = get_element(*A, i , j);
            float b = get_element(*B, i , j);
            float c = a - b;
            set_element(C, i , j , c);
        }
    }

    return C; // return the result
}

Basic_Matrix hadamard_product(Basic_Matrix *A, Basic_Matrix *B) {
    // ensure matrices are of the same size and type
    assert(A->ncols == B->ncols && "matrices dimensions mismatch");
    assert(A->nrows == B->nrows && "matrices dimensions mismatch");

    // initialize a new Basic_Matrix for the result
    Basic_Matrix C = create_matrix(A->nrows, A->ncols);

    // calculate element-wise product
    for (size_t i = 0; i < A->nrows; ++i) {
        for (size_t j = 0; j < A->ncols; ++j) {
            float a , b , c;
            a = get_element(*A, i , j);
            b = get_element(*B, i , j);
            c = a * b;
            set_element(C, i , j , c);
        }
    }

    return C; // return the result Basic_Matrix
}

Basic_Matrix dot_product(Basic_Matrix *A, Basic_Matrix *B) {
    // check if the matrices can be multiplied
    assert((A->ncols == B->nrows) && "error multiplying: dimensions mismatch");

    // initialize a new Basic_Matrix for the result
    Basic_Matrix C = create_matrix(A->nrows, B->ncols);

    // calculate the dot product
    for (size_t i = 0; i < C.nrows; ++i) {
        for (size_t j = 0; j < C.ncols; ++j) {
            float c = 0;
            for (size_t k = 0; k < A->ncols; ++k) {
                float a , b;
                a = get_element(*A, i , k );
                b = get_element(*B, k , j );
                c += a * b;
            }
            set_element(C , i , j , c);
        }
    }

    return C; // return the result Basic_Matrix
}

Basic_Matrix transpose(Basic_Matrix *A) {
    // initialize a new Basic_Matrix for the result
    Basic_Matrix C = create_matrix(A->ncols, A->nrows);

    for (size_t i = 0; i < A->nrows; ++i) {
        for (size_t j = 0; j < A->ncols; ++j) {
            float a;
            a = get_element(*A, i , j);
            set_element(C, j , i , a);
        }
    }

    return C; // return the transposed Basic_Matrix
}

bool test_matrix_equal(Basic_Matrix A, Basic_Matrix B) {
    // check if the dimensions match
    assert(((A.ncols == B.ncols) || (A.nrows == B.nrows))&& "not equal: different dimensions.");

    // check if all elements are equal
    for (size_t i = 0; i < A.nrows; ++i) {
        for (size_t j = 0; j < A.ncols; ++j) {
            float a , b;
            a = get_element(A, i, j);
            b = get_element(B, i, j); 
            if (a != b) {
                return false; // not equal
            }
        }
    }

    return true; // equal
}

Basic_Matrix fill(size_t nrows, size_t ncols, float fill_value) {
    // initialize Basic_Matrix
    Basic_Matrix a = create_matrix(nrows, ncols);

    // fill the Basic_Matrix with the specified value
    for (size_t i = 0; i < nrows; ++i) {
        for (size_t j = 0; j < ncols; ++j) {
            set_element(a, i , j , fill_value); // set all elements
        }
    }

    return a; // return the filled Basic_Matrix
}

void unload(Basic_Matrix *B) {
    // free allocated memory for the Basic_Matrix
    if (B->A != NULL)
    {
        free(B->A); // free the array of elements
        B->A = NULL; // avoid dangling pointer
    }

    B->nrows = 0; // reset rows
    B->ncols = 0; // reset columns
}

/* MATRIX FUNCTION IMPLEMENTATIONS ENDS */


/* HASHMAP FUNCTION IMPLEMENTATION BEGINS */
unsigned int hash_function(char *key)
{
    unsigned int hash = 0;
    unsigned int slen = strlen(key);

    for (unsigned int i = 0; i < slen && key[i] != '\0'; ++i) {
        hash = 31 * hash + key[i];
    }

    return hash % BUCKETS_SIZE;
}

bool hashmap_init(HashMap *hashmap)
{
    hashmap->buckets = calloc(BUCKETS_SIZE , sizeof(Dictionary));
    if (!hashmap->buckets) return false;
    hashmap->count = 0;
    return true;
}

bool hashmap_insert(HashMap *hashmap, char *key)
{
    unsigned int hash = hash_function(key);
    Dictionary *bucket = &hashmap->buckets[hash];
    
    if (bucket->key == NULL) {
        unsigned int slen = strlen(key);
        bucket->key = malloc(sizeof(char)* (slen + 1));
        if (bucket->key == NULL) {
            free(bucket->key);
            return false;
        }
        strcpy(bucket->key, key);
        bucket->value = 1;
        hashmap->count++;
        return true;
    } else if (strcmp(bucket->key, key) == 0) {
        bucket->value +=1;
        return true;
    }

    for (unsigned int i = 1; i < BUCKETS_SIZE; ++i) {
        unsigned int idx = (hash + i) % BUCKETS_SIZE;
        bucket = &hashmap->buckets[idx];
        if (bucket->key == NULL) {
            unsigned int slen = strlen(key);
            bucket->key = malloc(sizeof(char)* (slen + 1));
            if (bucket->key == NULL) {
                free(bucket->key);
                return false;
            }
            strcpy(bucket->key, key);
            bucket->value = 1;
            hashmap->count++;
            return true;
        } else if (strcmp(bucket->key, key) == 0) {
            bucket->value +=1;
            return true;
        }
    }
    return false;
}

bool hashmap_delete(HashMap *hashmap, char *key)
{
    unsigned int hash = hash_function(key);
    Dictionary *temp = &hashmap->buckets[hash];
    if (temp == NULL) {
        return false;
    }

    free(hashmap->buckets[hash].key);
    hashmap->buckets[hash].key = NULL;
    hashmap->buckets[hash].value = 0;
    hashmap->count--;

    return true;
}

unsigned int hashmap_lookup(HashMap *hashmap, char *key)
{
    if (!hashmap) exit(EXIT_FAILURE);

    unsigned int hash = hash_function(key);
    for (unsigned int i = 0; i < BUCKETS_SIZE; ++i) {
        unsigned int index = (hash + i) % BUCKETS_SIZE;
        Dictionary *bucket = &hashmap->buckets[index];

        if (bucket->key == NULL) {
            return 0;
        }

        else if (strcmp(key, bucket->key) == 0) {
            return bucket->value;
        }
    }
    return 0;
}

void hashmap_print(HashMap *hashmap, unsigned int iteration)
{
    for (unsigned int i = 0; i < iteration; ++i) {
        Dictionary *temp = &hashmap->buckets[i];
        if (temp->key != NULL) {
            Log_Out(INFO, "%s: %d .\n", temp->key, temp->value);
        }
    }
}

void hashmap_destroy(HashMap *hashmap)
{
    for (unsigned int i = 0; i < BUCKETS_SIZE; ++i) {
        free(hashmap->buckets[i].key);
    }
    free(hashmap->buckets);
    hashmap->count = 0;
}

/* HASHMAP FUNCTION IMPLEMENTATION ENDS */

#endif // BASIC_IMPLEMENTATION

#endif // BASIC_H_
