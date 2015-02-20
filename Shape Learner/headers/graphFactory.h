/* ************* Begin file graphFactory.h ***************************************/
/*
** 2015 February 18
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file graphFactory.h
*	\brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#ifndef _GRAPH_FACTORY_H_
#define _GRAPH_FACTORY_H_

#include "allHeaders.h"
using namespace std;


template <class Object,class Key=string> class Factory
{
      static std::map<Key,Object*> m_map;
public:

        static void Register(Key key,Object* obj);
        Object* Create(const Key& key);
};


#endif //_GRAPH_FACTORY_H_