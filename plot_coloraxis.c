#include <stdio.h>
#include "plot_coloraxis.h"

void init_color_axis( t_plot_coloraxis * coloraxis ) {
    init_axis( &coloraxis -> axis, 1 );
    init_palette( &coloraxis -> palette );
}

void scale_color_axis( t_plot_coloraxis * coloraxis, double *data, int size, t_argb *img) {
    load_palette( &coloraxis -> palette );

    for( int i = 0; i < size; i+= SCALE_BUF_SIZE ) {
        double buffer[SCALE_BUF_SIZE];
        
        int s = size-i+1;
        if ( s > SCALE_BUF_SIZE ) s = SCALE_BUF_SIZE;
        
        scale_arr_axis( &coloraxis -> axis, & data[i], s, buffer );
        scale_palette( &coloraxis -> palette, buffer, s, & img[i]);
    }

    free_palette( &coloraxis -> palette );
}

void draw_color_axis( t_plot_coloraxis *coloraxis, t_plot_area *area, cairo_t *cr ) {
    printf("(*warn*) Drawing color axis not implemented yet");
}
