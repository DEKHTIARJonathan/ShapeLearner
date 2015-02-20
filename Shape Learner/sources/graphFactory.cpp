/* ************* Begin file graphFactory.cpp ***************************************/
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
*	\file graphFactory.cpp
*	\brief Interface entre le logiciel et la base de données. Elle est la seule à réaliser des accès à la BDD
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

template <class Object,class Key> std::map<Key,Object*> Factory<Object,Key>::m_map=std::map<Key,Object*>();

template <class Object,class Key> void Factory<Object,Key>::Register(Key key,Object* obj)
{
        if(m_map.find(key)==m_map.end())
        {
               m_map[key]=obj;
        }
}

template <class Object,class Key> Object* Factory<Object,Key>::Create (const Key& key)
{
                Object* tmp=0;
                typename std::map<Key, Object*>::iterator it=m_map.find(key);
    
                if(it!=m_map.end())
                {
                      tmp=((*it).second)->Clone();
                      }

                return tmp;
}