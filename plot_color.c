#include "plot_color.h"

/**
 * This is a simple wrapper function for cairo_set_source_rgba
 * to allow the color to be set from a t_color structure.
 * @param   cr      Cairo context
 * @param   color   t_color structure
 */
void cairo_set_source_color (cairo_t *cr, t_color *color){

    cairo_set_source_rgba (cr,
        color -> r,
        color -> g,
        color -> b,
        color -> a );
}
