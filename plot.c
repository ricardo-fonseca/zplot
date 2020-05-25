#include "plot.h"
#include <stddef.h>

void init_plot( t_plot *plot ) {
    plot -> bkgTransparent = 0;
    plot -> bkgColor = (t_color) {.r=1,.g=1,.b=1,.a=1};

    init_plot_area( &plot->area );
    init_title( &plot -> title);

    init_axis( &plot -> axis[0], 0 );
    init_axis( &plot -> axis[1], 1 );

    init_list( &plot -> data );
}


void draw_plot( t_plot *plot, cairo_t *cr ) {
    
    // Plot background
    if ( !plot -> bkgTransparent ) {
        cairo_set_source_color( cr, &plot-> bkgColor);
        cairo_paint (cr);
    }

    draw_plot_area( &plot->area, cr );
    draw_title( &plot->title, &plot->area, cr );

    // Draw data
    for( t_plot_data* data = plot -> data.head; data; data = data -> next ) {
        if ( data -> draw ) {
            (data->draw)(data, &plot->area, plot->axis, cr);
        }
    }

    // Draw axis
    draw_axis( &plot->axis[0], &plot->area, cr );
    draw_axis( &plot->axis[1], &plot->area, cr );

}

void cleanup_plot( t_plot * plot ) {
    // For now only structures that may require cleanup are
    // plot data objects
    cleanup_list( & plot -> data );
}
