/* ************* Begin file ObjectClass.h ***************************************/
/*
** 2015 March 02
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file ObjectCLass.h
*	\brief ObjectClass Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _OBJECT_CLASS_
#define _OBJECT_CLASS_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include "StandardException.h"
#include "SDK/GraphClass.h"
#include "SDK/ObjectClass.h"
#include "SDK/Graph.h"
#include "SDK/Node.h"
#include "SDK/Point.h"
#include "SDK/Edge.h"

#include <odb/core.hxx>
#include <odb/callback.hxx>
#include <odb/boost/lazy-ptr.hxx>
#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/pgsql/database.hxx>
#include <odb/schema-catalog.hxx>

using namespace std;

namespace graphDBLib{

	class GraphDB;

	class ObjectClass
	{
	public:
		class Access {
			static boost::shared_ptr<ObjectClass> createObjectClass(string name);
		};

		string getKey() const;

		vector<unsigned long> getGraphs();

		/* =========== Template function =========== */
		string getClassName() const;
		/* =========== Template function =========== */

		void resynchronize();

	private:
		
		/*!
		*	\brief  Classical constructor needed to let ODB load objects from DB.
		*/
		ObjectClass() {}
		ObjectClass(string name);
	};
}

#endif // _OBJECT_CLASS_