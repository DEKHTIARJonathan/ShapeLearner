/* ************* Begin file jobManager.h ***************************************/
/*
** 2015 April 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file jobManager.h
*	\brief jobManager Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _JOB_MANAGER_
#define _JOB_MANAGER_

#ifdef _MSC_VER
	#pragma message("Compiling ShapeLearnerLib::shockGraphGenerator.h  - this should happen just once per project.\n")
#endif

#include <iostream>
#include <vector>
#include <string>
#include <boost/utility.hpp>

using namespace std;

class JobManager : boost::noncopyable {
public:
	static void initJobManager();
};

#endif // _JOB_MANAGER_