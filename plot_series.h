#ifndef __PLOT_SERIES__
#define __PLOT_SERIES__

#include <cairo.h>
#include "plot_color.h"
#include "plot_line.h"
#include "plot_area.h"
#include "plot_axis.h"
#include "plot_data.h"

enum t_symbol_style{ NOSYMBOL, DOT, CIRCLE, UPTRIANGLE, DOWNTRIANGLE, SQUARE, DIAMOND, PLUS, TIMES};

typedef struct {
    double *x;
    double *y;
    int np;

    enum t_line_style lineStyle;
    double lineThick;
    t_color lineColor;

    enum t_symbol_style symbolStyle;
    double symbolThick;
    double symbolSize;
    t_color symbolColor;

} t_plot_series;

void init_series( t_plot_series* series );
void draw_series( t_plot_series *series, t_plot_area *area, t_plot_axis axis[], cairo_t *cr );
void add_series( t_plot_series* series, t_plot_data_list* list);

#endif
