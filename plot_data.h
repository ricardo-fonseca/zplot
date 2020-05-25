#ifndef __PLOT_DATA__
#define __PLOT_DATA__

#include "plot_area.h"
#include "plot_axis.h"
#include <cairo.h>

typedef struct PlotData {
    void *obj;
    char *typeID;
    void (*draw)(struct PlotData *, t_plot_area *, t_plot_axis [], cairo_t * );
    void (*cleanup)(void*);
    struct PlotData *next;
} t_plot_data;

typedef struct {
    t_plot_data *head;
    t_plot_data *tail;
} t_plot_data_list;

void init_list( t_plot_data_list * list );
void cleanup_list( t_plot_data_list * list );

void add_data( t_plot_data_list * list, t_plot_data * data );
int remove_data_obj( t_plot_data_list * list, void * obj );

#endif
