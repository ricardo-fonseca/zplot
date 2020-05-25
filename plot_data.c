#include "plot_data.h"
#include <stddef.h>
#include <stdlib.h>

void init_list( t_plot_data_list * list ) {
    list -> head = NULL;
    list -> tail = NULL;
}

void cleanup_list( t_plot_data_list * list ) {
    t_plot_data *item = list -> head;
    while( item ) {
        t_plot_data *next = item -> next;

        if ( item -> cleanup ) item -> cleanup( item -> obj );
        free( item );
        item = next;
    }

    list -> head = list -> tail = NULL;
}

void add_data( t_plot_data_list * list, t_plot_data * data ){
    
    t_plot_data *item = (t_plot_data *) malloc( sizeof(t_plot_data));
    
    item -> obj = data -> obj;
    item -> typeID = data -> typeID;
    item -> draw  = data -> draw;
    item -> cleanup = data -> cleanup;
    item -> next = NULL;

    if ( !list -> head ) {
        list -> head = item;
    } else {
        list -> tail -> next = item;
    }

    list -> tail = item;
}

int remove_data_obj( t_plot_data_list * list, void * obj ) {
    t_plot_data *prev = NULL;
    t_plot_data *item;
    
    for(  item = list -> head; item; item = item -> next ) {
        if ( item -> obj == obj ) {
            if ( item == list -> head ) {
                list -> head = item -> next;
            } else {
                prev -> next = item -> next;
            }
            if ( item == list -> tail ) {
                list -> tail = prev;
            }
            free( item );
            return(0);
        }
        prev = item;
    }
    return(-1);
}
