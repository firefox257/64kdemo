
#ifndef DEF_C
#define DEF_C

#include <windows.h>			/* must include this before GL/gl.h */
#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */
#include <stdio.h>


#define bool unsigned char
#define true 1
#define false 0
#define null 0



#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define llong long long
#define ulong unsigned long long


#ifdef DEBUG
	#include <stdio.h>
	FILE * logFile;
	
	#define logStr(f) fprintf(logFile, f); fflush(logFile);
	#define logStrf(f, ...) fprintf(logFile, f, __VA_ARGS__); fflush(logFile);
	
#else 
	
	#define logStr(f) 
	#define logStrf(f, ...) 
#endif


ulong milli()
{
	static FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((ulong)ft.dwHighDateTime) << 32) | ft.dwLowDateTime)/10000;
}
ulong micro()
{
	static FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return ((((ulong)ft.dwHighDateTime) << 32) | ft.dwLowDateTime)/10;
}


#endif
