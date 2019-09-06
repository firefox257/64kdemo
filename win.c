/* An example of the minimal Win32 & OpenGL program.  It only works in
   16 bit color modes or higher (since it doesn't create a
   palette). */


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

const char * appName = "demo";

#define NODEBUG

#ifdef DEBUG
	#include <stdio.h>
	FILE * logFile;

	void logBool(bool b)
	{
		if(b) fwrite("true", 1, 4, logFile);
		else fwrite("false", 1, 5, logFile);
	}
	void logChar(char c)
	{
		fwrite(&c, 1, 1, logFile);
	}
	void logUChar(uchar c)
	{
		fwrite(&c, 1, 1, logFile);
	}
	
	void logShort(short d)
	{
		char b[20];
		int s = sprintf(b, "%d", d);
		fwrite(b, 1, s, logFile);	
	}
	void logUShort(ushort d)
	{
		char b[20];
		int s = sprintf(b, "%u", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logInt(int d)
	{
		char b[20];
		int s = sprintf(b, "%d", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logUInt(uint d)
	{
		char b[20];
		int s = sprintf(b, "%u", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logULong(ulong d)
	{
		char b[20];
		int s = sprintf(b, "%ll", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logLlong(llong d)
	{
		char b[20];
		int s = sprintf(b, "%ll", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logFloat(float d)
	{
		char b[20];
		int s = sprintf(b, "%f", d);
		fwrite(b, 1, s, logFile);	
	}
	
	void logDouble(double d)
	{
		char b[20];
		int s = sprintf(b, "%f", d);
		fwrite(b, 1, s, logFile);	
	}
	
	
	void logStr(char * msg)
	{
		int s = 0;
		while(msg[s] != 0) s++;
		fwrite(msg, 1, s, logFile);
		fflush(logFile);
	}
	
#else 
	#define logBool(msg)
	#define logChar(msg)
	#define logUChar(msg)
	#define logShort(msg)
	#define logUShort(msg)
	#define logInt(msg)
	#define logUInt(msg)
	#define logLlong(msg)
	#define logULong(msg)
	#define logFloat(msg)
	#define logDouble(msg)
	#define logStr(msg)  
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


void display()
{
    /* rotate a triangle around */
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2i(0,  1);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2i(-1, -1);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2i(1, -1);
    glEnd();
    glFlush();
}


LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
	
    static PAINTSTRUCT ps;
	
    switch(uMsg) {
		case WM_PAINT:
			display();
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		return 0;

		case WM_SIZE:
			glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
			PostMessage(hWnd, WM_PAINT, 0, 0);
		return 0;

		case WM_CHAR:
		switch (wParam) {
		case 27:			/* ESC key */
			PostQuitMessage(0);
			break;
		}
		return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

HWND CreateOpenGLWindow(int x, int y, int width, int height, 
		   BYTE type, DWORD flags)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    /* only register the window class once - use hInstance as a flag. */
    if (!hInstance) 
	{
		hInstance = GetModuleHandle(NULL);
		wc.style         = CS_OWNDC;
		wc.lpfnWndProc   = (WNDPROC)WindowProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = hInstance;
		wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName  = NULL;
		wc.lpszClassName = appName;

		if (!RegisterClass(&wc)) 
		{
			logStr("Cannot register window class.");
			return NULL;
		}
    }

    hWnd = CreateWindow(appName, appName, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, width, height, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) 
	{
			   logStr("CreateWindow() failed:  Cannot create a window.\r\n");
		return NULL;
    }

    hDC = GetDC(hWnd);

    /* there is no guarantee that the contents of the stack that become
       the pfd are zeroed, therefore _make sure_ to clear these bits. */
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | flags;
    pfd.iPixelType   = type;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) 
	{
		logStr("Cannot find a suitable pixel format.\r\n");
		return 0;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) 
	{
		logStr("Cannot set format specified.\r\n");
		return 0;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hDC, hWnd);

    return hWnd;
}    

int APIENTRY WinMain(HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow)
{
	
	#ifdef DEBUG
		logFile = fopen("log.txt", "a");
	#endif
	logStr("#################start##############################\r\n");
	logDouble(123.123); logStr("\r\n");
    HDC hDC;				/* device context */
    HGLRC hRC;				/* opengl context */
    HWND  hWnd;				/* window */
    MSG   msg;				/* message */

    hWnd = CreateOpenGLWindow(0, 0, 256, 256, PFD_TYPE_RGBA, 0);
    if (hWnd == NULL)
	return 1;

    hDC = GetDC(hWnd);
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    ShowWindow(hWnd, nCmdShow);
	while(true)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//while(GetMessage(&msg, hWnd, 0, 0)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if(msg.message == WM_QUIT)
        break;
	
		Sleep(33);
	}

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hDC, hWnd);
    wglDeleteContext(hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
}