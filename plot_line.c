#include <stddef.h>
#include "plot_line.h"

void cairo_set_line_style( cairo_t *cr, enum t_line_style style ){
    switch (style)
    {
    case NOLINE:
    case SOLID:
        cairo_set_dash( cr, NULL, 0, 0.0);
        break;
    
    case DASHED: {
            double dashes[] = {0.02,0.01};
            double offset = 0.0;
            cairo_set_dash( cr, dashes, 2, offset);
        }
        break;
    
    case DOTTED:{
            double dashes[] = {0.005,0.005};
            double offset = 0.0;
            cairo_set_dash( cr, dashes, 2, offset);
        }
        break;
    
    case DASHDOT:{
            double dashes[] = {0.02,0.005,0.005,0.005};
            double offset = 0.0;
            cairo_set_dash( cr, dashes, 4, offset);
        }
        break;
    
    case DASHDOTDOT:{
            double dashes[] = {0.02,0.005,0.005,0.005,0.005,0.005};
            double offset = 0.0;
            cairo_set_dash( cr, dashes, 6, offset);
        }
        break;
    }
}
