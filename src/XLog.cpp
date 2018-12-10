#include "XLog.h"
#ifdef _ARM_LINUX_
#include "HAL/Logger.h"
#endif
#include <string.h>
#include <string>

static int sXLogLevel = 0;
static bool sShowXLog = true;


typedef enum
{
    AVSDK_LOG_UNKNOWN = 0,
    AVSDK_LOG_DEFAULT,
    AVSDK_LOG_VERBOSE,
    AVSDK_LOG_DEBUG,
    AVSDK_LOG_INFO,
    AVSDK_LOG_WARN,
    AVSDK_LOG_ERROR,
    AVSDK_LOG_FATAL,
    AVSDK_LOG_SILENT,
} LogPriority_avsdk;

void WriteLog(LogPriority_avsdk nType, const char* strMsg)
{
    if (!sShowXLog)
    {
        return;
    }

    if (sXLogLevel <= nType)
    {
        char chTag[20] = {0};
        switch(nType)
        {
        case AVSDK_LOG_DEBUG:
			strcpy(chTag, "[DEBUG]");
            break;
        case AVSDK_LOG_INFO:
			strcpy(chTag, "[INFO]");
            break;
        case AVSDK_LOG_WARN:
			strcpy(chTag, "[WARN]");
            break;
        case AVSDK_LOG_ERROR:
			strcpy(chTag, "[ERROR]");
            break;
        }

#ifdef _ARM_LINUX_
        __os_log_write(nType, chTag, strMsg);
#elif defined(_WIN32)
		OutputDebugStringA(strMsg);
#elif (defined(_ANDROID_TR2_) || defined(_ANDROID_))
		__android_log_print(ANDROID_LOG_DEBUG, chTag, "%s",strMsg);
#endif
    }
}

void XLOGI(const char* format, ...)
{
    va_list vp;
    va_start(vp, format);
    char buffer[256] = {0};
    vsnprintf(buffer, sizeof(buffer), format, vp);
    WriteLog(AVSDK_LOG_INFO, buffer);
    va_end(vp);
}

void XLOGD(const char* format, ...)
{
    va_list vp;
    va_start(vp, format);
    char buffer[256] = {0};
    vsnprintf(buffer, sizeof(buffer), format, vp);
    WriteLog(AVSDK_LOG_DEBUG, buffer);
    va_end(vp);
}

void XLOGW(const char* format, ...)
{
    va_list vp;
    va_start(vp, format);
    char buffer[256] = {0};
    vsnprintf(buffer, sizeof(buffer), format, vp);
    WriteLog(AVSDK_LOG_WARN, buffer);
    va_end(vp);
}

void XLOGE(const char* format, ...)
{
    va_list vp;
    va_start(vp, format);
    char buffer[256] = {0};
    vsnprintf(buffer, sizeof(buffer), format, vp);
    WriteLog(AVSDK_LOG_ERROR, buffer);
    va_end(vp);
}
