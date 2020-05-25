#include "plot_area.h"

void init_plot_area( t_plot_area *area ) {
    area -> x0 = 0.1;
    area -> x1 = 0.9;

    area -> y0 = 0.1;
    area -> y1 = 0.9;

    area -> lineStyle = 0;
    area -> lineThick = 0.01;
    area -> lineColor = (t_color) {.r = 0, .g = 0, .b = 0, .a = 0.5};

    area -> bgStyle = 1;
    area -> bgColor = (t_color) {.r = 1, .g = 1, .b = 1, .a = 1};
}

void draw_plot_area( t_plot_area *area, cairo_t *cr ) {
	
    int style = 0;
    if ( area -> lineStyle ) style += 1;
    if ( area -> bgStyle ) style += 2;

    if ( style ) {

        cairo_rectangle (cr, area->x0, area->y0, 
            area->x1-area->x0, area->y1-area->y0);

        switch (style)
        {
        case 3:
            // Line and fill
            // For some reason that I don't quite understand
            // The line has to be drawn first
            cairo_set_source_color(cr, &area -> lineColor);
            cairo_set_line_width (cr, area -> lineThick);
            cairo_stroke_preserve (cr); 
            cairo_set_source_color(cr, &area -> bgColor);
            cairo_fill(cr);
            break;

        case 2:
            // Fill only
            cairo_set_source_color(cr, &area -> bgColor);
            cairo_fill(cr);
            break;

        case 1:
            // Line only
            cairo_set_source_color(cr, &area -> lineColor);
            cairo_set_line_width (cr, area -> lineThick);
            cairo_stroke(cr);
            break;
        }

    }
}
