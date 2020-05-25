#ifndef __PLOT_COLORMAP__
#define __PLOT_COLORMAP__

#include "plot_data.h"
#include "plot_coloraxis.h"

enum colormap_opacity{ OPACITY_FIXED, OPACITY_LINEAR, OPACITY_LINEAR_ABS, OPACITY_LOG_ABS };

typedef struct {
    double* data;
    int xdim, ydim;

    double x0, x1;
    double y0, y1;

    t_plot_coloraxis coloraxis;

    enum colormap_opacity opacityType;
    double opacityRange[2];
    double alpha;

} t_plot_colormap;

void init_colormap( t_plot_colormap* colormap );
void draw_colormap( t_plot_colormap* colormap, t_plot_area *area, t_plot_axis axis[], cairo_t *cr );
void add_colormap( t_plot_colormap* colormap, t_plot_data_list* list);

#endif
