#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

typedef struct
{
        int                     num_layers;
        int                     batch_size;
        int                     num_correct;
        int                     num_answered;
        double                  eta;
        double                  accuracy;
        matrix_t**              layers;
        matrix_t**              weights;
        matrix_t**              biases;
        matrix_t**              z;
        matrix_t**              layers_cost;
        matrix_t**              weights_cost;
        matrix_t**              biases_cost;
} neural_network_t;

neural_network_t*
init_net();

int
fill_net( neural_network_t* net );

void
peak_layer(neural_network_t* net, int layer );

void
show_layer( neural_network_t* net, int layer );

int
feed_forward( neural_network_t* net );

void
back_propagate( neural_network_t* net, matrix_t* y );

void
adjust_weights( neural_network_t* net );

void
adjust_biases( neural_network_t* net );

void
clear_costs( neural_network_t* net );

void
average_cost( neural_network_t* net, int batch_size );

double
get_cost( neural_network_t* net, matrix_t* y );

int
network_output( neural_network_t* net );

void
update_stats( neural_network_t* net, int answer );

void
clear_stats( neural_network_t* net );

void
print_stats( neural_network_t* net );

void
free_net( neural_network_t* net );

#endif /* __NEURAL_NETWORK_H__ */
