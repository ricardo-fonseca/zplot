#ifndef __PLOT_PALETTE__
#define __PLOT_PALETTE__

#include <stdint.h>
#include "plot_color.h"
#include "colortables.h"

typedef union {
    struct {
        uint8_t b, g, r, a;
    } argb;
    uint32_t pix;
} t_argb;


typedef struct {
    enum color_table id;  
    t_argb *table;
    int invert;
    double gamma;

    int over;
    t_color overColor;
    int under;
    t_color underColor;

} t_plot_palette;

void init_palette( t_plot_palette *palette );
void load_palette( t_plot_palette *palette );
void free_palette( t_plot_palette *palette );
void scale_palette( t_plot_palette *palette, double *data, int size, t_argb * img );

#endif
