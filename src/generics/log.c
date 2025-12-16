
/**************************************************************************************************\
*
* log.c
*
* Simple logger, to vary logging output according to log level 
*
\**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

/**************************************************************************************************\
* VARIABLES
\**************************************************************************************************/

LogLevel_t theLogLevel=LOGLEVEL_INFO;

void printLogMessage(char* message, va_list args, char* level)
{
    printf("%-7s ", level);
    vprintf(message, args);
    printf("\n");
}

/**************************************************************************************************\
*
* Sets the log level
*
\**************************************************************************************************/
void setLogLevel(LogLevel_t logLevel)
{
    theLogLevel=logLevel;
}

/**************************************************************************************************\
*
* Debug level logging
*
\**************************************************************************************************/
void logDebug(char* message, ...)
{
    if (theLogLevel<=LOGLEVEL_DEBUG)
    {
        va_list args;
        va_start(args, message);
        printLogMessage(message, args, "DEBUG");
    }
}

/**************************************************************************************************\
*
* Info level logging
*
\**************************************************************************************************/
void logInfo(char* message, ...)
{
    if (theLogLevel<=LOGLEVEL_INFO)
    {
        va_list args;
        va_start(args, message);
        printLogMessage(message, args, "INFO");
    }
}

/**************************************************************************************************\
*
* Warning level logging
*
\**************************************************************************************************/
void logWarning(char* message, ...)
{
    if (theLogLevel<=LOGLEVEL_WARNING)
    {
        va_list args;
        va_start(args, message);
        printLogMessage(message, args, "WARNING");
    }
}

/**************************************************************************************************\
*
* Info level logging
*
\**************************************************************************************************/
void logError(char* message, ...)
{
    if (theLogLevel<=LOGLEVEL_ERROR)
    {
        va_list args;
        va_start(args, message);
        printLogMessage(message, args, "ERROR");
    }
}

/**************************************************************************************************\
*
* Fatal level logging and say goodbye
*
\**************************************************************************************************/
void logFatal(char* message, ...)
{
    va_list args;
    va_start(args, message);
    printLogMessage(message, args, "FATAL");
    exit(0);
}

/**************************************************************************************************\
*
* Reporting, always show
*
\**************************************************************************************************/
void logReport(char* message, ...)
{
    va_list args;
    va_start(args, message);
    printLogMessage(message, args, "REPORT");
}
