/**************************************************************************************************\
* 
* Work dispatcher
* 
\**************************************************************************************************/


#define MAX_WORK_ITEMS      32
#define MAX_THREADS         10


void dispatcherClearWorkItems       ();
void dispatcherPushWorkItem         (void (*workfunction)(int worker, int workItem, void* params), void* workItemParameters);
void dispatcherStartWork            (int numberOfThreads, void (*finishFunction)(void* params), void* finishParameters);
int  dispatcherGetTotalWorkItems    ();
void mutexLock                      ();
void mutexUnlock                    ();
