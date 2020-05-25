#ifndef __PLOT_COLOR__
#define __PLOT_COLOR__

#include <cairo.h>

typedef struct {
    double r, g, b, a;
} t_color;

void cairo_set_source_color (cairo_t *cr, t_color *color);

#endif
