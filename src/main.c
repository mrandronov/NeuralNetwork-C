#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "data_set.h"
#include "neural_network.h"
#include "matrix.h"

void
train(neural_network_t* net, data_set_t* training_data)
{
        double                  cost = 0.0f;
        int                     ret = 0;
        int                     m = 10;
        int                     n = training_data->set_size;
        int                     batch_size = n / m;
        matrix_t*               y = init_matrix( net->layers[ net->num_layers - 1 ]->rows, 1 );

        printf( "m: %d\n", m );
        printf( "Batch size: %d\n", batch_size );
        printf( "=====================================\n" );

        for ( int i = 0; i < 10; i++ )
        {
                for ( int j = 0; j < batch_size; j++ )
                {
                        ret = read_number( training_data, net );

                        if ( ret )
                        {
                                break;
                        }

                        feed_forward( net );

                        y->data[ training_data->last_number ][ 0 ] = 1.0f;

                        cost = get_cost( net, y );

                        back_propagate( net, y );

                        update_stats( net, training_data->last_number );

                        y->data[ training_data->last_number ][ 0 ] = 0.0f;
                }

                printf( "Batch #%d | Cost = %f\n", i+1, ( cost / batch_size ) );
                cost = 0.0;

                average_cost( net, batch_size );

                adjust_weights( net );
                adjust_biases( net );
                clear_costs( net );
        }

        printf( "=====================================\n" );
        print_stats( net );
        matrix_free( y );
}

void
test( neural_network_t* net, data_set_t* test_data )
{
        int     ret;

        for ( int i = 0; i < test_data->set_size; i++ )
        {
                ret = read_number( test_data, net );

                if ( ret )
                {
                        break;
                }

                feed_forward( net );

                update_stats( net, test_data->last_number );
        }
}

int
main( int argc, char** argv )
{

        neural_network_t*         net1 = init_net();
        data_set_t*               test_data = init_data( "data/mnist_test.csv", 10000 );
        data_set_t*               training_data = init_data( "data/mnist_train.csv", 60000 );

        printf("All structures have been initialized!\n");

        printf( "Training network...\n" );

        train( net1, training_data );

        printf( "Training completed!\n" );


        clear_stats( net1 );


        printf( "Testing network...\n" );

        test( net1, test_data );

        printf( "Testing completed!\n" );


        print_stats( net1 );

        net_free( net1 );
        data_set_free( test_data );
        data_set_free( training_data );

        return 0;

}
