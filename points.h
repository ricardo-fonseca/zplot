#ifndef __POINTS__
#define __POINTS__


typedef struct {
    double x, y;
} t_point;

typedef struct {
    t_point *points;
    int bufferSize;
    int np;
} t_pointlist;

void init_pointlist( t_pointlist * list );
void cleanup_pointlist( t_pointlist * list );
void add_pointlist( t_pointlist * list, double x, double y );
void add_pointlist_point( t_pointlist * list, t_point point );

void merge_pointlist( t_pointlist *restrict listA, const t_pointlist *restrict listB,
    int prepend, int reverse );

#endif
