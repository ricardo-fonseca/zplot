#include "plot.h"
#include <cairo.h>

#define SX 800
#define SY 800

#include <math.h>

t_plot_series s1;
double xs1[100];
double ys1[100];

void series1( t_plot * plot ) {
    init_series( &s1);

    for( int i = 0; i<100; i++ ) {
        xs1[i] = -1.0 + i* 2.0/99;
        ys1[i] = 3*xs1[i]*xs1[i];
    }

    s1.x = xs1;
    s1.y = ys1;
    s1.np = 100;

    s1.lineStyle = DASHED;

    add_series( &s1, & plot -> data);
}

t_plot_series s2;
double xs2[50];
double ys2[50];

void series2( t_plot * plot ) {
    init_series( &s2);

    for( int i = 0; i<50; i++ ) {
        xs2[i] = -1.0 + i* 2.0/49;
        ys2[i] = 0.5*(xs2[i] + 1.0);
    }

    s2.x = xs2;
    s2.y = ys2;
    s2.np = 50;

    s2.symbolStyle = CIRCLE;

    add_series( &s2, & plot -> data);

}

t_plot_colormap cm;
double z[128*128];

void colormap( t_plot * plot ) {
    init_colormap( &cm );

    for( int j = 0; j < 128; j++ ) {
        double y = (2 * M_PI * j)/64;
        for( int i = 0; i < 128; i++ ) {
            double x = (2 * M_PI * i)/64;
            z[j*128 + i] = sin(x) * sin(y);
        }
    }


    cm.data = z;
    cm.xdim = 128;
    cm.ydim = 128;

    cm.x0 = -1;
    cm.x1 = +1;

    cm.y0 = 0;
    cm.y1 = +1;

    cm.coloraxis.axis.min = -1;
    cm.coloraxis.axis.max = +1;
    cm.coloraxis.palette.id = THERMAL;

    cm.opacityType = OPACITY_LINEAR_ABS;
    cm.opacityRange[0] = 0;
    cm.opacityRange[1] = 0.4;

    add_colormap( &cm, & plot -> data);

}

int main (int argc, char *argv[])
{

    t_plot plot;

    init_plot( &plot );

    // Plot title
    plot.title.text = "Sample";

    plot.axis[0].min = -1.1;
    plot.axis[0].max = 1.1;
    
    plot.axis[1].min = -0.1;
    plot.axis[1].max = 1.1;

    // Plot data
    series1( &plot );
    colormap( &plot );
    series2( &plot );


    // Create surface
    cairo_surface_t *surface = 
        cairo_image_surface_create(CAIRO_FORMAT_ARGB32, SX, SY);
    
    // Create context
    cairo_t *cr = cairo_create (surface);

    // This sets a 1.0 x 1.0 coordinate space for the context
    cairo_scale (cr, SX, SY);

    // Draw on context
    draw_plot( &plot, cr );

    // Write output and clean up
    cairo_surface_write_to_png (surface, "plot.png");
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

}

