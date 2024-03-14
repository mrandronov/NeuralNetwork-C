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

        fill_matrix( net->weights[ 0 ], RANDOM );
        fill_matrix( net->weights[ 1 ], RANDOM );
        fill_matrix( net->weights[ 2 ], RANDOM );

        printf("Network weights initialized successfully!\n");

        net->biases = ( matrix_t** ) malloc( ( net->num_layers-1 ) * sizeof( matrix_t* ) );
        net->biases[ 0 ] = init_matrix( 16, 1 );
        net->biases[ 1 ] = init_matrix( 16, 1 );
        net->biases[ 2 ] = init_matrix( 10, 1 );

        fill_matrix( net->biases[ 0 ], RANDOM );
        fill_matrix( net->biases[ 1 ], RANDOM );
        fill_matrix( net->biases[ 2 ], RANDOM );

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

        net->weights_cost = ( matrix_t** ) malloc( net->num_layers * sizeof( matrix_t* ) ); 
        net->weights_cost[ 0 ] = init_matrix( 16, 784 );
        net->weights_cost[ 1 ] = init_matrix( 16, 16 );
        net->weights_cost[ 2 ] = init_matrix( 10, 16 );

        printf("Network weights cost initialized successfully!\n");

        net->biases_cost = ( matrix_t** ) malloc( net->num_layers * sizeof( matrix_t* ) ); 
        net->biases_cost[ 0 ] = init_matrix( 16, 1 );
        net->biases_cost[ 1 ] = init_matrix( 16, 1 );
        net->biases_cost[ 2 ] = init_matrix( 10, 1 );

        printf("Network biases cost initialized successfully!\n");

        printf("Network intialization successful!\n");

        return net;
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

/* DEBUG FUNCTION */
void
print_matrix_size( matrix_t* m )
{
        printf( "rows: %d cols: %d\n", m->rows, m->cols );
}

int
feed_forward( neural_network_t* net )
{
        for (int i = 0; i < net->num_layers - 1; i++ ) 
        {
                /*
                        Calculate the "z" matrix, i.e, layer before applying
                        the activation function.
                 */
                /* printf( "net->weights[ %d ] ", i ); */
                /* print_matrix_size( net->weights[ i ] ); */
                /* print_matrix( net->weights[ i ] ); */

                /* printf( "net->layers[ %d ] ", i ); */
                /* print_matrix_size( net->layers[ i ] ); */
                /* print_matrix( net->layers[ i ] ); */

                /* printf( "net->z[ %d ] ", i ); */
                /* print_matrix_size( net->z[ i ] ); */

                /* printf( "net->biases[ %d ]\n", i ); */
                /* print_matrix_size( net->biases[ i ] ); */
                /* print_matrix( net->biases[ i ] ); */

                net->z[ i ] = dot_product( net->weights[ i ], net->layers[ i ] );

                /* printf( "net->z[ %d ] before biases\n", i ); */
                /* print_matrix( net->z[ i ] ); */

                net->z[ i ] = add_matrix( net->biases[ i ], net->z[ i ] );

                /* printf( "net->z[ %d ]\n", i ); */
                /* print_matrix( net->z [ i ] ); */

                net->layers[ i + 1 ] = activation( net->z[ i ] );

                /* printf( "net->layers[ %d + 1 ]\n", i ); */
                /* print_matrix( net->layers[ i + 1 ] ); */
        }

        return 0;
}

void
back_propagate( neural_network_t* net, matrix_t* y )
{
        matrix_t**              z = net->z;
        matrix_t**              a = net->layers;
        matrix_t**              weights = net->weights;
        matrix_t**              layers_cost = net->layers_cost;
        int                     n = net->num_layers;

        matrix_t*               cost_diff = subtract_matrix( a[ n - 1], y );

        cost_diff = hadamard_product( scalar_multiply( cost_diff, 2 ),
                        derivative_activation( z[ n - 2 ] ) );

        /* Find initial weight cost. */

        matrix_t*               layers_transpose = transpose( a[ n - 2 ] );
        matrix_t*               new_weight_cost = dot_product( cost_diff, layers_transpose );

        net->weights_cost[ n - 2 ] = add_matrix( new_weight_cost, net->weights_cost[ n - 2 ] );

        /* Find initial bias cost. */

        net->biases_cost[ n - 2 ] = add_matrix( cost_diff, net->biases_cost[ n - 2 ] );

        /* Find initial layer cost ( cost for L - 1 ). */

        matrix_t*               weight_transpose = transpose( weights[ n - 2 ] );
        layers_cost[ n - 3 ] = dot_product( weight_transpose, cost_diff );

        matrix_free( new_weight_cost );
        matrix_free( layers_transpose );
        matrix_free( weight_transpose );
        matrix_free( cost_diff );

        /*
                Find the cost for the remaining layers, weights, and biases.
         */

        for ( int i = n - 3; i > 0; i-- )
        {
                /* Find weight cost. */

                matrix_t*               layers_cost_diff = subtract_matrix( a[ i + 1 ], layers_cost[ i ] );
                matrix_t*               layers_scaled = scalar_multiply( layers_cost_diff, 2 );

                layers_cost_diff = hadamard_product( layers_scaled, derivative_activation( z[ i ] ) );

                matrix_t*               layers_transpose = transpose( a[ i - 1 ] );
                matrix_t*               partial_weight_cost = dot_product( layers_cost_diff, layers_transpose );
                matrix_t*               new_weight_cost = add_matrix( partial_weight_cost, net->weights_cost[ i - 1 ] );

                matrix_free( net->weights_cost[ i - 1 ] );
                net->weights_cost[ i - 1 ] = new_weight_cost;

                /* Find bias cost. */

                matrix_t*               new_biases_cost = add_matrix( layers_cost_diff, net->biases_cost[ i - 1 ] );

                matrix_free( net->biases_cost[ i - 1 ] );
                net->biases_cost[ i - 1 ] = new_biases_cost;

                /* Find layer cost. */

                matrix_t*               weights_transpose = transpose( weights[ i ] );
                matrix_t*               new_layers_cost = dot_product( weights_transpose, layers_cost_diff );

                matrix_free( layers_cost[ i ] );
                layers_cost[ i ] = new_layers_cost; 

                matrix_free( layers_cost_diff );
                matrix_free( new_weight_cost );
                matrix_free( layers_scaled );
                matrix_free( layers_transpose );
                matrix_free( partial_weight_cost );
                matrix_free( weights_transpose );
        }
}

void
average_cost( neural_network_t* net, int batch_size )
{
        double                  avg_factor = ( double ) 1.0 / ( double ) batch_size;

        /* printf( "Huhhh...?\n" ); */
        /* print_matrix( net->biases[ 1 ] ); */
        for ( int i = 0; i < net->num_layers-1; i++ )
        {
                net->layers_cost[ i ] = scalar_multiply( net->layers_cost[ i ], avg_factor );
                net->weights_cost[ i ] = scalar_multiply( net->weights_cost[ i ], avg_factor );
                net->biases_cost[ i ] = scalar_multiply( net->biases_cost[ i ], avg_factor );
                /* printf( "net->biases_cost[ %d ]\n", i ); */
                /* print_matrix( net->biases_cost[ i ] ); */
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

void
adjust_weights( neural_network_t* net )
{
        matrix_t**              w = net->weights;
        matrix_t**              weights_cost = net->weights_cost;

        for ( int i = net->num_layers - 2; i >= 0; i-- )
        {
                w[ i ] = subtract_matrix( w[ i ], weights_cost[ i ] );
        }
}

void
adjust_biases( neural_network_t* net )
{
        matrix_t**              b = net->biases;
        matrix_t**              biases_cost = net->biases_cost;

        for ( int i = net->num_layers-2; i >= 0; i-- )
        {
                b[ i ] = subtract_matrix( b[ i ], biases_cost[ i ] );
        }
}

void
clear_costs( neural_network_t* net )
{
        for ( int i = 0; i < net->num_layers - 2; i++ )
        {
                clear_matrix( net->layers_cost[ i ] );
                clear_matrix( net->weights_cost[ i ] );
        }
}

int
network_output( neural_network_t* net )
{
        int                 max = 0;
        matrix_t*      output_layer = net->layers[ net->num_layers - 1 ];

        /* printf( "output_layer\n" ); */
        /* print_matrix( output_layer ); */
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
net_free( neural_network_t* net )
{
        for ( int i = 0; i < net->num_layers - 1; i++ )
        {
                matrix_free( net->layers[ i ] );
                matrix_free( net->weights[ i ] );
                matrix_free( net->biases[ i ] );
                matrix_free( net->z[ i ] );
                matrix_free( net->layers_cost[ i ] );
        }
        matrix_free( net->layers[ net->num_layers - 1 ] );

        free( net );
}

