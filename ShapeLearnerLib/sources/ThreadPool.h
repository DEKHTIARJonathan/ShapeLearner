/* ************* Begin file ThreadPool.h ***************************************/
/*
** 2015 April 07
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file ThreadPool.h
*	\brief ThreadPool Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _THREAD_POOL_
#define _THREAD_POOL_

#include "allHeaders.h"
using namespace std;

/*!
*	\class ThreadPool
*	\brief Virtual Pure Class implementing the "thread pool" design pattern.
*/
class ThreadPool{
public:

private:
	static volatile unsigned int _nbActiveThread;;
	static volatile shockGraphsGenerator _inst;
	/*!
	*	\fn shockGraphsGenerator()
	*	\brief Private constructor
	*/
    shockGraphsGenerator();
	
	/*!
	*	\fn ~shockGraphsGenerator()
	*	\brief Private Destructor, automaticly called at the end of the program (object instanciated on stack). Close all ofstreams.
	*/
	~shockGraphsGenerator();
	
	/*!
	*	\fn shockGraphsGenerator(const shockGraphsGenerator&)
	*	\brief Singleton Pattern, forbid copy
	*/
	shockGraphsGenerator(const shockGraphsGenerator);
	
	/*!
	*	\fn void operator=(const shockGraphsGenerator&)
	*	\brief Singleton Pattern, forbid copy
	*/
	void operator=(const shockGraphsGenerator&);
};

#endif //_THREAD_POOL_