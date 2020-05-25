#ifndef __COLORTABLES__
#define __COLORTABLES__

#include <stdint.h>

enum color_table{
    THERMAL,HALINE,SOLAR,ICE,GRAY,OXY,DEEP,DENSE,
    ALGAE,MATTER,TURBID,SPEED,AMP,TEMPO,RAIN,PHASE,
    TOPO,BALANCE,DELTA,CURL,DIFF,TARN
};

const uint8_t * values_colortable( enum color_table id );

#endif
