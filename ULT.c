#include <assert.h>

#include <stdio.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <stdlib.h>

#include "readyQueue.h"

//added variables
Tid first_called;
the_queue* my_queue;

Tid 
ULT_CreateThread(void (*fn)(void *), void *parg)
{
	if (first_called == 0)
	{
		int success = first_call_func();
		
		if (success != 1)
			return success;
		
	}
  
	ThrdCtlBlk* new_thread = malloc(sizeof(ThrdCtlBlk));
	if(new_thread == NULL)
		return ULT_NOMEMORY;
	else
	{
		int count = 1;
		ThrdCtlBlk* temp = my_queue->head;
		if(temp->my_tid == 0)
			temp = temp->my_next;
		while(temp != NULL && temp->my_tid <= count)
		{
			count++;
			temp = temp->my_next;
		}
		
		new_thread->my_tid = count;
		add_to_queue(my_queue, new_thread);
		getcontext(&(new_thread->my_context));
		new_thread->my_context->uc_mcontext->gregs[REG_EIP] = &stub;
		
		//create stack
		//change stack pointers
		//push arguments onto stack
		//return tid
	}
	
	
	return ULT_FAILED;
}



Tid ULT_Yield(Tid wantTid)
{
	if (first_called == 0)
	{
		int success = first_call_func();
		
		if (success != 1)
			return success;
	}
	
	if (wantTid < -2 || wantTid >= ULT_MAX_THREADS)
		return ULT_INVALID;
	
	if (my_queue->head == NULL)
	{
		if (wantTid == ULT_ANY || wantTid == ULT_SELF)
			return ULT_NONE;
			
		else if (wantTid == 0)
			return 0;
	}
	
	if (wantTid == ULT_SELF)
	{
		int flag = 0;
	
		ThrdCtlBlk* create_tcb = malloc(sizeof(ThrdCtlBlk));
		getcontext( &(create_tcb->my_context) );
		
		if (flag == 0)
		{
			flag++;
			add_to_queue( my_queue, create_tcb );
			ThrdCtlBlk* find_tcb = find_in_queue( my_queue, create_tcb->my_tid );
			my_queue->running = find_tcb;
			remove_from_queue( my_queue, find_tcb->my_tid );
			setcontext( &(find_tcb->my_context) );
		}
	
		return my_queue->running->my_tid;
	}
	
	else if (wantTid == ULT_ANY)
	{
		add_to_queue( my_queue, my_queue->running );
		my_queue->running = my_queue->head;
		remove_from_queue( my_queue, my_queue->running->my_tid );
		return my_queue->running->my_tid;
	}
	
	else
	{
		add_to_queue( my_queue, my_queue->running );
		ThrdCtlBlk* find_tcb = find_in_queue( my_queue, wantTid );
		
		if (find_tcb == NULL)
		{
			remove_from_queue( my_queue, my_queue->running->my_tid );
			return ULT_INVALID;
		}
		
		else
		{
			my_queue->running = find_tcb;
			remove_from_queue( my_queue, find_tcb->my_tid );
			return my_queue->running->my_tid;
		}
	}
	
	return ULT_FAILED;

}


Tid ULT_DestroyThread(Tid tid)
{
	if (first_called == 0)
	{
		int success = first_call_func();
		
		if (success != 1)
			return success;
		
	}
	
	return ULT_FAILED;
}

//returns 1 on success, anything else is Tid error
Tid first_call_func()
{
	my_queue = malloc( sizeof(the_queue) );
		
	//couldn't even allocate queue
	if (my_queue == NULL)
		return ULT_NOMEMORY;
	
	int try_allocate = init_queue( my_queue );
	
	//for some reason, memory is already full from the start
	if (try_allocate == ULT_NOMEMORY)
		return ULT_NOMEMORY;
	
	first_called = 1;
	return 1;
}

void stub(void (*root)(void *), void *arg) 
{
	// thread starts here 
	Tid ret; 
	root(arg); // call root function 
	ret = ULT_DestroyThread(ULT_SELF) 
	assert(ret == ULT_NONE); // we should only get here if we are the last thread. 
	exit(0); // all threads are done, so process should exit

}

