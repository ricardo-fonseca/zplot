#define BUFFER_SEGMENT 1024

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "points.h"

/**
 * Initialize pointlist object
 * @param list  pointlist object pointer
 */
void init_pointlist( t_pointlist * list ) {
    list -> points = NULL;
    list -> bufferSize = 0;
    list -> np = 0;
}

/**
 * Cleanup pointlist object, freeing allocated memory if any
 * @param list  pointlist object pointer
 */
void cleanup_pointlist( t_pointlist * list ) {
    if ( list -> points ) {
        free( list -> points );
        list -> points = NULL;
    }

    list -> bufferSize = 0;
    list -> np = 0;
}

/**
 * Adds point to pointlist object, growing buffer if needed
 * @param list  pointlist object pointer
 * @param x     x position
 * @param y     y position  
 */
void add_pointlist( t_pointlist * list, double x, double y ) {
    if ( list -> np == list -> bufferSize ) {
        list -> bufferSize += BUFFER_SEGMENT;
        list -> points = realloc( list -> points, list -> bufferSize * sizeof( t_point ) );
    }

    list -> points[ list -> np ].x = x;
    list -> points[ list -> np ].y = y;
    list -> np++;
}

/**
 * Adds point to pointlist object, growing buffer if needed
 * @param list  pointlist object pointer
 * @param point point data  
 */
void add_pointlist_point( t_pointlist * list, t_point point ) {
    if ( list -> np == list -> bufferSize ) {
        list -> bufferSize += BUFFER_SEGMENT;
        list -> points = realloc( list -> points, list -> bufferSize * sizeof( t_point ) );
    }

    list -> points[ list -> np ] = point;
    list -> np++;
}

/**
 * Merges two point lists. Result is returned in the first list
 * @param listA     First pointlist object
 * @param listB     Second pointlist object
 * @param prepend   Set to 1 to prepend listB, or 0 to append it
 * @param reverse   Set to 1 to reverse listB, or 1 to keep the order  
 */
void merge_pointlist( t_pointlist *restrict listA, const t_pointlist *restrict listB,
    int prepend, int reverse ) {

    if ( listB -> np > 0 ) {
        int total = listA -> np + listB -> np;
        int size = ((total + BUFFER_SEGMENT - 1)/BUFFER_SEGMENT) * BUFFER_SEGMENT;
        if ( size < listA -> bufferSize) size = listA -> bufferSize;
        t_point *points = (t_point *) malloc( size * sizeof(t_point));

        if ( prepend ) {
            if ( reverse ) {
                for( int i = 0; i < listB -> np; i++ ) {
                    points[ i ] = listB -> points[ listB -> np - i - 1];
                }
            } else {
                memcpy( points, listB -> points, listB -> np * sizeof(t_point));
            }
            memcpy( points + listB -> np, listA -> points, listA -> np * sizeof(t_point));
        } else {
            memcpy( points, listA -> points, listA -> np * sizeof(t_point));
            if ( reverse ) {
                for( int i = 0; i < listB -> np; i++ ) {
                    points[ listA -> np + i ] = listB -> points[ listB -> np - i - 1];
                }
            } else {
                memcpy( points + listA -> np, listB -> points, listB -> np * sizeof(t_point));
            }
        }

        free( listA -> points );
        listA -> points = points;
        listA -> np = total;
        listA -> bufferSize = size;

    }

}
