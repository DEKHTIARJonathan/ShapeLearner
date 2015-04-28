/* ************* Begin file DAGMatcherLib.h ***************************************/
/*
** 2015 April 28
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file DAGMatcherLib.h
*	\brief DAGMatcherLib Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"

namespace dml {
	class DAGMatcherLib{
	public:
		static void InitDAGMatcherLib() throw(StandardExcept);
	private:
		DAGMatcherLib();
	};
}