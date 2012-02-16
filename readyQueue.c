#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "ULT.h"
#include "readyQueue.h"

Tid init_queue( the_queue* queue )
{	
	ThrdCtlBlk* run_tcb = malloc( sizeof(ThrdCtlBlk) );
	int alloc = getcontext( &(run_tcb->my_context) );
	
	if ( alloc != 0 )
		return ULT_NOMEMORY;
	
	//set up queue
	queue->head = NULL;			//no currently running thread yet
	queue->num_tcb = 0;			//no ready threads in queue
	
	//set up tid 0 thread
	run_tcb->my_tid = 0;
	run_tcb->my_next = NULL;
	run_tcb->my_prev = NULL;
	
	//tid 0 is first running thread
	queue->running = run_tcb;
	
	return 0;
}

//returns 0 on failure, 1 on success
void add_to_queue( the_queue* queue, ThrdCtlBlk* tcb )
{

	if (queue->head == NULL)
		queue->head = tcb;

	else
	{
		ThrdCtlBlk* temp = queue->head;
		
		while (temp->my_next != NULL)
		{
			if (temp->my_tid > tcb->my_tid)
				break;
			temp = temp->my_next;
		}
		
		temp->my_prev->my_next = tcb;
		tcb->my_prev = temp->my_prev;
		temp->my_prev = tcb;
		tcb->my_next = temp;
	}
	
}

ThrdCtlBlk* find_in_queue( the_queue* queue, Tid find_tid )
{
	if (queue->head == NULL)
		return NULL;
		
	else
	{
		ThrdCtlBlk* temp = queue->head;
		
		while (temp != NULL && temp->my_tid != find_tid)
		{
			temp = temp->my_next;
		}
		
		return temp;
	}
}

int remove_from_queue( the_queue* queue, Tid find_tid )
{
	if (queue->head == NULL)
		return 0;
	
	else if (queue->head->my_tid == find_tid)
	{
		queue->head = NULL;
		return 1;
	}
	
	else
	{
		ThrdCtlBlk* temp = queue->head;
		
		while (temp != NULL && temp->my_tid != find_tid)
		{
			temp = temp->my_next;
		}
		
		if (temp == NULL)
			return 0;
		
		else
		{
			ThrdCtlBlk* before = temp->my_prev;
			ThrdCtlBlk* after = temp->my_next;
			
			before->my_next = after;
			after->my_prev = before;
			return 1;
		}
	}
}