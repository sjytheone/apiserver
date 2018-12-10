#ifndef __XLOG_H__
#define __XLOG_H__


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

#if (defined(_ANDROID_TR2_) || defined(_ANDROID_))
#include <jni.h>
#include <android/log.h> 
#endif


void XLOGD(const char* format, ...);
void XLOGI(const char* format, ...);
void XLOGW(const char* format, ...);
void XLOGE(const char* format, ...);
#endif

