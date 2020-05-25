#ifndef __PLOT_LINE__
#define __PLOT_LINE__

#include <cairo.h>

enum t_line_style{ NOLINE, SOLID, DASHED, DOTTED, DASHDOT, DASHDOTDOT };

void cairo_set_line_style( cairo_t *cr, enum t_line_style style );

#endif
