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

    s2.lineColor = (t_color) {.r=0.1,.g=0.7,.b=.1,.a=0.8};
    s2.symbolStyle = CIRCLE;
    s2.symbolColor = (t_color) {.r=0.1,.g=0.7,.b=.1,.a=1.0};

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

    add_colormap( &cm, &plot -> data);
}

t_plot_polygon poly;
t_pointlist pos;

void polygon( t_plot * plot ) {
    init_polygon( &poly );

    add_pointlist( &pos, -1.0, 0.0 );
    add_pointlist( &pos, +1.0, 0.0 );
    add_pointlist( &pos,  0.0, 1.0 );

    poly.pos = &pos;
    poly.closed = 1;

    poly.style = 3;
    poly.fillColor = (t_color) {.r=0.8,.g=0.2,.b=0.1,.a=0.6};

    add_polygon( &poly, & plot -> data);
}

t_plot_contour ctr;
double ctr_values[16];
double z2[64*64];

void contour( t_plot * plot ) {

    for( int j = 0; j < 64; j++ ) {
        double y = j/63.0;
        for( int i = 0; i < 64; i++ ) {
            double x = -1 + 2 * i/63.0;
            z2[j*64 + i] = y*y+x*x;
        }
    }

    // Draw a colormap below for reference
    init_colormap( &cm );
    cm.data = z2;
    cm.xdim = 64;
    cm.ydim = 64;

    cm.x0 = -1;
    cm.x1 = +1;

    cm.y0 = 0;
    cm.y1 = +1;

    cm.coloraxis.axis.min = 0;
    cm.coloraxis.axis.max = +1;
    cm.coloraxis.palette.id = HALINE;
    add_colormap( &cm, &plot -> data);

    for( int i = 0; i < 16; i++ ) {
        ctr_values[i] =  (i+1)*2.0/17;
    }

    init_contour( &ctr );

    ctr.data = z2;
    ctr.xdim = 64;
    ctr.ydim = 64;

    ctr.x0 = -1;
    ctr.x1 = +1;

    ctr.y0 = 0;
    ctr.y1 = +1;

    ctr.values = ctr_values;
    ctr.nvalues = 16;

    add_contour( &ctr, & plot -> data);
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
    //series1( &plot );
    //colormap( &plot );
    //series2( &plot );

    // polygon( &plot );
    contour( &plot );


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

