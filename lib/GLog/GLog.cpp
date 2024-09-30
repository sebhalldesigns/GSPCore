#include <GLog/GLog.hpp>

#include <cstdio>
#include <ctime>
#include <cstdarg>

void GLog::Info(const char* message, ...) 
{
    time_t currentTime = time(0);
    struct tm* timeInfo = localtime(&currentTime);

    #ifndef EMSCRIPTEN
        printf("[%02d:%02d:%02d] ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        printf("\033[0;94mINFO\033[0m ");
    #else
        printf("[%02d:%02d:%02d] INFO ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    #endif

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("\n");
}

void GLog::Warning(const char* message, ...) {
    time_t currentTime = time(0);
    struct tm* timeInfo = localtime(&currentTime);

    #ifndef EMSCRIPTEN
        printf("[%02d:%02d:%02d] ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        printf("\033[0;93mWARNING\033[0m ");
     #else
        printf("[%02d:%02d:%02d] WARNING ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    #endif

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("\n");
}

void GLog::Error(const char* message, ...) {
    time_t currentTime = time(0);
    struct tm* timeInfo = localtime(&currentTime);

    #ifndef EMSCRIPTEN
        printf("[%02d:%02d:%02d] ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
        printf("\033[0;91mERROR\033[0m ");
    #else
        printf("[%02d:%02d:%02d] ERROR ", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    #endif

    va_list args;
    va_start(args, message);
    vprintf(message, args);
    va_end(args);

    printf("\n");
}