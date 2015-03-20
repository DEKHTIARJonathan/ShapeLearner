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
*	\author DEKHTIAR Jonathan
*/

#ifndef _OBJECT_CLASS_
#define _OBJECT_CLASS_

#include "allHeaders.h"
using namespace std;

class GraphManager; // Forward Declaration of the class contained in graphManager.h

class ObjectClass
{
public:
	class Access {
		friend class GraphManager;
		static boost::shared_ptr<ObjectClass> createObjectClass(string name){
			return boost::shared_ptr<ObjectClass> (new ObjectClass(name));
		}
	};

	string getKey() const {return objectClassName;}

	vector<unsigned long> getGraphs();

	/* =========== Template function =========== */
	string getClassName() const { return "ObjectClass"; }
	/* =========== Template function =========== */

	void updateInDB();
	string saveInDB();

private:
	ObjectClass() {}
	ObjectClass(string name);

	string objectClassName;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(ObjectClass)
#pragma db member(ObjectClass::objectClassName) id

#endif // _OBJECT_CLASS_