/**************************************************************************************************\
* 
*  testWorkDispatcher.c
*  Test the workdispatcher functions
* 
\**************************************************************************************************/
#include <stdio.h>
#include <unistd.h>

#include "testframe.h"
#include "log.h"
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
    logInfo("Workfunction: Worker # %d, Work Item# %d, data: %s", worker, workItem, item);
    sleep(1);
    logInfo("Workfunction: Worker # %d done", worker);
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
    logInfo("Final function %s", item);
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
    dispatcherStartWork(3, finalFunction, (void*)finalItem, true);  // start, wait till work finished
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
    moduleTestWrapUp();
}