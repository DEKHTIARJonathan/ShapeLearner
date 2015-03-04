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

class ObjectClass
{
public:
	ObjectClass(const string& name);

	string getKey() const {return objectClassName;}
	void setKey(const string& key) {
		GraphManager::openManager().deleteObject(*this);
		objectClassName = key;
		GraphManager::openManager().updateObject(*this);
	}

	/* =========== Template function =========== */
	string getClassName() { return "ObjectClass"; }
	/* =========== Template function =========== */

private:
	ObjectClass() {}
	string objectClassName;

	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(ObjectClass)
#pragma db member(ObjectClass::objectClassName) id


#endif // _OBJECT_CLASS_