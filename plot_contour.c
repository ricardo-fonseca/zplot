#include "plot_contour.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "plot_data.h"
#include "plot_contour.h"
#include "plot_polygon.h"

enum direction {LEFT, RIGHT, DOWN, UP};

const char contourTypeID[] = "t_plot_contour";

void draw_contour_data( t_plot_data *data, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ) {
    if ( data -> typeID == contourTypeID ) {
        t_plot_contour* contour = (t_plot_contour*) data -> obj;
        draw_contour( contour, area, axis, cr );
    } else {
        fprintf(stderr,"(*error*) draw_contour_data called without a t_plot_contour object.\n");
        exit(-1);
    }
}

void add_contour( t_plot_contour* contour, t_plot_data_list* list) {
    t_plot_data item;

    item.obj = (void *) contour;
    item.typeID = (char *) contourTypeID;
    item.draw = & draw_contour_data;
    item.cleanup = NULL;

    add_data( list, &item );
}


void init_contour( t_plot_contour* contour ){
    contour -> data = NULL;
    contour -> xdim = 0;
    contour -> ydim = 0;

    contour -> x0 = contour -> x1 = 0;
    contour -> y0 = contour -> y1 = 0;

    contour -> lineStyle = SOLID;
    contour -> lineThick = 0.003;
    contour -> lineColor = (t_color) {.r=0,.g=0,.b=0,.a=1};

    contour -> values = NULL;
    contour -> nvalues = 0;
}

void start_contour( double const val, double const * const data, uint8_t * const lut, int const stride,
    int i0, int j0, int * restrict dir, double * restrict x, double * restrict y )
{

    const int idx = i0 + j0 * stride;

    // Initial point
    switch (lut[idx])
    {
    case 1: // '\'
    case 3: // '-'
    case 5: // '//'
    case 7: // '/'
    case 8: // '\\'
        *x = i0;
        *y = j0 + (val - data[idx])/(data[idx+stride] - data[idx]);
        *dir = RIGHT;
        break;

    case 2: // '/'
    case 6: // '|'
        *x = i0 + (val - data[idx])/(data[idx+1] - data[idx]);
        *y = j0;
        *dir = UP;
        break;

    case 4:
        *x = i0 + (val - data[idx+stride])/(data[idx+stride+1] - data[idx+stride]);
        *y = j0 + 1.0;
        *dir = DOWN;
        break;
    }
}

void step_contour( double const val, double const * const data, uint8_t * lut, int const stride,
    int *i_, int *j_, int *dir_, double *x, double *y )
{
    int i = *i_;
    int j = *j_;

    const int idx = i + j * stride;

    // Find motion direction and clear LUT at current cell
    int t = lut[ idx ];
    int dir = *dir_;

    switch (t)
    {
    case 1: 
        t = 0;
        switch(dir) {
        case RIGHT:
            dir = DOWN;
            break;
        case UP:
            dir = LEFT;
            break;
        }
        break;
    case 2: 
        t = 0;
        switch(dir) {
        case UP:
            dir = RIGHT;
            break;
        case LEFT:
            dir = DOWN;
            break;
        }
        break;
    case 3: // '-'
        t = 0;
        switch(dir) {
        case LEFT:
            dir = LEFT;
            break;
        case RIGHT:
            dir = RIGHT;
            break;
        }
        break;
    case 4: // '\'
        t = 0;
        switch(dir) {
        case DOWN:
            dir = RIGHT;
            break;
        case LEFT:
            dir = UP;
            break;
        }
        break;
    case 5: // '//'
        switch(dir) {
        case LEFT:
            dir = UP;
            t = 2;
            break;
        case DOWN:
            dir = LEFT;
            t = 2;
            break;
        case RIGHT:
            dir = DOWN;
            t = 7;
            break;
        case UP:
            dir = RIGHT;
            t = 7;
            break;
        }
        break;
    case 6: // '|'
        t = 0;
        switch(dir) {
        case DOWN:
            dir = DOWN;
            break;
        case UP:
            dir = UP;
            break;
        }
        break;
    case 7: // '/'
        t = 0;
        switch(dir) {
        case RIGHT:
            dir = UP;
            break;
        case DOWN:
            dir = LEFT;
            break;
        }
        break;
    case 8: // '\\'
        switch(dir) {
        case LEFT:
            dir = UP;
            t = 1;
            break;
        case DOWN:
            dir = RIGHT;
            t = 1;
            break;
        case RIGHT:
            dir = DOWN;
            t = 4;
            break;
        case UP:
            dir = LEFT;
            t = 4;
            break;
        }
        break;
    }

    // Store new LUT value and motion direction
    lut[ idx ] = t;
    *dir_ = dir;

    // Move to next cell and get new point coordinates in grid units
    switch (dir)
    {
    case LEFT:
        *x = i;
        *y = j + (val - data[idx])/(data[idx+stride] - data[idx]);
        i = i - 1;
        break;
    case RIGHT:
        *x = i + 1.0;
        *y = j + (val - data[idx+1])/(data[idx+stride+1] - data[idx+1]);
        i = i + 1;
        break;
    case DOWN:
        *x = i + (val - data[idx])/(data[idx+1] - data[idx]);
        *y = j;
        j = j - 1;
        break;
    case UP:
        *x = i + (val - data[idx+stride])/(data[idx+stride+1] - data[idx+stride]);
        *y = j + 1.0;
        j = j + 1;
        break;                        
    }

    *i_ = i;
    *j_ = j;
}

void draw_contour( t_plot_contour *contour, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ){

    // 
    double const x0 = contour -> x0;
    double const y0 = contour -> y0;

    double const dx = (contour -> x1 - contour -> x0) / contour -> xdim;
    double const dy = (contour -> y1 - contour -> y0) / contour -> ydim;

    // Create cairo image
    cairo_save( cr );

    double const sx = area -> x1 - area -> x0;
    double const sy = area -> y1 - area -> y0;

    // Clip to plot region
    cairo_rectangle( cr, area->x0, area->y0, sx, sy );
    cairo_clip(cr);

    double * restrict data = contour->data;
    uint8_t * restrict mask = (uint8_t *) malloc( (contour -> xdim)*(contour -> ydim) );
    uint8_t * restrict lut  = (uint8_t *) malloc( (contour -> xdim)*(contour -> ydim) );
    int const stride = contour -> xdim;

    t_plot_polygon poly;
    init_polygon(&poly);

    t_pointlist listA, listB;
    init_pointlist( &listA );
    init_pointlist( &listB );


    for( int k = 0; k < contour -> nvalues; k ++) {
        
        // Set mask array
        double const value = contour -> values[k];

        for (int i = 0; i < (contour -> xdim)*(contour -> ydim); i++ )
            mask[i] = (data[i]<value)?0:1;

        // Set look-up-table array
        for (int j = 0; j < (contour -> ydim-1); j++) {
            for (int i = 0; i < (contour -> xdim-1); i++ ) {
                uint8_t t = 
                     mask[ (i  ) + (j  )*stride ] + 
                    (mask[ (i+1) + (j  )*stride ] << 1)+
                    (mask[ (i+1) + (j+1)*stride ] << 2)+
                    (mask[ (i  ) + (j+1)*stride ] << 3);
                
                // Simplify mask and disambiguate saddle points
                switch (t) {
                    case 5: {
                        double center = 0.25 * (
                            data[ (i  ) + (j  )*stride ] +
                            data[ (i+1) + (j  )*stride ] +
                            data[ (i+1) + (j+1)*stride ] +
                            data[ (i  ) + (j+1)*stride ] );
                        if ( center < value ) t = 8;
                        break;
                    }
                    case 10: {
                        double center = 0.25 * (
                            data[ (i  ) + (j  )*stride ] +
                            data[ (i+1) + (j  )*stride ] +
                            data[ (i+1) + (j+1)*stride ] +
                            data[ (i  ) + (j+1)*stride ] );
                        if ( center > value ) t = 5;
                        else t = 8;
                        break;
                    }
                    default:
                        if ( t >= 8 ) t = 15 - t;
                }

                lut[ i + j * stride ] = t;
            }
        }
        
        // Create paths
        for (int j0 = 0; j0 < (contour -> ydim-1); j0++) {
            for (int i0 = 0; i0 < (contour -> xdim-1); i0++ ) {
                uint8_t t = lut[ i0 + j0 * stride ];
                if ( t > 0 ) {
                    // Reset lists (this does not free memory)
                    listA.np = 0;
                    listB.np = 0;

                    // Get initial point
                    double x, y;
                    int dir0;
                    start_contour( value, data, lut, stride, i0, j0, &dir0, &x, &y );
                    add_pointlist( &listA, x, y );

                    int i = i0; int j = j0; int dir = dir0;
                    int finished = 0;

                    while( !finished ) {
                        double x, y;
                        step_contour( value, data, lut, stride, 
                            &i, &j, &dir, &x, &y );

                        add_pointlist( &listA, x, y );

                        if ( i == i0 && j == j0 ) {
                            finished = 1;
                            poly.closed = 1;
                        } else if ( i<0 || i >= (contour -> xdim-1) || 
                                    j<0 || j >= (contour -> ydim-1) ) {
                            finished = 2;
                            poly.closed = 0;
                        }
                    }

                    // Not a closed path - follow contour on opposite direction
                    if ( finished == 2 ) {
                        
                        switch (dir0)
                        {
                        case LEFT:
                            dir = RIGHT;
                            i = i0 + 1;
                            j = j0;
                            break;
                        case RIGHT:
                            dir = LEFT;
                            i = i0 - 1;
                            j = j0;
                            break;
                        case DOWN:
                            dir = UP;
                            i = i0;
                            j = j0 + 1;
                            break;
                        case UP:
                            dir = DOWN;
                            i = i0;
                            j = j0 - 1;
                            break;                        
                        }

                        while( i>=0 && i < (contour -> xdim-1) && 
                               j>=0 && j < (contour -> ydim-1) ) {
                            double x, y;
                            step_contour( value, data, lut, stride, 
                                &i, &j, &dir, &x, &y );

                            add_pointlist( &listB, x, y );
                        }

                        merge_pointlist( &listA, &listB, 1, 1 );
                    }

                    // Scale the points to the object dimensions
                    for( int k = 0; k < listA.np; k++ ) {
                        listA.points[k].x = (0.5 + listA.points[k].x) * dx + x0;
                        listA.points[k].y = (0.5 + listA.points[k].y) * dy + y0;
                    }

                    // Draw polygon
                    poly.pos = &listA;
                    draw_polygon( &poly, area, axis, cr );

                    // If original cell was a saddle point repeat cell
                    if ( lut[ i0 + j0 * stride ] > 0 ) i0--;

                }
            }
        }
    }

    cleanup_pointlist( &listA );
    cleanup_pointlist( &listB );

    cleanup_polygon( &poly );

    free(lut);
    free(mask);

    cairo_restore( cr );
}
