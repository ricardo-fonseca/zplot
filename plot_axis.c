#include "plot_axis.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>

/**
 * Initializes axis parameters to default values
 * 
 * @param axis  Axis
 */
void init_axis( t_plot_axis* axis, int dir ) {
    axis -> type = LINEAR;
    axis -> min = 0;
    axis -> max = 1;
    axis -> sym = 0.01;
    axis -> direction = dir;

    axis -> color = (t_color) {.r=0,.g=0, .b=0, .a=1};
    axis -> thick = 0.003;

    axis -> major = (t_ticks) {
        .number = -1,
        .dir = 1,
        .thick = 0.01,
        .len = 0.05,
        .color = (t_color) {.r=0,.g=0, .b=0, .a=1}
    };

    axis -> minor = (t_ticks) {
        .number = -1,
        .dir = 1,
        .thick = 0.005,
        .len = 0.025,
        .color = (t_color) {.r=0,.g=0, .b=0, .a=0.5}
    };
}

/**
 * Returns the automatic major tick interval for the axis range
 * @param   axis        Axis object
 * @param   interval    Interval between major tick marks
 * @param   ia          Integer index of the first tick mark
 */
void tick_interval( t_plot_axis* axis, double* interval, int64_t* ia ) {
    
    double const delta = axis->max - axis->min;
    *interval = pow(10.0, floor( log10(delta) ));
    int s = delta/ *interval;

    switch(s) {
    case 2:
        *interval *= 0.5;
        break;
    case 1:
        *interval *= 0.2;
    }

    *ia = floor(axis->min / *interval);
    if (*ia * *interval < axis -> min) *ia = *ia + 1;
}



/**
 * Draws axis on cairo context
 * 
 * @param axis  Axis
 * @param area  Plot area
 * @param cr    Cairo context
 */
void draw_axis( t_plot_axis* axis, t_plot_area *area, cairo_t *cr ){
    // Draw axis line
    cairo_set_source_color( cr, &axis->color);
    cairo_set_line_width( cr, axis -> thick );

    cairo_move_to( cr, area -> x0, area -> y1);
    switch (axis->direction){
    case 0:
        cairo_line_to( cr, area -> x1, area -> y1);
        break;
    case 1:
        cairo_line_to( cr, area -> x0, area -> y0);
        break;
    }
    cairo_stroke(cr);

    int64_t ia;
    double interval;
    tick_interval( axis, &interval, &ia);

    cairo_select_font_face (cr, "Lato", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
    cairo_set_font_size (cr, 0.03 );
    cairo_text_extents_t te;

    switch (axis->direction){
    case 0: {
        double const length = area -> x1 - area -> x0;

        // Major tick marks
        for( int64_t i = ia; i * interval <= axis -> max; i++ ) {
            double value = i * interval;
            double pos = area -> x0 + scale_axis( axis, value ) * length;

            cairo_move_to( cr, pos, area -> y1 + 0.01);
            cairo_line_to( cr, pos, area -> y1 - 0.01);
            cairo_stroke(cr);

            char str[32];
            snprintf(str,32,"%g", value);
            cairo_text_extents (cr, str, &te);

            cairo_move_to( cr, 
                pos - te.width / 2 - te.x_bearing, 
                area -> y1 + 0.05);
            cairo_show_text(cr, str);
        }

    }
    break;
    case 1: {
        double const length = area -> y1 - area -> y0;
        for( int64_t i = ia; i * interval <= axis -> max; i++ ) {
            double value = i * interval;
            double pos = area -> y1 - scale_axis( axis, value ) * length;

            cairo_move_to( cr, area -> x0 - 0.01, pos);
            cairo_line_to( cr, area -> x0 + 0.01, pos);
            cairo_stroke(cr);

            char str[32];
            snprintf(str,32,"%g", value);
            cairo_text_extents (cr, str, &te);

            cairo_move_to( cr, 
                area -> x0 - te.width - te.x_bearing - 0.02, 
                pos - te.height / 2 - te.y_bearing);
            cairo_show_text(cr, str);
        }
    }
    break;
    }
}

/**
 * Scales a value according to the axis parameters. The value
 * returned will be 0 if the value is equal to the axis
 * minimum, and 1 if the value is equal to the axis maximum.
 * 
 * Note that the value may be outside of axis range. In this
 * case the return value will be outside the [0,1] range
 * 
 * @param   axis    Axis object
 * @param   val     Value to scale
 * @return          Value scaled in axis units
 */
double scale_axis( t_plot_axis* axis, double val ){
    double scale = (val - axis->min)/(axis->max-axis->min);
    return scale;
}

/**
 * Scales an array of values according to the axis parameters. 
 * The values  returned will be 0 if the value is equal to the axis
 * minimum, and 1 if the value is equal to the axis maximum.
 * 
 * Note that the value may be outside of axis range. In this
 * case the return value will be outside the [0,1] range
 * 
 * @param   axis    Axis object
 * @param   val     Array of values to scale
 * @param   size    Number of values to scale
 * @param   out     Output values
 */
void scale_arr_axis( t_plot_axis* axis, double *val, int size, double *out ){
    
    double rdelta = 1.0/(axis->max-axis->min);
    for( int i = 0; i < size; i++) {    
        out[i] = (val[i] - axis->min) * rdelta;
    }
}
