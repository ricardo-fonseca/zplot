#ifndef __PLOT__
#define __PLOT__

#include "plot_color.h"
#include "plot_area.h"
#include "plot_title.h"
#include "plot_axis.h"
#include "plot_data.h"
#include "plot_series.h"
#include "plot_colormap.h"
#include "plot_polygon.h"
#include "plot_contour.h"


typedef struct {
    int bkgTransparent;
    t_color bkgColor;

    t_plot_area area;
    t_plot_title title;
    t_plot_axis axis[2];

    t_plot_data_list data;

} t_plot;

void init_plot( t_plot *plot );
void draw_plot( t_plot *plot, cairo_t *cr );
void cleanup_plot( t_plot *plot );

#endif
