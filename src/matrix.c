#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "matrix.h"

char*
get_error_message( char const* operationString, char const* msg )
{
        size_t          bufSize = snprintf( NULL, 0, "In %s(..) %s!\n.", operationString, msg ); 
        char*           buf = ( char* ) malloc( bufSize + 1 );

        sprintf( buf, "In %s(...): %s!\n.", operationString, msg );

        return buf;
}

void assert_msg( int condition, char *msg )
{
        if ( !condition )
        {
                return;
        }

        printf( "Error: %s\n", msg );

        exit( 1 );
}


double**
alloc_matrix( int rows, int cols )
{
        double**        arr = ( double ** ) malloc( rows * sizeof( double * ) );

        for ( int i = 0; i < rows; i++ )
        {
                arr[ i ] = ( double * ) calloc( cols, sizeof( double * ) );
        }

        return arr;
}


matrix_t*
init_matrix( int rows, int cols )
{
        matrix_t*          mat = ( matrix_t* ) calloc( 1, sizeof( matrix_t ) );

        mat->rows = rows;
        mat->cols = cols;
        mat->data = alloc_matrix( rows, cols );

        return mat;
}


void
print_matrix( matrix_t* mat )
{
        for ( int i = 0; i < mat->rows; i++ )
        {
                for ( int j = 0; j < mat->cols; j++ )
                {
                        printf( "%.15f ", mat->data[ i ][ j ] );
                }

                printf( "\n" );
        }

        printf( "\n" );
}

double
random_value()
{
        return ( double )( rand( )-( RAND_MAX/2 ) ) / ( double )( RAND_MAX );
}

void
fill_matrix( matrix_t* mat )
{
        srand( time( NULL ) );

        for ( int i = 0; i < mat->rows; i++ )
        {
                for ( int j = 0; j < mat->cols; j++ )
                {
                        mat->data[ i ][ j ] = random_value();
                }
        }
}


void
clear_matrix( matrix_t* mat )
{
        for ( int i = 0; i < mat->rows; i++ )
        {
                for ( int j = 0; j < mat->cols; j++ )
                {
                        mat->data[ i ][ j ] = 0; 
                }
        }
}

double
add( double a, double b )
{
        return a + b;
}

double
subtract( double a, double b )
{
        return a - b;
}

double
multiply( double a, double b )
{
        return a * b;
}

char* get_operation_as_string( double ( *operation ) ( double, double ) )
{
        if ( operation == add )
        {
                return "Add";
        }
        else if ( operation == subtract )
        {
                return "Subtract";
        }
        else if ( operation == multiply )
        {
                return "Multiply";
        }
        return "No available operation function!";
}

/*
        operate( ... ) implements Adding, Subtracting, and Multiplying
        two matrices, supplied as arguments A and B. The return value
        should be the result of the operation. If any error occurs,
        an error message is printed and the process is exited.

        These operations are purely scalar operations on the matrix 
        arguments A and B. Their dimensions must be the same in this 
        case and the output matrix will also have the same dimensions.
 */

matrix_t*
operate( 
        matrix_t* A,
        matrix_t* B,
        double ( *operation )( double, double ) )
{
        char*           operationString = get_operation_as_string( operation );

        assert_msg( ( A->rows != B->rows ), 
                get_error_message( operationString, "A->rows != B->rows" ) );

        assert_msg( ( A->cols != B->cols ), 
                get_error_message( operationString, "A->cols != B->cols" ) );

        matrix_t*               result = init_matrix( A->rows, A->cols );

        for ( int i = 0; i < A->rows; i++ )
        {
                for ( int j = 0; j < A->cols; j++ )
                {
                        result->data[ i ][ j ] = operation( A->data[ i ][ j ], B->data[ i ][ j ] );

                        if ( isnan( result->data[ i ][ j ] ) || !isfinite( result->data[ i ][ j ] ) )
                        {
                                printf( "A->data is %f\n", A->data[ i ][ j ] );
                                printf( "B->data is %f\n", B->data[ i ][ j ] );
                                printf( "Result is %f\n", result->data[ i ][ j ] );
                        }

                        assert_msg( isnan( result->data[ i ][ j ] ), 
                                get_error_message( operationString, "operation result is NAN!" ) );

                        assert_msg( !isfinite( result->data[ i ][ j ] ),
                                get_error_message( operationString, "result is infinite!" ) );
                }
        }

        return result;
}

matrix_t*
add_matrix( matrix_t* A, matrix_t* B )
{
        return operate( A, B, add );
}

matrix_t*
subtract_matrix( matrix_t* A, matrix_t* B )
{
        return operate( A, B, subtract );
}

matrix_t*
hadamard_product( matrix_t* A, matrix_t* B )
{
        return operate( A, B, multiply );
}

matrix_t*
dot_product( matrix_t* A, matrix_t* B )
{
        assert_msg( ( A->cols != B->rows ), 
                "in dot_product( ): A->cols != B->rows" );

        matrix_t*               result = init_matrix( A->rows, B->cols );

        for ( int i = 0; i < result->rows; i++ )
        {
                for ( int j = 0; j < result->cols; j++ )
                {
                        for ( int k = 0; k < A->cols; k++ )
                        {
                                result->data[ i ][ j ] += A->data[ i ][ k ] * B->data[ k ][ j ];

                                assert_msg( isnan( result->data[ i ][ j ] ), 
                                        "in dot_product( ): operation result is NAN!" );

                                assert_msg( !isfinite( result->data[ i ][ j ] ), 
                                        "in dot_product( ): operation result is infinite!" );
                        }
                }
        }

        return result;
}

matrix_t*
transpose( matrix_t* A )
{
        matrix_t*               result = init_matrix( A->cols, A->rows );

        for ( int i = 0; i < A->rows; i++ )
        {
                for ( int j = 0; j < A->cols; j++ )
                {
                        result->data[ j ][ i ] = A->data[ i ][ j ];
                }
        }

        return result;
}

matrix_t*
scalar_multiply( matrix_t* A, double s )
{
        matrix_t*               result = init_matrix( A->rows, A->cols );
        for ( int i = 0; i < A->rows; i++ )
        {
                for ( int j = 0; j < A->cols; j++ )
                {
                        result->data[ i ][ j ] = A->data[ i ][ j ] * s;

                        if ( isnan( result->data[ i ][ j ] ) || !isfinite( result->data[ i ][ j ] ) )
                        {
                                printf( "Uh oh, NaN or infinity detected! -> %f * %f == %f\n", A->data[ i ][ j ], s, ( A->data[ i ][ j ] * s ) );
                        }
                        assert_msg( isnan( result->data[ i ][ j ] ), "in scalar_multiply( ): operation result is NAN!" );
                        assert_msg( !isfinite( result->data[ i ][ j ] ), "in scalar_multiply( ): operation result is infinite!" );
                }
        }

        return result;
}

double
sigmoid( double x )
{
        return ( 1 / ( 1 + exp( -x ) ) );
}

matrix_t*
activation( matrix_t* A )
{
        matrix_t*               result = init_matrix( A->rows, A->cols );

        for ( int i = 0; i < A->rows; i++ )
        {
                for ( int j = 0; j < A->cols; j++ )
                {
                        result->data[ i ][ j ] = sigmoid( A->data[ i ][ j ] );

                        assert_msg( isnan( result->data[ i ][ j ] ), "in activation( ): operation result is NAN!" );
                        assert_msg( !isfinite( result->data[ i ][ j ] ), "in activation( ): operation result is infinite!" );
                }
        }

        return result;
}

matrix_t*
derivative_activation( matrix_t* A )
{
        matrix_t*               result = init_matrix( A->rows, A->cols );

        for ( int i = 0; i < A->rows; i++ )
        {
                for ( int j = 0; j < A->cols; j++ )
                {
                        double          num = sigmoid( A->data[ i ][ j ] );

                        if ( !isfinite( num ) )
                        {
                                num = 1.0;
                        }

                        result->data[ i ][ j ] = num * ( 1 - sigmoid( A->data[ i ][ j ] ) );

                        assert_msg( isnan( result->data[ i ][ j ] ), "in derivative_activation( ): operation result is NAN!" );

                        assert_msg( !isfinite( result->data[ i ][ j ] ), "in derivative_activation( ): operation result is infinite!" );
                }
        }

        return result;
}

matrix_t*
copy_matrix( matrix_t* mat )
{
        matrix_t*          result = init_matrix( mat->rows, mat->cols );

        for ( int i = 0; i < mat->rows; i++ )
        {
                for ( int j = 0; j < mat->cols; j++ )
                {
                        result->data[ i ][ j ] = mat->data[ i ][ j ];
                }
        }

        return result;
}

void
free_matrix( matrix_t* mat )
{
        clear_matrix( mat );

        for ( int i = 0; i < mat->rows; i++ )
        {
                free( mat->data[ i ] );
        }

        free( mat );
}
