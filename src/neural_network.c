#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "neural_network.h"
#include "matrix.h"

// Hard code neural network size for now
neural_network_t*
init_net( )
{
        neural_network_t*      net = ( neural_network_t* ) malloc( sizeof( neural_network_t ) );

        net->num_layers = 4;
        // Hard coded batch for now
        net->batch_size = 1000;
        net->eta = 3.0;
        net->accuracy = 0.0;
        net->num_correct = 0;
        net->num_answered = 0;

        net->layers = ( matrix_t** ) malloc( net->num_layers * sizeof( matrix_t* ) );

        net->layers[ 0 ] = init_matrix( 784, 1 );
        net->layers[ 1 ] = init_matrix( 16, 1 );
        net->layers[ 2 ] = init_matrix( 16, 1 );
        net->layers[ 3 ] = init_matrix( 10, 1 );

        printf("Network layers initialized successfully!\n");

        net->weights = ( matrix_t** ) malloc( ( net->num_layers - 1 ) * sizeof( matrix_t* ) );
        net->weights[ 0 ] = init_matrix( 16, 784 );
        net->weights[ 1 ] = init_matrix( 16, 16 );
        net->weights[ 2 ] = init_matrix( 10, 16 );

        printf("Network weights initialized successfully!\n");

        net->biases = ( matrix_t** ) malloc( ( net->num_layers-1 ) * sizeof( matrix_t* ) );
        net->biases[ 0 ] = init_matrix( 16, 1 );
        net->biases[ 1 ] = init_matrix( 16, 1 );
        net->biases[ 2 ] = init_matrix( 10, 1 );

        printf("Network biases initialized successfully!\n");

        net->z = ( matrix_t** ) malloc( net->num_layers * sizeof( matrix_t* ) );
        net->z[ 0 ] = init_matrix( 16, 1 );
        net->z[ 1 ] = init_matrix( 16, 1 );
        net->z[ 2 ] = init_matrix( 10, 1 );

        printf("Network z matrix initialized successfully!\n");

        net->layers_cost = ( matrix_t** ) malloc( net->num_layers * sizeof( matrix_t* ) ); 
        net->layers_cost[ 0 ] = init_matrix( 16, 1 );
        net->layers_cost[ 1 ] = init_matrix( 16, 1 );
        net->layers_cost[ 2 ] = init_matrix( 10, 1 );

        printf("Network layers cost initialized successfully!\n");

        printf("Network intialization successful!\n");

        return net;
}

int
fill_net( neural_network_t* net )
{
        for (int i = 0; i < net->num_layers-1; i++ )
        {
                fill_matrix( net->weights[ i ] );
                fill_matrix( net->biases[ i ] );
        }

        return 0;
}

void
peak_layer( neural_network_t* net, int layer )
{
        int         rows;
        int         cols;
        int         abbreviated_rows = 0;
        int         abbreviated_cols = 0;

        rows = net->layers[ layer ]->rows;
        cols = net->layers[ layer ]->cols;

        printf( "Printing rows: %d cols: %d\n", rows, cols ); 

        if ( rows > 10 )
        {
                rows = 10;
                abbreviated_rows = 1;

                printf( "Abbreviated %d rows...\n", net->layers[ layer ]->rows - 10 );
        }

        if ( cols > 10 )
        {
                cols = 10;
                abbreviated_cols = 1;

                printf( "Abbreviated %d columns...\n", net->layers[ layer ]->cols - 10 );
        }

        for ( int i = 0; i < rows; i++ )
        {
                for ( int j = 0; j < cols; j++ )
                {
                        printf( "%f ", net->layers[ layer ]->data[ i ][ j ] );
                }

                if ( abbreviated_cols )
                {
                        printf( "..." );
                }

                printf( "\n" );
        }

        if ( abbreviated_rows )
        {
                printf( "...\n" );
        }
}

void
show_layer( neural_network_t* net, int layer )
{
        for ( int i = 0; i < net->layers[ layer ]->rows; i++ )
        {
                for ( int j = 0; j < net->layers[ layer ]->cols; j++ )
                {
                        printf( "%f ", net->layers[ layer ]->data[ i ][ j ] );
                }
                printf( "\n" );
        }
}

matrix_t*
calculate_z( matrix_t* weights,
        matrix_t* biases,
        matrix_t* layer )
{
        return add_matrix( dot_product( weights, layer ), biases );
}

int
feed_forward( neural_network_t* net )
{
        for (int i = 0; i < net->num_layers - 1; i++ ) 
        {
                net->z[ i ] = add_matrix( 
                                dot_product( net->weights[ i ], net->layers[ i ] ), 
                                net->biases[ i ] );

                net->layers[ i + 1 ] = activation( net->z[ i ] );
        }

        return 0;
}

matrix_t*
find_output_cost( neural_network_t* net, matrix_t* y )
{
        int                     num_layers = net->num_layers;
        matrix_t*               z = net->z[ num_layers - 2 ];
        matrix_t*               a = net->layers[ num_layers - 1 ];

        matrix_t*               temp = hadamard_product( 
                                                subtract_matrix( a, y ), 
                                                derivative_activation( z ) );

        return add_matrix( temp, net->layers_cost[ num_layers - 2 ] );
}

void
back_propagate( neural_network_t* net, matrix_t* y )
{
        matrix_t**              z = net->z;
        matrix_t**              weights = net->weights;
        matrix_t**              layers_cost = net->layers_cost;
        int                     num_layers = net->num_layers;

        //printf( "beginning back propagation...\n" );
        
        net->layers_cost[ net->num_layers - 2 ] = find_output_cost( net, y );

        for ( int i = num_layers-3; i >= 0; i-- )
        {
                matrix_t*               weights_transpose = transpose( weights[ i + 1 ] );
                matrix_t*               delta = dot_product( weights_transpose, layers_cost[ i + 1 ] );
                matrix_t*               sigmoid_prime = derivative_activation( z[ i ] );

                layers_cost[ i ] = add_matrix( hadamard_product( delta, sigmoid_prime ), layers_cost[ i ] );

                clear_matrix( z[ i ] );

                free_matrix( delta );
                free_matrix( sigmoid_prime );
                free_matrix( weights_transpose );
        }

}

void
average_cost( neural_network_t* net, int batch_size )
{
        double                  avg_factor = ( double ) 1.0 / ( double ) batch_size;

        for ( int i = 0; i < net->num_layers-1; i++ )
        {
                net->layers_cost[ i ] = scalar_multiply( net->layers_cost[ i ], avg_factor );
        }
}

double
get_cost( neural_network_t* net, matrix_t* y )
{
        double                  result = 0.0f;

        for ( int i = 0; i < net->layers[ net->num_layers - 1 ]->rows; i++ )
        {
                double value = net->layers[ net->num_layers - 1 ]->data[ i ][ 0 ] - y->data[ i ][ 0 ];
                result += ( value * value );
        }

        return result;
}

matrix_t*
calc_weight_gradient( matrix_t* layers, matrix_t* layers_cost )
{
        return dot_product( layers_cost, transpose( layers ) );
}

matrix_t*
calc_bias_gradient( matrix_t* layers_cost )
{
        matrix_t*               result = init_matrix( layers_cost->rows, 1 );

        for ( int i = 0; i < result->rows; i++ )
        {
                result->data[ i ][ 0 ] = layers_cost->data[ i ][ 0 ];
        }

        return result;
}

void
adjust_weights( neural_network_t* net )
{
        matrix_t**              w = net->weights;
        matrix_t**              layers = net->layers;
        matrix_t**              layers_cost = net->layers_cost;
        int                     num_layers = net->num_layers;

        for ( int i = num_layers - 2; i >= 0; i-- )
        {
                matrix_t*          weight_gradient = calc_weight_gradient( layers[ i ], layers_cost[ i ] );

                w[ i ] = subtract_matrix( w[ i ], weight_gradient );

                free_matrix( weight_gradient );
        }
}

void
adjust_biases( neural_network_t* net )
{
        int                     num_layers = net->num_layers;
        matrix_t**              b = net->biases;
        matrix_t**              layers_cost = net->layers_cost;

        for ( int i = num_layers-2; i >= 0; i-- )
        {
                b[ i ] = subtract_matrix( b[ i ], layers_cost[ i ] );
        }
}

void
clear_costs( neural_network_t* net )
{
    clear_matrix( net->layers_cost[ 0 ] );
    clear_matrix( net->layers_cost[ 1 ] );
    clear_matrix( net->layers_cost[ 2 ] );
}

int
network_output( neural_network_t* net )
{
        int                 max = 0;
        matrix_t*      output_layer = net->layers[ net->num_layers - 1 ];

        for ( int i = 0; i < output_layer->rows; i++ )
        {
                if ( output_layer->data[ i ][ 0 ] >= output_layer->data[ max ][ 0 ] )
                {
                        max = i;
                }
        }

        return max;
}

void
update_stats( neural_network_t* net, int answer )
{
        if ( network_output( net ) == answer )
        {
                net->num_correct++;
        }

        net->num_answered++;
        net->accuracy = ( double ) net->num_correct / ( double ) net->num_answered;
}

void
clear_stats( neural_network_t* net )
{
        net->num_correct = 0;
        net->num_answered = 0;
        net->accuracy = 0;
}

void
print_stats( neural_network_t* net )
{
        printf( "Network accuracy was %%%f\n", ( net->accuracy*100.0 ) );
        printf( "num_correct = %d\n", net->num_correct );
        printf( "num_answered = %d\n", net->num_answered );
}

void
free_net( neural_network_t* net )
{
        free( net->weights );
        free( net->biases );
        free( net->z );
        free( net->layers );
        free( net->layers_cost );
        free( net );
}

