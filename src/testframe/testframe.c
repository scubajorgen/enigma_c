/**************************************************************************************************\
* 
* testframe.c
* Functions that facilitate testing
* 
\**************************************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#include "log.h"
#include "testframe.h"
#include "toolbox.h"

char    sessionName[TEST_NAME_LENGTH+1];
char    moduleName[TEST_NAME_LENGTH+1];
char    testName[TEST_NAME_LENGTH+1];
int     testsPassed;
int     testsFailed;
int     testsExecuted;
int     assertionsExecuted;
bool    passed;

int     testsPassedTotal;
int     testsFailedTotal;
int     testsExecutedTotal;
int     assertionsExecutedTotal;

struct  timeval  testStopTime, testStartTime;

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void testSessionStart       (char* name)
{
    strncpy(sessionName, name, TEST_NAME_LENGTH);
    logInfo("###############################################################################");
    logInfo("# TEST SESSION STARTED: %s", sessionName);
    logInfo("###############################################################################");
    testsExecutedTotal      =0;
    testsPassedTotal        =0;
    testsFailedTotal        =0;
    assertionsExecutedTotal =0;
    gettimeofday(&testStartTime, NULL);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void testSessionWrapUp()
{
    logInfo("###############################################################################");
    logInfo("# Tests Executed     : %d", testsExecutedTotal);
    logInfo("# Tests Passed       : %d", testsPassedTotal);
    logInfo("# Tests Failed       : %d", testsFailedTotal);
    logInfo("# Assertions executed: %d", assertionsExecutedTotal);
    gettimeofday(&testStopTime, NULL);
    float diff=timeDifference(testStartTime, testStopTime);
    logInfo("# Execution time     : %.3f seconds", diff/1000);
    logInfo("###############################################################################");
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void moduleTestStart(char* name)
{
    strncpy(moduleName, name, TEST_NAME_LENGTH);
    testsPassed         =0;
    testsExecuted       =0;
    testsFailed         =0;
    assertionsExecuted  =0;
    logInfo("###############################################################################");
    logInfo("Testing Module %s", moduleName);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void moduleTestWrapUp       ()
{
    logInfo("Test result %s ", moduleName);
    logInfo("executed: %3d, passed: %3d, failed: %3d, assertions: %3d", testsExecuted, testsPassed, testsFailed, assertionsExecuted);
    logInfo("###############################################################################");
    if (testsFailed>0)
    {
        exit(0);
    }
    testsExecutedTotal      +=testsExecuted;
    testsPassedTotal        +=testsPassed;
    testsFailedTotal        +=testsFailed;
    assertionsExecutedTotal +=assertionsExecuted;
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void testStart              (char* name)
{
    strncpy(testName, name, TEST_NAME_LENGTH);
    testsExecuted++;
    passed=true;
    logInfo("-------------------------------------------------------------------------------");
    logInfo("Testing %s - %-20s", moduleName, testName);
}

/**************************************************************************************************\
* 
* 
* 
\**************************************************************************************************/
void testWrapUp             ()
{
    if (passed)
    {
        testsPassed++;
    }
    else
    {
        testsFailed++;
    }
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertStringEquals(char* expected, char* result)
{
    if (strcmp(expected, result)==0)
    {
        logDebug("Test %s - %-20s (%03d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%03d): FAILED! Expected %s, result was %s", moduleName, testName, assertionsExecuted, expected, result);
        passed=false;
    }
    assertionsExecuted++;
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertIsNull(void* result)
{
    if (result==NULL)
    {
        logDebug("Test %s - %-20s (%03d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%03d): FAILED! Expected NULL, result was not NULL", moduleName, testName, assertionsExecuted);
        passed=false;
    }
    assertionsExecuted++;
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertNotNull(void* result)
{
    if (result!=NULL)
    {
        logDebug("Test %s - %-20s (%03d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%03d): FAILED! Expected not NULL, result was NULL", moduleName, testName, assertionsExecuted);
        passed=false;
    }
    assertionsExecuted++;
}

/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertIntEquals(int expected, int result)
{
    if (expected==result)
    {
        logDebug("Test %s - %-20s (%03d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%03d): FAILED! Expected %d, result was %d", moduleName, testName, assertionsExecuted, expected, result);
        passed=false;
    }
    assertionsExecuted++;
}



/**************************************************************************************************\
* 
* Helper function
* 
\**************************************************************************************************/
void assertLongNotEquals(long notExpected, long result)
{
    if (notExpected!=result)
    {
        logDebug("Test %s - %-20s (%3d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%3d): FAILED! Not expected %ld, result was %ld", moduleName, testName, assertionsExecuted, notExpected, result);
        passed=false;
    }
    assertionsExecuted++;
}

/**************************************************************************************************\
* 
* Helper funcion
* 
\**************************************************************************************************/
void assertFloatEquals(float expected, float result)
{
    if (floorf(expected*10000.0)==floorf(result*10000.0))
    {
        logDebug("Test %s - %-20s (%3d): Passed!", moduleName, testName, assertionsExecuted);
    }
    else
    {
        logError("Test %s - %-20s (%3d): FAILED! Expected %f, result was %f", moduleName, testName, assertionsExecuted, expected, result);
        passed=false;
    }
    assertionsExecuted++;
}
