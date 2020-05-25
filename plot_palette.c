#include "plot_palette.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>


void init_palette( t_plot_palette *palette ){
    palette -> id = GRAY;
    palette -> table = NULL;
    palette -> invert = 0;
    palette -> gamma = 1.0;

    palette -> over = 0;
    palette -> overColor = (t_color) {.r=1,.g=1,.b=1,.a=1};

    palette -> under = 0;
    palette -> underColor = (t_color) {.r=0,.g=0,.b=0,.a=1};
}

void scale_palette( t_plot_palette *palette, double *data, int size, t_argb * img ) {

    t_argb over;
    t_argb under;

    if ( palette -> under ) {
        double alpha = palette -> underColor.a;
        under.argb.a = alpha * 255;
        under.argb.r = alpha * palette -> underColor.r * 255;
        under.argb.g = alpha * palette -> underColor.g * 255;
        under.argb.b = alpha * palette -> underColor.b * 255;
    } else {
        under = palette -> table[ 0 ];
    }

    if ( palette -> over ) {
        double alpha = palette -> overColor.a;
        over.argb.a = alpha * 255;
        over.argb.r = alpha * palette -> overColor.r * 255;
        over.argb.g = alpha * palette -> overColor.g * 255;
        over.argb.b = alpha * palette -> overColor.b * 255;
    } else {
        over = palette -> table[ 255 ];
    }

    for( int i = 0; i < size; i++ ) {
        if ( data[i] <= 0 ) {
            img[i] = under;
        } else if ( data[i] >= 1 ) {
            img[i] = over;
        } else {
            int idx = data[i] * 255;
            img[i] = palette -> table[ idx ];
        }
    }
}

void load_palette( t_plot_palette *palette ) {
    if ( palette -> table ) free(palette -> table);
    palette -> table = (t_argb *) malloc( 256 * sizeof(t_argb));
    
    // Load color table

    // Gamma correction not implemented yet

    const uint8_t *val = values_colortable( palette -> id );
    for( int i = 0; i < 256; i++) {
        t_argb color;
        color.argb.a = 255;
        color.argb.r = val[ 3*i ];
        color.argb.g = val[ 3*i + 1 ];
        color.argb.b = val[ 3*i + 2 ];

        if ( palette -> invert ) {
            palette -> table[255-i] = color;
        } else {
            palette -> table[i] = color;
        }
    }
}

void free_palette( t_plot_palette *palette ) {
    if ( palette -> table ) {
        free( palette -> table );
        palette -> table = NULL;
    }
}
