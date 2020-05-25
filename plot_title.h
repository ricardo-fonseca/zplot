#ifndef __PLOT_TITLE__
#define __PLOT_TITLE__

#include <cairo.h>
#include "plot_color.h"
#include "plot_area.h"

typedef struct {
    char *text;
    t_color color;
    int halign;
    double font_size;
} t_plot_title;

void init_title( t_plot_title* title );
void draw_title( t_plot_title* title, t_plot_area *area, cairo_t *cr );

#endif
