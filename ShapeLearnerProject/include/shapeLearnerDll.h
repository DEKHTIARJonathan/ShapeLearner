/* ************* Begin file shapeLearnerDll.h ***************************************/
/*
** 2015 July 08
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/*!
*	\file shapeLearnerDll.h
*	\brief ShapeLearnerDLL header file.
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _Shape_Learner_DLL_
#define _Shape_Learner_DLL_

#include <iostream>
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//__declspec(dllexport) bool openDataBase(string initFile = "")
__declspec(dllexport) void openDataBase(char* _dbUser, char* _dbPass, char* _dbName, char* _dbHost, unsigned int _dbPort, char* _dbInit = "") ;

//__declspec(dllexport) void initMatcher() ;

__declspec(dllexport) void signBinaryImage(char* _imgPath, char* _imgClass, unsigned int _jobID) ;

__declspec(dllexport) unsigned int getActiveThread() ;

__declspec(dllexport) void waitBeforeClosing() ;

#ifdef __cplusplus
}
#endif

#endif // _Shape_Learner_DLL_
