#ifndef _BASIC_UTILS_H_
#define _BASIC_UTILS_H_

#include <iostream>
#include <string>
#include <assert.h>

#ifdef WIN32
#include <float.h>
#define isnan  _isnan
#define finite _finite
#define NORM2(X, Y) sqrt((X)*(X) + (Y)*(Y))
#define round(X) ( ((X) < 0) ? -(int)(0.5 - (X)) : (int)((X) + 0.5) )
#define strcasecmp(A, B) _stricmp(A, B)
#define FILE_SEP '\\'
#else // ie, Linux OS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NORM2(X, Y) std::sqrt((X)*(X) + (Y)*(Y))
#define FILE_SEP '/'
#endif //WIN32

#ifndef INFINITY
#define INFINITY	1000000
#endif

#define ShowStatus(M) std::cout << std::string(M) << std::endl

#define ShowStatus1(M,A) std::cout << std::string(M) << \
	" " << std::string(A) << std::endl

#define ShowStatus2(M,N,S) std::cout << std::string(M) << " " \
	<< N << " " << std::string(S) << std::endl

#define ShowMsg(M) std::cout << std::endl << std::string(M) << std::endl
#define ShowUsage(M) std::cerr << "Usage: " << std::string(M) << std::endl

#define ShowError(M) std::cerr << "Error: " << std::string(M) << std::endl
#define ShowError1(M,A) std::cerr << "Error: " << std::string(M) \
	<< " " << std::string(A) << std::endl

#define ShowErrorAndNumber(M,N) std::cerr << "Error: " << std::string(M) \
	<< " " << N << std::endl

#define ShowOpenFileError(F) std::cerr << "Error: Cannot open file '" \
	<< std::string(F) << "'" << std::endl

#define ShowCreateFileError(F) std::cerr << "Error: Cannot create file '" \
	<< std::string(F) << "'" << std::endl

#define MIN(X, Y) ( (X) <= (Y) ? (X):(Y) )
#define MAX(X, Y) ( (X) >= (Y) ? (X):(Y) )
#define SIGN(X) ( ( (X) == 0 ) ? 0 : ( ( (X) > 0 ) ? 1 : -1) )
#define AVG(X, Y) ((X + Y) / 2.0)
#define IFIN(X, Y, Z, A, B) ((X >= Y && X <= Z) ? (A) : (B))

//! Safe acos() function
#define SafeArcCosine(X) ((X > 1) ? acos(1.0) : ((X < -1) ? acos(-1.0) : acos(X)))

// Simple printing functions
#define PRINT(OS, EXP)	OS << #EXP << ": " << EXP << ", "
#define PRINTN(OS, EXP)	OS << #EXP << ": " << EXP << std::endl
#define PRINT2(OS, LBL, EXP) OS << LBL << ": " << EXP << ", "
#define PRINT_OPEN(OS, EXP)	OS << '[' << #EXP << ": " << EXP << ", "
#define PRINT_CLOSE(OS, EXP) OS << #EXP << ": " << EXP << ']' << std::endl

// XML printing functions
#define XML_PRINT(OS, LBL, EXP) OS << "<" << LBL << ">" << EXP << "</" << LBL << ">\n"
#define XML_OPEN(OS, LBL) OS << "<" << LBL << ">\n"
#define XML_CLOSE(OS, LBL) OS << "</" << LBL << ">\n"
#define XML_OPEN_ATTS(OS, LBL) OS << "<" << LBL
#define XML_PRINT_ATT(OS, A) OS << " " << #A << "=\"" << A << "\""
#define XML_PRINT_ATT2(OS, A, V) OS << " " << A << "=\"" << V << "\""
#define XML_END_ATTS(OS) OS << ">\n"
#define XML_CLOSE_ATTS(OS) OS << "/>\n"

#define PRINT_LINE  { std::cerr << std::endl << "LINE " << __LINE__ \
	<< " at " << __FILE__ << std::endl; }

// Debug Functions
#ifdef _DEBUG

#define DBG_ONLY(C) C;

#define DBG_PRN_VAL(A) #A << " = " << (A) << ", "
#define DBG_PRN_LINE(A) std::cerr << A << std::endl;

#define DBG_PRINT1(A)             { DBG_PRN_LINE( DBG_PRN_VAL(A) ) }
#define DBG_PRINT2(A,B)           { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) ) }
#define DBG_PRINT3(A,B,C)         { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) << DBG_PRN_VAL(C) ) }
#define DBG_PRINT4(A,B,C,D)       { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) << DBG_PRN_VAL(C) << DBG_PRN_VAL(D) ) }
#define DBG_PRINT5(A,B,C,D,E)     { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) << DBG_PRN_VAL(C) << DBG_PRN_VAL(D) << DBG_PRN_VAL(E) ) }
#define DBG_PRINT6(A,B,C,D,E,F)   { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) << DBG_PRN_VAL(C) << DBG_PRN_VAL(D) << DBG_PRN_VAL(E) << DBG_PRN_VAL(F) ) }
#define DBG_PRINT7(A,B,C,D,E,F,G) { DBG_PRN_LINE( DBG_PRN_VAL(A) << DBG_PRN_VAL(B) << DBG_PRN_VAL(C) << DBG_PRN_VAL(D) << DBG_PRN_VAL(E) << DBG_PRN_VAL(F) << DBG_PRN_VAL(G) ) }

#define DBG_NEWLINE { std::cerr << std::endl; }

#define DBG_PRN_LINE_INFO(A) std::cerr << "\n" << __FILE__ << "/" << __LINE__ << ": " << A << std::endl;

#define DBG_MSG1(A)              { DBG_PRN_LINE_INFO(A) }
#define DBG_MSG2(A,B)            { DBG_PRN_LINE_INFO(A << ' ' << B) }
#define DBG_MSG3(A,B,C)          { DBG_PRN_LINE_INFO(A << ' ' << B << ' ' << C) }
#define DBG_MSG4(A,B,C,D)        { DBG_PRN_LINE_INFO(A << ' ' << B << ' ' << C << ' ' << D) }
#define DBG_MSG5(A,B,C,D,E)      { DBG_PRN_LINE_INFO(A << ' ' << B << ' ' << C << ' ' << D << ' ' << E) }
#define DBG_MSG6(A,B,C,D,E,F)    { DBG_PRN_LINE_INFO(A << ' ' << B << ' ' << C << ' ' << D << ' ' << E << ' ' << F) }
#define DBG_MSG7(A,B,C,D,E,F,G)  { DBG_PRN_LINE_INFO(A << ' ' << B << ' ' << C << ' ' << D << ' ' << E << ' ' << F << ' ' << G) }

#define DBG_VAL(A) #A << " = " << (A) << ", "

#define DBG_STREAM(A) std::cerr << A << std::endl;

#define DBG_FLUSH(A) std::cerr << A << std::flush;

//! Parameters are LF: LogFile object, A: 'X' followed by any number of '<< X'. e.g., 'X << X << X'
#define DBG_LOG(LF, A) if (LF.IsActive()) { if (!LF.IsOpen()) { LF.OpenFile(); } LF << A << std::endl; }

//! A is executed only if debug log is active
#define DBG_LOG_ONLY(LF, A) if (LF.IsActive()) { if (!LF.IsOpen()) { LF.OpenFile(); } A; }

#define DBG_C_LOG(A) DBG_LOG(g_dagCreationLog, "\n" << __FUNCTION__ << ": " << A)
#define DBG_M_LOG(A) DBG_LOG(g_dagMatchingLog, "\n" << __FUNCTION__ << ": " << A)

#define DBG_C_LOG_ONLY(A) DBG_LOG_ONLY(g_dagCreationLog, A)
#define DBG_M_LOG_ONLY(A) DBG_LOG_ONLY(g_dagMatchingLog, A)

#define DBG_C_LOG_IF(A,B) { if (B) { DBG_LOG(g_dagCreationLog, __FUNCTION__ << ": " << A) } }

#define DBG_C_LOG_BEGIN_GROUP DBG_LOG(g_dagCreationLog, "\n\n")
#define DBG_M_LOG_BEGIN_GROUP DBG_LOG(g_dagMatchingLog, "\n\n")

#define DBG_C_LOG_NEW_LINE DBG_LOG(g_dagCreationLog, ".")
#define DBG_M_LOG_NEW_LINE DBG_LOG(g_dagMatchingLog, ".")

#define DBG_PRINT_IF(A,B)  { if (B) DBG_PRN_LINE( DBG_PRN_VAL(A) ) }
#define DBG_MSG_IF(A,B)    { if (B) DBG_PRN_LINE_INFO(A) }
#define DBG_STREAM_IF(A,B) { if (B) DBG_STREAM(A) }

#define DBG_SHOW(A) { std::cerr << std::endl << __FILE__ << "/" << __LINE__ \
	<< ':' << #A << " = " << (A) << std::endl; }

#define DBG_LINE  { std::cerr << std::endl << "DBG_LINE " << __LINE__ \
	<< " at " << __FILE__ << std::endl; }

#ifdef WIN32
#define ASSERT(X) if (!(X)) {_ASSERTE(X); throw 1;}
#else
#define ASSERT(X) \
	if(!(X)) { \
		std::cerr << "ERROR: Assertion failed \"" << #X << "\" in " __FILE__ \
			<< ':' << __LINE__ << std::endl; \
		assert(false); \
	}
#endif

#define WARNING(X, M) \
	if(X) { \
		std::cerr << "WARNING! " << M << ": \"" << #X << "\" in " __FILE__ \
			<< ':' << __LINE__ << std::endl; \
	}

#define WARNING1(X, M, A) \
	if(X) { \
		std::cerr << "WARNING! " << M << ": \"" << #X \
			<< " with " << #A << " = " << A \
			<< "\" in " __FILE__ << ':' << __LINE__ << std::endl; \
	}

#define WARNING2(X, M, A, B) \
	if(X) { \
		std::cerr << "WARNING! " << M << ": \"" << #X \
			<< " with " << #A << " = " << A << " and " << #B << " = " << B \
			<< "\" in " __FILE__ << ':' << __LINE__ << std::endl; \
	}

#define ASSERT_VALID_NUM(A) ASSERT(!isnan(A) && finite(A))
#define ASSERT_VALID_POINT(P) ASSERT(!isnan(P.x) && finite(P.x) && !isnan(P.y) && finite(P.y))
#define ASSERT_UNIT_INTERVAL(A) ASSERT(A >= 0 && A <= 1)

#ifdef WIN32
	#ifdef _CRTDBG_MAP_ALLOC
			#define MYDEBUG_NEW   new(_NORMAL_BLOCK, __FILE__, __LINE__)
			#define new MYDEBUG_NEW
	#endif
#endif

#else
	#define DBG_PRN_VAL(A)
	#define DBG_PRN_LINE(A)

	#define DBG_PRINT1(A)
	#define DBG_PRINT2(A,B)
	#define DBG_PRINT3(A,B,C)
	#define DBG_PRINT4(A,B,C,D)
	#define DBG_PRINT5(A,B,C,D,E)
	#define DBG_PRINT6(A,B,C,D,E,F)
	#define DBG_PRINT7(A,B,C,D,E,F,G)

	#define DBG_PRINT_NEWLINE

	#define DBG_MSG1(A)
	#define DBG_MSG2(A,B)
	#define DBG_MSG3(A,B,C)
	#define DBG_MSG4(A,B,C,D)
	#define DBG_MSG5(A,B,C,D,E)
	#define DBG_MSG6(A,B,C,D,E,F)
	#define DBG_MSG7(A,B,C,D,E,F,G)

	#define DBG_VAL(A)
	#define DBG_STREAM(A)
	#define DBG_FLUSH(A)

	#define DBG_LOG(LF, A)
	#define DBG_C_LOG(A)
	#define DBG_M_LOG(A)
	#define DBG_C_LOG_BEGIN_GROUP
	#define DBG_M_LOG_BEGIN_GROUP
	#define DBG_C_LOG_NEW_LINE
	#define DBG_M_LOG_NEW_LINE
	#define DBG_C_LOG_ONLY(A)
	#define DBG_M_LOG_ONLY(A)

	#define DBG_C_LOG_IF(A,B)

	#define DBG_PRINT_IF(A,B)
	#define DBG_MSG_IF(A,B)
	#define DBG_STREAM_IF(A,B)

	#define DBG_SHOW(A)
	#define DBG_LINE

	//#define DBG_ONLY(C)
	#define DBG_ONLY(C) C;

#ifdef WIN32
	//#define ASSERT(X) _ASSERTE(X)
	#define ASSERT(X) if (!(X)) {throw 1;}
#else
	#define ASSERT(X)
#endif

	#define WARNING(X, M)
	#define WARNING1(X, M, A)
	#define WARNING2(X, M, A, B)
	#define ASSERT_VALID_NUM(A)
	#define ASSERT_VALID_POINT(P)
	#define ASSERT_UNIT_INTERVAL(A)

#endif //_DEBUG

#endif //_BASIC_UTILS_H_
