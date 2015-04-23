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

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::ObjectClass.h  - this should happen just once per project.\n")
	#include "stdafx.h"
	#include "allHeaders.h"
#else
	#include "odbHeaders.h"
#endif

using namespace std;

class GraphDB; // Forward Declaration of the class contained in graphDB.h

/*!	
*	\class ObjectClass
*	\brief Part of the Graph Data Model. An ObjectClass is for instance : "Rod" / "Piston" / "Wheel" / ...
*/
class ObjectClass
{
public:
	/*!	
	*	\class ObjectClass::Access
	*	\brief Limit instantiation only to GraphDB. Static subclass which role is only to execute its unique static method.
	*/
	class Access {
		friend class GraphDB;
		static boost::shared_ptr<ObjectClass> createObjectClass(string name){
			return boost::shared_ptr<ObjectClass> (new ObjectClass(name));
		}
	};

	string getKey() const {return objectClassName;}

	vector<unsigned long> getGraphs();

	/* =========== Template function =========== */
	string getClassName() const { return "ObjectClass"; }
	/* =========== Template function =========== */

private:
	string objectClassName;
	
	/*!
	*	\brief  Classical constructor needed to let ODB load objects from DB.
	*/
	ObjectClass() {}
	ObjectClass(string name);

	/*!
	*	\fn void updateInDB();
	*	\brief Update the object in the database.
	*/
	void updateInDB();

	/*!
	*	\fn string saveInDB();
	*	\brief Persist the object in the database.
	*/
	string saveInDB();

	/*!
	*	\brief Friendship required in order to let ODB manage the object.
	*/
	friend class odb::access;
};

#pragma db value(std::string) type("VARCHAR(255)")
#pragma db object(ObjectClass)
#pragma db member(ObjectClass::objectClassName) id

#endif // _OBJECT_CLASS_