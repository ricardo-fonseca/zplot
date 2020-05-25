#include "plot_title.h"

void init_title( t_plot_title* title ){
    title -> text = "";
    title -> color = (t_color) {.r = 0, .g = 0, .b = 0, .a = 1};
    title -> halign = 0;
    title -> font_size = 0.05;
}

void draw_title( t_plot_title* title, t_plot_area *area, cairo_t *cr ){
    cairo_set_source_color( cr, &title->color );
    cairo_select_font_face (cr, "Lato", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL );
    cairo_set_font_size (cr, title -> font_size );

    cairo_text_extents_t te;
    cairo_text_extents (cr, title->text, &te);

    double x = (area -> x0 + area -> x1)/2;
    double y = (area -> y0)/2;

    cairo_move_to (cr, 
            x - te.width / 2 - te.x_bearing,
			y - te.height / 2 - te.y_bearing);
	cairo_show_text (cr, title -> text );
}
