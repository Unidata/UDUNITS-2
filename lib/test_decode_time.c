#include <stdio.h>
#include "udunits2.h"
#include <stdlib.h>

#define BUFLEN 1024

int main( int argc, char *argv[] )
{
    int     year, month, day, hour, minute;
    double  second, resolution;
    double  tval;
    ut_system *unitSystem;
    ut_unit   *utu1;

    /* Initialize unit system */
    unitSystem = ut_read_xml(NULL);
    if( unitSystem == NULL ) {
        fprintf( stderr, "error initializing unit system\n" );
        exit(-1);
    }

    /* Decode a time value of 0, print its date */
    tval = 0.0;
    ut_decode_time( tval, &year, &month, &day, &hour, &minute,
        &second, &resolution );
    /*printf( "decoding time location A: tval=%lf  %d/%d/%d %d:%d:%06.3lf\n",
        tval, year, month, day, hour, minute, second );*/
    printf( "decoding time location A: tval=%f  %d/%d/%d %d:%d:%06.3f\n",
        tval, year, month, day, hour, minute, second );

    /* Repeat just to make sure */
    ut_decode_time( tval, &year, &month, &day, &hour, &minute,
        &second, &resolution );
    /*printf( "decoding time location B: tval=%lf  %d/%d/%d %d:%d:%06.3lf\n",
        tval, year, month, day, hour, minute, second );*/
    printf( "decoding time location B: tval=%f  %d/%d/%d %d:%d:%06.3f\n",
        tval, year, month, day, hour, minute, second );

    /* Parse a timestamp string */
    utu1 = ut_parse( unitSystem, "days since 2010-01-08 11:44", UT_ASCII );
    if( utu1 == NULL ) {
        fprintf( stderr, "Error parsing unit string with current date\n" );
        exit(-1);
    }

    /* Repeat decoding a time value of 0, print its date */
    tval = 0.0;
    ut_decode_time( tval, &year, &month, &day, &hour, &minute, &second,
        &resolution );
    /*printf( "decoding time location B: tval=%lf  %d/%d/%d %d:%d %lf\n",
            tval, year, month, day, hour, minute, second );*/
    printf( "decoding time location C: tval=%f  %d/%d/%d %d:%d:%06.3f\n",
            tval, year, month, day, hour, minute, second );

    return(0);
}
