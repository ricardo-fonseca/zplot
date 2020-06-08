#ifndef __PLOT_POLYGON__
#define __PLOT_POLYGON__

#include <cairo.h>
#include "plot_color.h"
#include "plot_line.h"
#include "plot_area.h"
#include "plot_axis.h"
#include "plot_data.h"

#include "points.h"

typedef struct {
    t_pointlist *pos;

    int closed;

    int style;
    
    double lineThick;  
    enum t_line_style lineStyle;
    t_color lineColor;

    t_color fillColor;

} t_plot_polygon;

void init_polygon( t_plot_polygon* polygon );
void cleanup_polygon( t_plot_polygon* polygon );
void draw_polygon( t_plot_polygon *polygon, t_plot_area *area, t_plot_axis axis[], cairo_t *cr );
void add_polygon( t_plot_polygon* polygon, t_plot_data_list* list);


#endif
