// This header file is meant to use the Visual C++ compiler
// to compile code for Unix. Thus, there are a number of
// functions and data types that need to be defined.

#ifndef __UNIX_FUNC_PROT__
#define __UNIX_FUNC_PROT__

#include "stdafx.h"

// CONSTANTS
#define SIGALRM 0
#define SIGKILL 0
#define WIFEXITED(A) 1

// DATA TYPES
typedef unsigned long pid_t;
typedef int pixel;
typedef int pixval;

/*struct option {
	const char *name;
	int has_arg;
	int *flag;
	int val;
};

char* optarg = NULL;
int optind = 0;*/

// FUNCTIONS
pid_t fork(void) { return 0; }
unsigned int alarm(unsigned int seconds)  { return 0; }
pid_t waitpid(pid_t pid, int *status, int options)  { return 0; }
void (*signal(int signum, void (*handler)(int)))(int) { return 0;}
int kill(pid_t pid, int sig)  { return 0; }

//int getopt_long_only (int, char**, const char*,	struct option*, int *);

// lib PPM functions
/*pixel** ppm_allocarray( int cols, int rows );
pixel* ppm_allocrow( int cols );
void ppm_freearray( pixel** pixels, int rows );
void pbm_freerow( pixel* pixelrow );
void ppm_readppminit( FILE* fp, int* colsP, int* rowsP, pixval* maxvalP, int* formatP );
void ppm_readppmrow( FILE* fp, pixel* pixelrow, int cols, pixval maxval, int format );
pixel** ppm_readppm( FILE* fp, int* colsP, int* rowsP, pixval* maxvalP );
void ppm_writeppminit( FILE* fp, int cols, int rows, pixval maxval, int forceplain );
void ppm_writeppmrow( FILE* fp, pixel* pixelrow, int cols, pixval maxval, int forceplain );
void ppm_writeppm( FILE* fp, pixel** pixels, int cols, int rows, pixval maxval, int forceplain );*/

#endif // no WIN32
#endif //__UNIX_FUNC_PROT__
