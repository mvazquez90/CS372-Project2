#include <assert.h>
#include <stdio.h>

/* We want the extra information from these definitions */
#ifndef __USE_GNU
#define __USE_GNU
#endif /* __USE_GNU */
#include <stdlib.h>

#include "ULT.h"

struct ThrdCtlBlk *Head = NULL;
Tid runningId = 0;


//-------------------DELETE-------------------
int main()
{
  Tid ret;
  Tid ret2;

  printf("Starting tests...\n");

  /*
   * Initial thread yields
   */
  ret = ULT_Yield(ULT_SELF);
  assert(ret==ULT_NONE);
  printf("Initial thread returns from Yield(SELF)\n");
  ret = ULT_Yield(0); /* See ULT.h -- initial thread must be Tid 0 */
  assert(ULT_isOKRet(ret));
  printf("Initial thread returns from Yield(0)\n");
  ret = ULT_Yield(ULT_ANY);
  assert(ret == ULT_NONE);
  printf("Initial thread returns from Yield(ANY)\n");
  ret = ULT_Yield(0xDEADBEEF);
  assert(ret == ULT_INVALID);
  printf("Initial thread returns from Yield(INVALID)\n");
  ret = ULT_Yield(16);
  assert(ret == ULT_INVALID);
  printf("Initial thread returns from Yield(INVALID2)\n");
  
  return 0;
}

//--------------------------------------------

Tid ULT_CreateThread(void (*fn)(void *), void *parg)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}



Tid ULT_Yield(Tid wantTid)
{
  // if wantTid is out of the range of acceptable Tids
  // then return ULT_INVALID. 
  if(wantTid < -2 || wantTid > ULT_MAX_THREADS)
	return ULT_INVALID;
  
  //if wantTid is 0 then return 0;
  if(wantTid == 0)
	return 0;
	
  // if no threads have been created yet or 
  // all threads have been destroyed/
  else if(Head == NULL)
  {
	if(wantTid == ULT_ANY || wantTid == ULT_SELF)
		return ULT_NONE;
	else
		return ULT_INVALID;
  }
  
  else if(wantTid == ULT_ANY)
  {
	Tid temp = Head->id;
	runningId = temp;
	return temp;
  }
  
  else if(wantTid == ULT_SELF)
  {
	return runningId;
  }
  
  else
  {
	ThrdCtlBlk *temp;
	temp = Head;
	while(temp != NULL)
	{
		if(temp->id == wantTid)
		{
			Tid newRunning = temp->id;
			runningId = newRunning;
			return runningId;
		}
		temp = temp->next;
	}
	return ULT_INVALID;
  }

}


Tid ULT_DestroyThread(Tid tid)
{
  assert(0); /* TBD */
  return ULT_FAILED;
}





