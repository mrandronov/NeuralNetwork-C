#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "data_set.h"

data_set_t*
init_data( char* file_name, int size )
{
        data_set_t*             data = ( data_set_t* ) calloc( 1, sizeof( data_set_t ) );

        data->file_name = file_name;
        data->fp = fopen( file_name, "r" );

        if ( data->fp == NULL )
        {
                printf( "Error while opening file %s: %s!\n", file_name, strerror( errno ) );
                exit( 1 );
        }

        data->set_size = size;
        data->read_count = 0;

        printf( "Successfully opened %s\n", file_name );

        return data;
}

int
read_number( data_set_t* data, neural_network_t* net )
{
        ssize_t                 read;
        size_t                  len = 0;
        char*                   line = NULL;

        if ( data->read_count >= data->set_size )
        {
                printf( "WARNING: end of data has already been reached!\n" );
                printf( "Free this object and initialize a new data set!\n" );
                return 1;
        }

        read = getline( &line, &len, data->fp );
        data->read_count++;
        if ( read == -1 )
        {
                printf( "WARNING: error occured while reading line from file stream: %s\n", strerror( errno ) );
                return 1;
        }

        /*
                The data is in the format a line with comma separated values.
                I use strtok to tokenize the values, getting everything between
                the commas.

                The first number is the digit that the image data represents.

                The image itself is represented with pixel brightness values
                between 0 and 255. The network works with values between 0 and
                1, so I average the brightness values to fit within that range.
         */

        int                     c = 0;
        char*                   token = strtok( line, "," );

        data->last_number = atoi(token);
        token = strtok( NULL, "," );

        while ( token && c < 784 )
        {
                /* printf( "token: %s %d %f\n", token, atoi(token), (double)(atoi(token)/ 255.0f) ); */
                net->layers[ 0 ]->data[ c ][ 0 ] = ( double ) atoi(token) / 255.0f;
                token = strtok( NULL, "," );
                c++;
        }

        return 0;
}

void
data_set_free( data_set_t* data )
{
        if ( fclose( data->fp ) != 0 )
        {
                printf( "Error while closing file stream: %s\n", strerror( errno ) );
        }

        free( data );
}

