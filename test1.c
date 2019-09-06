
#define bool unsigned char
#define true 1
#define false 0
#define null 0

#define DEBUG

#ifdef DEBUG
	#include <stdio.h>
	FILE * logFile;

	void logStr(char * msg)
	{
		int s = 0;
		while(msg[s] != 0) s++;
		fwrite(msg, 1, s, logFile);
		fflush(logFile);
	}
#else 
	#define logStr(msg)  
#endif

struct p3
{
	double x, y, z;
};

struct p3a
{
	int s;
	struct p3 * d;
	
};

void p3aInit(struct p3a * at, int s)
{
	at->s = s;
	at->d = (struct p3 *)malloc(sizeof(struct p3) * s);
}
void p3aFree(struct p3a*at)
{
	at->s = 0;
	free(at->d);
}


int main()
{
	#ifdef DEBUG
		logFile = fopen("log.txt", "a");
	#endif
	
	
	
	logStr("hi there");
	
	
	return 0;
}




