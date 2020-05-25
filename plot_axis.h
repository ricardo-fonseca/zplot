#ifndef __PLOT_AXIS__
#define __PLOT_AXIS__

#include <cairo.h>
#include "plot_color.h"
#include "plot_area.h"

typedef struct {
    // Number of tick marks
    int number;
    // Tick line direction
    int dir;
    // Tick line thickness
    double thick;
    // Tick line length
    double len;
    // Tick color
    t_color color;
} t_ticks;


enum axis_type {LINEAR, LOG, SYMLOG};

typedef struct {

    // Axis type
    enum axis_type type;
    
    // Axis range
    double min, max;
    // Threshold for sym. axis
    double sym;

    // Direction
    int direction;

    // Axis color
    t_color color;
    // Axis thickness
    double thick;
    // Major / minor tickmarks properties
    t_ticks major, minor;

} t_plot_axis;

void draw_axis( t_plot_axis* axis, t_plot_area *area, cairo_t *cr );
void init_axis( t_plot_axis* axis, int dir );

double scale_axis( t_plot_axis* axis, double val ); 
void scale_arr_axis( t_plot_axis* axis, double *val, int size, double *out );

#endif
