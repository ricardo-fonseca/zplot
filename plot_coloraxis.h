#ifndef __PLOT_COLORAXIS__
#define __PLOT_COLORAXIS__

#include "plot_axis.h"
#include "plot_palette.h"

#define SCALE_BUF_SIZE 1024

typedef struct {
    t_plot_axis axis;
    t_plot_palette palette;
} t_plot_coloraxis;

void init_color_axis( t_plot_coloraxis * coloraxis );
void scale_color_axis( t_plot_coloraxis * coloraxis, double *data, int size, t_argb *img);
void draw_color_axis( t_plot_coloraxis *series, t_plot_area *area, cairo_t *cr );

#endif
