#include "plot_colormap.h"
#include "plot_data.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

const char colormapTypeID[] = "t_plot_colormap";

void draw_colormap_data( t_plot_data *data, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ) {
    if ( data -> typeID == colormapTypeID ) {
        t_plot_colormap* colormap = (t_plot_colormap*) data -> obj;
        draw_colormap( colormap, area, axis, cr );
    } else {
        fprintf(stderr,"(*error*) draw_colormap_data called without a t_plot_series object.\n");
        exit(-1);
    }
}

void add_colormap( t_plot_colormap* colormap, t_plot_data_list* list) {
    t_plot_data item;

    item.obj = (void *) colormap;
    item.typeID = (char *) colormapTypeID;
    item.draw = & draw_colormap_data;
    item.cleanup = NULL;

    add_data( list, &item );
}

void init_colormap( t_plot_colormap* colormap ){

    colormap -> data = NULL;
    colormap -> xdim = 0;
    colormap -> ydim = 0;

    colormap -> x0 = colormap -> x1 = 0;
    colormap -> y0 = colormap -> y1 = 0;

    init_color_axis( & colormap -> coloraxis );

    colormap -> opacityType = OPACITY_FIXED;
    colormap -> opacityRange[0] = 0;
    colormap -> opacityRange[1] = 0;
    colormap -> alpha = 1;
}

void set_opacity( t_plot_colormap* colormap, t_argb* img ) {
    const int size = colormap -> xdim * colormap -> ydim;

    switch (colormap -> opacityType) {
    case OPACITY_FIXED:
        if ( colormap -> alpha < 1 ) {
            uint8_t a = colormap -> alpha * 255;
            for(int i = 0; i < size; i++) {
                t_argb color = img[i];
                color.argb.a = a;
                color.argb.r *= colormap -> alpha;
                color.argb.g *= colormap -> alpha;
                color.argb.b *= colormap -> alpha;
                img[i] = color;
            }
        }
        break;
    case OPACITY_LINEAR:
        for(int i = 0; i < size; i++) {
            double const min = colormap -> opacityRange[0];
            double const max = colormap -> opacityRange[1];
            double const delta = max-min;

            double a = ( colormap -> data[i] - min )/delta;
            if (a<0) a = 0;
            if (a>1) a = 1;
            a *= colormap -> alpha;
            
            t_argb color = img[i];
            color.argb.a *= a;
            color.argb.r *= a;
            color.argb.g *= a;
            color.argb.b *= a;
            img[i] = color;
        }
        break;
    case OPACITY_LINEAR_ABS:
        for(int i = 0; i < size; i++) {
            double const min = colormap -> opacityRange[0];
            double const max = colormap -> opacityRange[1];
            double const delta = max-min;

            double a = (fabs(colormap -> data[i])-min)/delta;
            if (a<0) a = 0;
            if (a>1) a = 1;
            a *= colormap -> alpha;
            
            t_argb color = img[i];
            color.argb.a *= a;
            color.argb.r *= a;
            color.argb.g *= a;
            color.argb.b *= a;
            img[i] = color;
        }
        break;
    case OPACITY_LOG_ABS:
        for(int i = 0; i < size; i++) {
            double const min = log10(colormap -> opacityRange[0]);
            double const max = log10(colormap -> opacityRange[1]);
            double const delta = max-min;

            double a = (log10(fabs(colormap -> data[i]))-min)/delta;
            if (a<0) a = 0;
            if (a>1) a = 1;
            a *= colormap -> alpha;
            
            t_argb color = img[i];
            color.argb.a *= a;
            color.argb.r *= a;
            color.argb.g *= a;
            color.argb.b *= a;
            img[i] = color;
        }
        break;
    }
}

void draw_colormap( t_plot_colormap* colormap, t_plot_area *area, t_plot_axis axis[], cairo_t *cr ){

    const int size = colormap -> xdim * colormap -> ydim;
    t_argb* img = (t_argb*) malloc( size * sizeof(t_argb));  

    // Create colormap
    scale_color_axis( & colormap -> coloraxis, colormap -> data, size, img);

    // Set transparency values
    set_opacity( colormap, img );

    // Create cairo image
    cairo_save( cr );

    double const sx = area -> x1 - area -> x0;
    double const sy = area -> y1 - area -> y0;

    // Clip to plot region
    cairo_rectangle( cr, area->x0, area->y0, sx, sy );
    cairo_clip(cr);

    int stride = cairo_format_stride_for_width (
        CAIRO_FORMAT_ARGB32,
        colormap -> xdim);

    cairo_surface_t *image =
        cairo_image_surface_create_for_data (
            (unsigned char *) img,
            CAIRO_FORMAT_ARGB32,
            colormap -> xdim,
            colormap -> ydim,
            stride
        );

    double x = scale_axis( &axis[0], colormap -> x0);
    double y = scale_axis( &axis[1], colormap -> y0);
   
    double w = scale_axis( &axis[0], colormap -> x1) - x;
    double h = scale_axis( &axis[1], colormap -> y1) - y;

    cairo_translate(cr, area -> x0 + x*sx, area -> y0 + y*sy);
    cairo_scale (cr, w*sx/(colormap -> xdim), h*sy/colormap -> ydim);
    cairo_set_source_surface (cr, image, 0.0, 0.0);
    cairo_paint (cr);
    cairo_surface_destroy (image);
    
    free(img);

    cairo_restore( cr );
}
