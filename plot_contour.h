#ifndef __PLOT_CONTOUR__
#define __PLOT_CONTOUR__

#include <cairo.h>
#include "plot_color.h"
#include "plot_line.h"
#include "plot_area.h"
#include "plot_axis.h"
#include "plot_data.h"

typedef struct {
    double* data;
    int xdim, ydim;

    double x0, x1;
    double y0, y1;

    enum t_line_style lineStyle;
    double lineThick;
    t_color lineColor;

    double* values;
    int nvalues;
} t_plot_contour;

void init_contour( t_plot_contour* contour );
void draw_contour( t_plot_contour *contour, t_plot_area *area, t_plot_axis axis[], cairo_t *cr );
void add_contour( t_plot_contour* contour, t_plot_data_list* list);

#endif
