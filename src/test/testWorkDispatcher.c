/**************************************************************************************************\
* 
*  testWorkDispatcher.c
*  Test the workdispatcher functions
* 
\**************************************************************************************************/
#include <stdio.h>
#include <unistd.h>

#include "testframe.h"
#include "workDispatcher.h"

/**************************************************************************************************\
* 
* Test the work dispatcher
* 
\**************************************************************************************************/
char* workItems[]=
{
    "Item 1",
    "Item 2",
    "Item 3"
};

char* finalItem="Final item";

/**************************************************************************************************\
* 
* Worker function 
* 
\**************************************************************************************************/
void workFunction(int worker, int workItem, void* params)
{
    char* item;
    item=(char*) params;
    printf("Workfunction: Worker # %d, Work Item# %d, data: %s\n", worker, workItem, item);
    sleep(1);
    fflush(stdout);
}

/**************************************************************************************************\
* 
* Finish function 
* 
\**************************************************************************************************/
void finalFunction(void* params)
{
    char* item;
    item=(char*) params;
    printf("Final function %s\n", item);
    assertStringEquals(finalItem, item);
    fflush(stdout);
}

/**************************************************************************************************\
* 
* Test 
* 
\**************************************************************************************************/
void testWorkDispatcherDispatch(void)
{
    testStart("Dispatching");

    dispatcherClearWorkItems();
    dispatcherPushWorkItem(workFunction, (void*)workItems[0]);
    dispatcherPushWorkItem(workFunction, (void*)workItems[1]);
    dispatcherPushWorkItem(workFunction, (void*)workItems[2]);
    dispatcherStartWork(3, finalFunction, (void*)finalItem);
    testWrapUp();
} 


/**************************************************************************************************\
* 
* Test main function
* 
\**************************************************************************************************/
void testWorkDispatcher()
{
    moduleTestStart("Work Dispatcher");
    testWorkDispatcherDispatch();
    sleep(3);           // Make sure workers are finished
    moduleTestWrapUp();
}