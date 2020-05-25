/*
    Compile with:

    clang -I/usr/local/include/cairo -L/usr/local/lib/cairo -lcairo demo.c 
*/

#include <cairo.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void
hello_world()
{
    cairo_surface_t *surface =
        cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
    cairo_t *cr =
        cairo_create (surface);

    cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 0.9, 0.9, 0.9);
    cairo_move_to (cr, 10.0, 50.0);
    cairo_show_text (cr, "Hello, world");

    cairo_destroy (cr);
    cairo_surface_write_to_png (surface, "hello.png");
    cairo_surface_destroy (surface);
}

void
stroke()
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo_set_line_width (cr, 0.1);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
	cairo_stroke (cr);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "stroke.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void
fill()
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_rectangle (cr, 0.25, 0.25, 0.5, 0.5);
	cairo_fill (cr);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "fill.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void
show_text()
{
    cairo_surface_t *surface;
	cairo_t *cr;
	cairo_text_extents_t te;

	/* Prepare drawing area */
	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
	cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size (cr, 1.2);
	cairo_text_extents (cr, "a", &te);
	cairo_move_to (cr, 0.5 - te.width / 2 - te.x_bearing,
			0.5 - te.height / 2 - te.y_bearing);
	cairo_show_text (cr, "a");

	/* Write output and clean up */
	cairo_surface_write_to_png (surface, "showtext.png");
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}

void
paint()
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_paint_with_alpha (cr, 0.5);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "paint.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void
mask()
{
    cairo_surface_t *surface;
    cairo_t *cr;
	cairo_pattern_t *linpat, *radpat;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	linpat = cairo_pattern_create_linear (0, 0, 1, 1);
	cairo_pattern_add_color_stop_rgb (linpat, 0, 0, 0.3, 0.8);
	cairo_pattern_add_color_stop_rgb (linpat, 1, 0, 0.8, 0.3);

	radpat = cairo_pattern_create_radial (0.5, 0.5, 0.25, 0.5, 0.5, 0.75);
	cairo_pattern_add_color_stop_rgba (radpat, 0, 0, 0, 0, 1);
	cairo_pattern_add_color_stop_rgba (radpat, 0.5, 0, 0, 0, 0);

	cairo_set_source (cr, linpat);
	cairo_pattern_destroy (linpat);

	cairo_mask (cr, radpat);
	cairo_pattern_destroy (radpat);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "mask.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void
set_sourcergba()
{
    cairo_surface_t *surface;
    cairo_t *cr;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_move_to (cr, 0, 0);
	cairo_line_to (cr, 1, 1);
	cairo_move_to (cr, 1, 0);
	cairo_line_to (cr, 0, 1);
	cairo_set_line_width (cr, 0.2);
	cairo_stroke (cr);

	cairo_rectangle (cr, 0, 0, 0.5, 0.5);
	cairo_set_source_rgba (cr, 1, 0, 0, 0.80);
	cairo_fill (cr);

	cairo_rectangle (cr, 0, 0.5, 0.5, 0.5);
	cairo_set_source_rgba (cr, 0, 1, 0, 0.60);
	cairo_fill (cr);

	cairo_rectangle (cr, 0.5, 0, 0.5, 0.5);
	cairo_set_source_rgba (cr, 0, 0, 1, 0.40);
	cairo_fill (cr);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "setsourcergba.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void
set_sourcegradient()
{
    cairo_surface_t *surface;
    cairo_t *cr;
	cairo_pattern_t *radpat, *linpat;

    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 120, 120);
    cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	cairo_scale (cr, 120, 120);

	/* Drawing code goes here */
	radpat = cairo_pattern_create_radial (0.25, 0.25, 0.1,  0.5, 0.5, 0.5);
	cairo_pattern_add_color_stop_rgb (radpat, 0,  1.0, 0.8, 0.8);
	cairo_pattern_add_color_stop_rgb (radpat, 1,  0.9, 0.0, 0.0);

	int i, j;
	for (i=1; i<10; i++)
		for (j=1; j<10; j++)
			cairo_rectangle (cr, i/10.0 - 0.04, j/10.0 - 0.04, 0.08, 0.08);
	cairo_set_source (cr, radpat);
	cairo_fill (cr);

	linpat = cairo_pattern_create_linear (0.25, 0.35, 0.75, 0.65);
	cairo_pattern_add_color_stop_rgba (linpat, 0.00,  1, 1, 1, 0);
	cairo_pattern_add_color_stop_rgba (linpat, 0.25,  0, 1, 0, 0.5);
	cairo_pattern_add_color_stop_rgba (linpat, 0.50,  1, 1, 1, 0);
	cairo_pattern_add_color_stop_rgba (linpat, 0.75,  0, 0, 1, 0.5);
	cairo_pattern_add_color_stop_rgba (linpat, 1.00,  1, 1, 1, 0);

	cairo_rectangle (cr, 0.0, 0.0, 1, 1);
	cairo_set_source (cr, linpat);
	cairo_fill (cr);

	/* Write output and clean up */
    cairo_surface_write_to_png (surface, "setsourcegradient.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);
}

void textextents()
{
    /* Variable declarations */
	cairo_surface_t *surface;
	cairo_t *cr;
	double x, y, px, ux=1, uy=1, dashlength;
	char text[]="joy";
	cairo_font_extents_t fe;
	cairo_text_extents_t te;

	/* Prepare drawing area */
	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 240);
	cr = cairo_create (surface);
	/* Example is in 26.0 x 1.0 coordinate space */
	cairo_scale (cr, 240, 240);
	cairo_set_font_size (cr, 0.5);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_extents (cr, &fe);

	cairo_device_to_user_distance (cr, &ux, &uy);
	if (ux > uy)
		px = ux;
	else
		px = uy;
	cairo_font_extents (cr, &fe);
	cairo_text_extents (cr, text, &te);
	x = 0.5 - te.x_bearing - te.width / 2;
	y = 0.5 - fe.descent + fe.height / 2;

	/* baseline, descent, ascent, height */
	cairo_set_line_width (cr, 4*px);
	dashlength = 9*px;
	cairo_set_dash (cr, &dashlength, 1, 0);
	cairo_set_source_rgba (cr, 0, 0.6, 0, 0.5);
	cairo_move_to (cr, x + te.x_bearing, y);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y + fe.descent);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y - fe.ascent);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_move_to (cr, x + te.x_bearing, y - fe.height);
	cairo_rel_line_to (cr, te.width, 0);
	cairo_stroke (cr);

	/* extents: width & height */
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_set_line_width (cr, px);
	dashlength = 3*px;
	cairo_set_dash (cr, &dashlength, 1, 0);
	cairo_rectangle (cr, x + te.x_bearing, y + te.y_bearing, te.width, te.height);
	cairo_stroke (cr);

	/* text */
	cairo_move_to (cr, x, y);
	cairo_set_source_rgb (cr, 0, 0, 0);
	cairo_show_text (cr, text);

	/* bearing */
	cairo_set_dash (cr, NULL, 0, 0);
	cairo_set_line_width (cr, 2 * px);
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_move_to (cr, x, y);
	cairo_rel_line_to (cr, te.x_bearing, te.y_bearing);
	cairo_stroke (cr);

	/* text's advance */
	cairo_set_source_rgba (cr, 0, 0, 0.75, 0.5);
	cairo_arc (cr, x + te.x_advance, y + te.y_advance, 5 * px, 0, 2 * M_PI);
	cairo_fill (cr);

	/* reference point */
	cairo_arc (cr, x, y, 5 * px, 0, 2 * M_PI);
	cairo_set_source_rgba (cr, 0.75, 0, 0, 0.5);
	cairo_fill (cr);

	/* Write output and clean up */
	cairo_surface_write_to_png (surface, "textextents.png");
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}

void
tipsfont()
{
    cairo_surface_t *surface;
	cairo_t *cr;
	int i;
	cairo_font_extents_t fe;
	cairo_text_extents_t te;
	char alphabet[] = "AbCdEfGhIjKlMnOpQrStUvWxYz";
	char letter[2];

	/* Prepare drawing area */
	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 780, 30);
	cr = cairo_create (surface);
	/* Example is in 26.0 x 1.0 coordinate space */
	cairo_scale (cr, 30, 30);
	cairo_set_font_size (cr, 0.8);

	/* Drawing code goes here */
	cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
	cairo_select_font_face (cr, "Georgia",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_font_extents (cr, &fe);
	for (i=0; i < strlen(alphabet); i++) {
		/* get each letter separately
		 * (from comp.lang.c FAQ, Question 13.2) */
		*letter = '\0';
		strncat (letter, alphabet + i, 1);

		cairo_text_extents (cr, letter, &te);
		cairo_move_to (cr, i + 0.5 - te.x_bearing - te.width / 2,
				0.5 - fe.descent + fe.height / 2);
		cairo_show_text (cr, letter);
	}

	/* Write output and clean up */
	cairo_surface_write_to_png (surface, "tips-font.png");
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
}

int
main (int argc, char *argv[])
{
    hello_world();
    stroke();
    fill();
    show_text();
    paint();
    mask();
    set_sourcergba();
    set_sourcegradient();
    textextents();
    tipsfont();

    return 0;
}
