#pragma once

#include <stdio.h>
#include <stdlib.h>

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
fill_matrix( matrix_t* mat );

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
free_matrix( matrix_t* mat );

