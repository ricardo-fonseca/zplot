#ifndef __PLOT_AREA__
#define __PLOT_AREA__

#include <cairo.h>
#include "plot_color.h"

typedef struct {
    double x0, x1;
    double y0, y1;

    int lineStyle;
    double lineThick;
    t_color lineColor;

    int bgStyle;
    t_color bgColor;
} t_plot_area;

void init_plot_area( t_plot_area *area );
void draw_plot_area( t_plot_area *area, cairo_t *cr );

#endif
