#ifndef LOG_H
#define LOG_H

typedef enum
{
    LOGLEVEL_DEBUG  =0,
    LOGLEVEL_INFO   =1,
    LOGLEVEL_WARNING=2,
    LOGLEVEL_ERROR  =3,
    LOGLEVEL_FATAL  =4
} LogLevel_t;

void setLogLevel(LogLevel_t logLevel);
void logDebug   (char* message, ...);
void logInfo    (char* message, ...);
void logWarning (char* message, ...);
void logError   (char* message, ...);
void logFatal   (char* message, ...);
void logReport  (char* message, ...);

#endif