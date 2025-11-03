/**************************************************************************************************\
* 
* Generic Work dispatcher: Dispatches work items over multiple threads. Each thread picks the 
* work item of the stack and processes it by calling a custom work function.
* After all work items have been processed, a custom finishing function is executed.
* 
\**************************************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "log.h"
#include <unistd.h>

#include "workDispatcher.h"


// This struct contains attributes of the Worker thread
typedef struct
{
    int                 threadIndex;
    pthread_t           threadId;
    bool                finished;
} WorkerParameters;

// This defines a work item for the workers
typedef struct
{
    void (*workfunction)(int worker, int workItem, void* params);   // Custom function that executes a work item
    void* workItemParameters;                                       // Custom work item data
} WorkItem;

// This defines the parameters applying to all items
typedef struct
{
    void (*finishFunction)(void* params);                           // Custom finishing function
    void *finishParameters;                                         // Custom finishing function data
} WorkParameters;

WorkerParameters        workerParams[MAX_THREADS];
WorkItem                dispatcherWorkItems[MAX_WORK_ITEMS];
WorkParameters          dispatchWorkParameters;
int                     dispatcherNumberOfWorkItems         =0;
int                     dispatcherInitialNumberOfWorkItems  =-1;
int                     threadStarted                       =0;
int                     threadsRunning                      =0;
pthread_mutex_t         workMutex;
long                    workStartTime;
long                    workEndTime;

/**************************************************************************************************\
* FUNCTIONS
\**************************************************************************************************/
/**************************************************************************************************\
* 
* Clear the list of work items
* 
\**************************************************************************************************/
void dispatcherClearWorkItems()
{
    pthread_mutex_lock(&workMutex);
    if (!threadStarted)
    {
        dispatcherNumberOfWorkItems         =0;
        dispatcherInitialNumberOfWorkItems  =0;
    }
    else
    {
        logError("Error adding workitem: threads have already been started");
    }
    pthread_mutex_unlock(&workMutex);
}

/**************************************************************************************************\
* 
* Pushes a work item on the stack
* 
\**************************************************************************************************/
void dispatcherPushWorkItem        (void (*workfunction)(int worker, int workItem, void* params), void* workItemParameters)
{
    pthread_mutex_lock(&workMutex);
    if (!threadStarted)
    {
        dispatcherWorkItems[dispatcherNumberOfWorkItems].workfunction       =workfunction;
        dispatcherWorkItems[dispatcherNumberOfWorkItems].workItemParameters =workItemParameters;
        dispatcherNumberOfWorkItems++;
        dispatcherInitialNumberOfWorkItems=dispatcherNumberOfWorkItems;
    }
    else
    {
        logError("Error adding workitem: threads have already been started");
    }
    pthread_mutex_unlock(&workMutex);
}



/**************************************************************************************************\
* 
* Thread function. Executes the next available work item at the end of the list
* 
\**************************************************************************************************/
void* workThreadFunction(void *params)
{
    WorkerParameters*       workerParams;
    WorkItem*               item;

    int                     done;
    int                     workItem;
    int                     lastManStanding;
    long                    timeDiff;

    workerParams    =(WorkerParameters*)params; 
    logDebug("Started thread %d", workerParams->threadIndex);

    pthread_mutex_lock(&workMutex);   
    threadsRunning++;
    pthread_mutex_unlock(&workMutex);   

    done=0;
    while (!done)
    {

        // Pop the next item from the stack
        pthread_mutex_lock(&workMutex);
        if (dispatcherNumberOfWorkItems>0)
        {
            dispatcherNumberOfWorkItems--;
            workItem    =dispatcherNumberOfWorkItems;
            item        =&dispatcherWorkItems[workItem];
        }
        else
        {
            done=1;
        }
        pthread_mutex_unlock(&workMutex);

        if (!done)
        {
            // Execute the work
            item->workfunction(workerParams->threadIndex, workItem, item->workItemParameters);
        }
        else
        {
            pthread_mutex_lock(&workMutex);   
            threadsRunning--;
            if (threadsRunning==0)
            {
                lastManStanding=1;
            }
            else
            {
                lastManStanding=0;
            }
            pthread_mutex_unlock(&workMutex);   

            // If this is the last thread running, execute the final function
            if (lastManStanding)
            {
                logDebug("Last man standing: %d", workerParams->threadIndex);
                dispatchWorkParameters.finishFunction(dispatchWorkParameters.finishParameters);
                workEndTime =time(NULL);
                timeDiff    =workEndTime-workStartTime;
                logDebug("Time elapsed: %ld'%02ld\"", timeDiff/60, timeDiff%60);
            }

            pthread_mutex_lock(&workMutex);
            workerParams->finished=true; // signal work done!
            pthread_mutex_unlock(&workMutex);   

            logDebug("Worker %d finished", workerParams->threadIndex);
            fflush(stdout);
        }
    }
    return NULL;    
} 

/**************************************************************************************************\
* 
* This function starts indicted number of worker threads. 
* 
\**************************************************************************************************/
void dispatcherStartWork(int numberOfThreads, void (*finishFunction)(void* params), void* finishParameters, bool waitUntilFinished)
{
    int             i;

    pthread_mutex_lock(&workMutex);
    if (!threadStarted)
    {
        dispatchWorkParameters.finishFunction   =finishFunction;
        dispatchWorkParameters.finishParameters =finishParameters;

        workStartTime =time(NULL);
        // create the threads
        dispatcherInitialNumberOfWorkItems=dispatcherNumberOfWorkItems;
        i=0;
        while (i<numberOfThreads)
        {
            workerParams[i].threadIndex         =i;
            workerParams[i].finished            =false;
            pthread_create(&(workerParams[i].threadId), 
                           NULL, 
                           workThreadFunction, 
                           (void *)(workerParams+i)); 
            i++;
        }
    }
    else
    {
        logWarning("Workers are already running; wait till finished");
        fflush(stdout);
    }
    pthread_mutex_unlock(&workMutex);

    // There are two exit methods:
    // - Just wait till all work is done
    // - Thread exit, to prevent programm to finish
    if (waitUntilFinished)
    {
        bool exit=false;
        while (!exit)
        {

            pthread_mutex_lock(&workMutex);
            exit=true;
            for (int j=0;j<numberOfThreads && exit;j++)
            {
                if (!workerParams[j].finished)
                {
                    exit=false;
                }
            }
            pthread_mutex_unlock(&workMutex);
            sleep(1);
        }
    }
    else
    {
        pthread_exit(NULL);  
    }
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
int dispatcherGetTotalWorkItems()
{
    return dispatcherInitialNumberOfWorkItems;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void mutexLock()
{
    pthread_mutex_lock(&workMutex);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void mutexUnlock()
{
    pthread_mutex_unlock(&workMutex);
}
