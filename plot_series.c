#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include "plot_data.h"
#include "plot_series.h"

const char seriesTypeID[] = "t_plot_series";

void draw_series_data( t_plot_data *data, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ) {
    if ( data -> typeID == seriesTypeID ) {
        t_plot_series* series = (t_plot_series*) data -> obj;
        draw_series( series, area, axis, cr );
    } else {
        fprintf(stderr,"(*error*) draw_series_data called without a t_plot_series object.\n");
        exit(-1);
    }
}

void add_series( t_plot_series* series, t_plot_data_list* list) {
    t_plot_data item;

    item.obj = (void *) series;
    item.typeID = (char *) seriesTypeID;
    item.draw = & draw_series_data;
    item.cleanup = NULL;

    add_data( list, &item );
}


void init_series( t_plot_series* series ){
    series -> x = NULL;
    series -> y = NULL;
    series -> np = 0;

    series -> lineStyle = SOLID;
    series -> lineThick = 0.003;
    series -> lineColor = (t_color) {.r=0.5,.g=0,.b=0,.a=1};

    series -> symbolStyle = NOSYMBOL;
    series -> symbolThick = 0.001;
    series -> symbolSize = 0.01;
    series -> symbolColor = (t_color) {.r=0.5,.g=0,.b=0,.a=1};
}

void draw_series( t_plot_series *series, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ){

    cairo_save( cr );

    double const sx = area -> x1 - area -> x0;
    double const sy = area -> y1 - area -> y0;

    // Clip to plot region
    cairo_rectangle( cr, area->x0, area->y0, sx, sy );
    cairo_clip(cr);

    // Draw line
    if (( series -> lineStyle > NOLINE ) && ( series -> np > 1 )) {
        cairo_set_source_color( cr, &series-> lineColor);
        cairo_set_line_width( cr, series -> lineThick );
        cairo_set_line_style( cr,series -> lineStyle );

        double x = scale_axis( &axis[0], series -> x[0]);
        double y = scale_axis( &axis[1], series -> y[0]);

        cairo_move_to( cr, area -> x0 + x*sx, area -> y1 - y*sy);
        for(int i = 1; i < series -> np; i++) {
            x = scale_axis( &axis[0], series -> x[i]);
            y = scale_axis( &axis[1], series -> y[i]);
            cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy);
        }
        cairo_stroke(cr);

        // Clear dash
        cairo_set_line_style( cr, SOLID );
    }

    // Draw symbols
    if (( series -> symbolStyle > NOSYMBOL ) && ( series -> np > 0 )) {
        cairo_set_source_color( cr, &series-> symbolColor);
        cairo_set_line_width( cr, series -> symbolThick );

        switch (series -> symbolStyle)
        {
        case DOT:
            cairo_set_line_width( cr, series -> symbolSize / sqrt(2.0) );
            for(int i = 0; i < series -> np; i++) {
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                cairo_move_to( cr, area -> x0 + x*sx, area -> y1 - y*sy);
                cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy);
            }
            cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke (cr);
            break;

        case CIRCLE:
            for(int i = 0; i < series -> np; i++) {
                double const rad = series -> symbolSize / sqrt(2.0);
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                cairo_new_sub_path ( cr );
                cairo_arc( cr, area -> x0 + x*sx, area -> y1 - y*sy,
                    rad, 0, 2 * M_PI );
            }
            cairo_stroke (cr);
            break;

        case UPTRIANGLE:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize * sqrt(3.0) / 2;
                double const b = series -> symbolSize / 2;
                double const c = series -> symbolSize;

                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy + b);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy + b);
                cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy - c );
                cairo_close_path( cr );
            }
            cairo_stroke (cr);
            break;

        case DOWNTRIANGLE:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize * sqrt(3.0) / 2;
                double const b = series -> symbolSize / 2;
                double const c = series -> symbolSize;

                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy - b);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy - b);
                cairo_line_to( cr, area -> x0 + x*sx, area -> y1 - y*sy + c );
                cairo_close_path( cr );
            }
            cairo_stroke (cr);
            break;

        case SQUARE:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize / sqrt(2.0) ;
    
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy + a);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy + a);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy - a);
                cairo_line_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy - a);
                cairo_close_path( cr );
            }
            cairo_stroke (cr);
            break;
        
        case DIAMOND:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize;
    
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy    );
                cairo_line_to( cr, area -> x0 + x*sx    , area -> y1 - y*sy + a);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy    );
                cairo_line_to( cr, area -> x0 + x*sx    , area -> y1 - y*sy - a);
                cairo_close_path( cr );
            }
            cairo_stroke (cr);
            break;

        case PLUS:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize;
    
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy );
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy );
                cairo_move_to( cr, area -> x0 + x*sx    , area -> y1 - y*sy + a );
                cairo_line_to( cr, area -> x0 + x*sx    , area -> y1 - y*sy - a );
            }
            cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke (cr);
            break;

        case TIMES:
            for(int i = 0; i < series -> np; i++) {
                double const a = series -> symbolSize / sqrt(2.0);
    
                double x = scale_axis( &axis[0], series -> x[i]);
                double y = scale_axis( &axis[1], series -> y[i]);
                
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy + a);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy - a);
                cairo_move_to( cr, area -> x0 + x*sx - a, area -> y1 - y*sy - a);
                cairo_line_to( cr, area -> x0 + x*sx + a, area -> y1 - y*sy + a);
            }
            cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
            cairo_stroke (cr);
            break;

        case NOSYMBOL:
            break;
        }
    }

    cairo_restore( cr );

}
