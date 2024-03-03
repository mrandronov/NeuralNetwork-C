#ifndef __DATA_SET_H__
#define __DATA_SET_H__

#include <stdio.h>
#include <stdlib.h>

#include "neural_network.h"

typedef struct
{
    FILE*       fp;
    char*       file_name;
    int         set_size;
    int         last_number;
} data_set_t;

data_set_t*
init_data( char* file_name, int size );

int
read_number( data_set_t* data, neural_network_t* net );

void
free_data( data_set_t* data );

#endif /* __DATA_SET_H__ */
