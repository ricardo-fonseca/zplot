#include "plot_polygon.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>

const char polygonTypeID[] = "t_plot_polygon";


void init_polygon( t_plot_polygon* polygon ) {
    polygon -> pos = NULL;
    polygon -> closed = 0;
    polygon -> style = 1;

    polygon -> lineThick = 0.002;
    polygon -> lineStyle = SOLID;
    polygon -> lineColor = (t_color) {.r=0,.g=0,.b=0,.a=1};
    polygon -> fillColor = (t_color) {.r=0,.g=0,.b=0,.a=1};
}

void cleanup_polygon( t_plot_polygon* polygon ) {
    if ( polygon -> pos ) cleanup_pointlist( polygon -> pos );
    init_polygon( polygon );
}


void draw_polygon( t_plot_polygon *polygon, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ) {

    double const sx = area -> x1 - area -> x0;
    double const sy = area -> y1 - area -> y0;

    if ( polygon -> pos -> np > 0 ) {

        cairo_set_line_width( cr, polygon -> lineThick );
        cairo_set_line_style( cr, polygon -> lineStyle );

        if ( polygon -> style > 0 ) {

            double x;
            double y;

            x = scale_axis( &axis[0], polygon -> pos -> points[0].x);
            y = scale_axis( &axis[1], polygon -> pos -> points[0].y);

            cairo_move_to( cr, area -> x0 + x*sx, area -> y1 - y*sy );
            for( int i = 1; i < polygon -> pos -> np; i ++ ) {
                x = scale_axis( &axis[0], polygon -> pos -> points[i].x);
                y = scale_axis( &axis[1], polygon -> pos -> points[i].y);
                cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy );
            }

            if ( polygon -> closed ) {
                cairo_close_path( cr );
            }
            
            switch ( polygon -> style ) {
            case 1:
                cairo_set_source_color( cr, &polygon-> lineColor);
                cairo_stroke(cr);
                break;
            case 2:
                cairo_set_source_color(cr, &polygon -> fillColor);
                cairo_fill(cr);
                break;
            case 3:
                cairo_set_source_color( cr, &polygon-> lineColor);
                cairo_stroke_preserve (cr); 
                cairo_set_source_color(cr, &polygon -> fillColor);
                cairo_fill(cr);
                break;
            }

        } else {
            for( int i = 0; i < polygon -> pos -> np; i ++ ) {
                double x = scale_axis( &axis[0], polygon -> pos -> points[i].x);
                double y = scale_axis( &axis[1], polygon -> pos -> points[i].y);
                cairo_move_to( cr, area -> x0 + x*sx, area -> y1 - y*sy );
                cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy );
            }
            cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
            cairo_set_source_color( cr, &polygon-> lineColor);
            cairo_stroke (cr);
        }

        // Clear dash
        cairo_set_line_style( cr, SOLID );
    }    
}

void draw_polygon_data( t_plot_data *data, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ) {
    if ( data -> typeID == polygonTypeID ) {
        t_plot_polygon* poly = (t_plot_polygon*) data -> obj;
        draw_polygon( poly, area, axis, cr );
    } else {
        fprintf(stderr,"(*error*) draw_polygon_data called without a t_plot_polygon object.\n");
        exit(-1);
    }
}

void add_polygon( t_plot_polygon* polygon, t_plot_data_list* list) {
    t_plot_data item;

    item.obj = (void *) polygon;
    item.typeID = (char *) polygonTypeID;
    item.draw = & draw_polygon_data;
    item.cleanup = NULL;

    add_data( list, &item );
}
