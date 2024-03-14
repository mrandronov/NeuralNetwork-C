#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>

typedef enum fill_type
{
        ZEROS,
        RANDOM
} fill_type_t;

typedef struct
{
        int             rows;
        int             cols;
        double**        data;
} matrix_t;

double**
alloc_matrix( int rows, int cols );

matrix_t*
init_matrix( int rows, int cols );

void
print_matrix_size( matrix_t* m );

void
print_matrix( matrix_t* mat );

void
fill_matrix( matrix_t* mat, fill_type_t fill_type );

void
clear_matrix( matrix_t* mat );

matrix_t*
add_matrix( matrix_t* A, matrix_t* B );

matrix_t*
subtract_matrix( matrix_t* A, matrix_t* B );

matrix_t*
hadamard_product( matrix_t* A, matrix_t* B );

matrix_t*
dot_product( matrix_t* A, matrix_t* B );

matrix_t*
transpose( matrix_t* A );

matrix_t*
scalar_multiply( matrix_t* A, double s );

matrix_t*
activation( matrix_t* A );

matrix_t*
derivative_activation( matrix_t* A );

matrix_t*
copy_matrix( matrix_t* mat );

void
matrix_free( matrix_t* mat );

#endif /* __MATRIX_H__ */
