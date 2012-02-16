#ifndef _READYQUEUE_H_
#define _READYQUEUE_H_

#include "ULT.h"

typedef struct the_queue {

	ThrdCtlBlk *head;
	ThrdCtlBlk *running;
	int num_tcb;
	
} the_queue;

Tid init_queue( the_queue* myQueue );
void add_to_queue( the_queue* queue, ThrdCtlBlk* tcb );
ThrdCtlBlk* find_in_queue( the_queue* queue, Tid find_tid );
int remove_from_queue( the_queue* queue, Tid find_tid );

#endif