#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "data_set.h"

data_set_t*
init_data( char* file_name, int size )
{
    data_set_t*                 data = ( data_set_t* ) calloc( 1, sizeof( data_set_t ) );
    
    data->file_name = file_name;
    data->fp = fopen( file_name, "r" );

    if ( data->fp == NULL )
    {
        printf( "Error while opening file %s: %s!\n", file_name, strerror(errno) );
        exit( 1 );
    }

    data->set_size = size;
    printf( "Successfully opened %s\n", file_name );

    return data;
}

int
read_number( data_set_t* data, neural_network_t* net )
{
    ssize_t             read;
    size_t              len = 0;
    char*               line = NULL;
    
    read = getline( &line, &len, data->fp );
    if ( read == -1 )
    {
        printf( "WARNING: error occured or EOF reached!\n" );
        return 1;
    }

    data->last_number = line[ 0 ] - '0';

    // Parse one line of data
    int                 c = 0;
    int                 num = 0;
    for ( int i = 2; line[ i ] != '\n'; i++ )
    {
        if ( line[ i ] != ',' )
        {
            num *= 10;
            num += line[ i ] - '0';
        }
        else
        {
            net->layers[ 0 ]->data[ c ][ 0 ] = ( double )( ( double ) num / ( double ) 255 );
            num = 0;
            c++;
        }
    }

    return 0;
}

void
free_data( data_set_t* data )
{
    free(data);
}

