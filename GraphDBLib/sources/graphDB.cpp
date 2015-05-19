/* ************* Begin file graphDB.cpp ***************************************/
/*
** 2015 February 23
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file graphDB.cpp
*	\brief GraphDB source file. Public Interface and Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace graphDBLib;

/* *******************************************************************
*                             Object MAPS                            *
 ********************************************************************/

map<unsigned long, boost::shared_ptr<Node>>		GraphDB::NodeMap;
map<unsigned long, boost::shared_ptr<Point>>	GraphDB::PointMap;
map<unsigned long, boost::shared_ptr<Edge>>		GraphDB::EdgeMap;
map<unsigned long, boost::shared_ptr<Graph>>	GraphDB::GraphMap;
map<string, boost::shared_ptr<ObjectClass>>		GraphDB::ObjectClassMap;
map<string, boost::shared_ptr<GraphClass>>		GraphDB::GraphClassMap;

boost::mutex									GraphDB::mtxNode;
boost::mutex									GraphDB::mtxPoint;
boost::mutex									GraphDB::mtxEdge;
boost::mutex									GraphDB::mtxGraph;
boost::mutex									GraphDB::mtxObjectClass;
boost::mutex									GraphDB::mtxGraphClass;

/* *******************************************************************
*                           USER INTERFACE                           *
******************************************************************* */

void GraphDB::closeDatabase() throw(StandardExcept) {
	if (!DatabaseManager::Interface::isDbOpen()){
		Logger::Log((string)__FUNCTION__ + "\nError : The database is not opened yet. Impossible to close it.", constants::LogError);
		
	}
	else{
		try{
			DatabaseManager::Interface::closeDatabase();
		}
		catch (const std::exception& e)
		{
			Logger::Log(e.what (), constants::LogError);
			
		}
	}
}

void GraphDB::openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit)   throw(StandardExcept) {
	try{
		if (DatabaseManager::Interface::isDbOpen()){
			Logger::Log((string)__FUNCTION__ + "\n Error : The Database has already been instantiated. It's impossible to modify the Database's parameters", constants::LogError);
			
		}
		else
			DatabaseManager::Interface::openDatabase(_dbUser, _dbPass, _dbName, _dbHost, _dbPort, _dbInit);
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}

/* *******************************************************************
*                          COMMON INTERFACE                          *
 ********************************************************************/

boost::weak_ptr<Point> GraphDB::CommonInterface::getPoint(const unsigned long keyDB) throw(StandardExcept){
	try{
		_Lock_Point_
		map<unsigned long, boost::shared_ptr<Point>>::const_iterator it = PointMap.find(keyDB);
		map<unsigned long, boost::shared_ptr<Point>>::const_iterator endIT = PointMap.end();
		_Unlock_Point_
		if(it != endIT)
			return  boost::weak_ptr<Point>(it->second);
		else{
			boost::shared_ptr<Point> tmp (loadObject<Point> (keyDB));
			if (!tmp)
				throw StandardExcept((string)__FUNCTION__+"(const unsigned long  keyDB)", "Error : The Point referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
			_Lock_Point_
			PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(keyDB,tmp));
			_Unlock_Point_
			return boost::weak_ptr<Point>(tmp);
		}
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}
boost::weak_ptr<Point> GraphDB::CommonInterface::getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph){
	try{
		boost::shared_ptr<Point> tmp (Point::Access::createPoint(_refNode, _refGraph));
		_Lock_Point_
		PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(tmp->getKey(), tmp));
		_Unlock_Point_
		return boost::weak_ptr<Point>(tmp);
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
	}
}

boost::weak_ptr<Node> GraphDB::CommonInterface::getNode(const unsigned long keyDB) throw(StandardExcept){
	try{
		_Lock_Node_
		map<unsigned long, boost::shared_ptr<Node>>::const_iterator it = NodeMap.find(keyDB);
		map<unsigned long, boost::shared_ptr<Node>>::const_iterator endIT = NodeMap.end();
		_Unlock_Node_
		if(it != endIT)
			return  boost::weak_ptr<Node>(it->second);
		else{
			boost::shared_ptr<Node> tmp (loadObject<Node> (keyDB));
			if (!tmp)
				throw StandardExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Node referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
			_Lock_Node_
			NodeMap.insert(pair<unsigned long , boost::shared_ptr<Node>>(keyDB,tmp));
			_Unlock_Node_
			return boost::weak_ptr<Node>(tmp);
		}
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}
boost::weak_ptr<Node> GraphDB::CommonInterface::getNode(const boost::weak_ptr<Graph> _refGraph){
	try{
		boost::shared_ptr<Node> tmp (Node::Access::createNode(_refGraph));
		_Lock_Node_
		NodeMap.insert(pair<unsigned long, boost::shared_ptr<Node>>(tmp->getKey(), tmp));
		_Unlock_Node_
		return boost::weak_ptr<Node>(tmp);
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}

boost::weak_ptr<Edge> GraphDB::CommonInterface::getEdge(const unsigned long keyDB) throw(StandardExcept){
	try{
		_Lock_Edge_
		map<unsigned long, boost::shared_ptr<Edge>>::const_iterator it = EdgeMap.find(keyDB);
		map<unsigned long, boost::shared_ptr<Edge>>::const_iterator endIT = EdgeMap.end();
		_Unlock_Edge_
		if(it != endIT)
			return  boost::weak_ptr<Edge>(it->second);
		else{
			boost::shared_ptr<Edge> tmp (loadObject<Edge> (keyDB));
			if (!tmp)
				throw StandardExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Edge referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
			_Lock_Edge_
			EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(keyDB,tmp));
			_Unlock_Edge_
			return boost::weak_ptr<Edge>(tmp);
		}
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}
boost::weak_ptr<Edge> GraphDB::CommonInterface::getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph){
	try{
		boost::shared_ptr<Edge> tmp (Edge::Access::createEdge(_source, _target, _refGraph));
		_Lock_Edge_
		EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(tmp->getKey(), tmp));
		_Unlock_Edge_
		return boost::weak_ptr<Edge>(tmp);
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}

boost::weak_ptr<Graph> GraphDB::CommonInterface::getGraph(const unsigned long keyDB) throw(StandardExcept){
	try{
		_Lock_Graph_
		map<unsigned long, boost::shared_ptr<Graph>>::const_iterator it = GraphMap.find(keyDB);
		map<unsigned long, boost::shared_ptr<Graph>>::const_iterator endIT = GraphMap.end();
		_Unlock_Graph_
		if(it != endIT)
			return boost::weak_ptr<Graph>(it->second);
		else{
			boost::shared_ptr<Graph> tmp (loadObject<Graph> (keyDB));
			if (!tmp)
				throw StandardExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Graph referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
			_Lock_Graph_
			GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(keyDB,tmp));
			_Unlock_Graph_
			return boost::weak_ptr<Graph>(tmp);
		}
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}
boost::weak_ptr<Graph> GraphDB::CommonInterface::getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName){
	try{
		boost::shared_ptr<Graph> tmp (Graph::Access::createGraph(_graphClass, _objectClass, _objectName));
		_Lock_Graph_
		GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(tmp->getKey(), tmp));
		_Unlock_Graph_
		return boost::weak_ptr<Graph>(tmp);
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}

boost::weak_ptr<GraphClass> GraphDB::CommonInterface::getGraphClass(const string& name, const bool isDirect, const bool isAcyclic) throw(StandardExcept){
		_Lock_GraphClass_
		map<string, boost::shared_ptr<GraphClass>>::const_iterator it = GraphClassMap.find(name);
		map<string, boost::shared_ptr<GraphClass>>::const_iterator endIT = GraphClassMap.end();
		_Unlock_GraphClass_
		if(it != endIT)
			return  boost::weak_ptr<GraphClass>(it->second);
		else{
			boost::shared_ptr<GraphClass> tmp (loadObject<GraphClass>(name));
			if (!tmp)
				tmp.swap(boost::shared_ptr<GraphClass> (GraphClass::Access::createGraphClass(name, isDirect, isAcyclic)));
			_Lock_GraphClass_
			GraphClassMap.insert(pair<string, boost::shared_ptr<GraphClass>>(name,tmp));
			_Unlock_GraphClass_
			return boost::weak_ptr<GraphClass>(tmp);
		}
}

boost::weak_ptr<ObjectClass> GraphDB::CommonInterface::getObjectClass(const string& name) throw(StandardExcept){
	try{
		_Lock_ObjectClass_
		map<string, boost::shared_ptr<ObjectClass>>::const_iterator it = ObjectClassMap.find(name);
		map<string, boost::shared_ptr<ObjectClass>>::const_iterator endIT = ObjectClassMap.end();
		_Unlock_ObjectClass_
		if(it != ObjectClassMap.end())
			return  boost::weak_ptr<ObjectClass>(it->second);
		else{
			boost::shared_ptr<ObjectClass> tmp (loadObject<ObjectClass>(name));
			if (!tmp)
				tmp.swap(boost::shared_ptr<ObjectClass> (ObjectClass::Access::createObjectClass(name)));
			_Lock_ObjectClass_
			ObjectClassMap.insert(pair<string, boost::shared_ptr<ObjectClass>>(name, tmp));
			_Unlock_ObjectClass_
			return boost::weak_ptr<ObjectClass>(tmp);
		}
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
	}
}

/* *******************************************************************
*                          Private FUNCTIONS                         *
 ********************************************************************/

bool GraphDB::removeObjectFromMap(boost::shared_ptr<Point> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_Point_
		if(rslt &= !!PointMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<Point> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");
		_Unlock_Point_

		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return false;
	}
}

bool GraphDB::removeObjectFromMap(boost::shared_ptr<Edge> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_Edge_
		if(rslt &= !!EdgeMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<Edge> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");
		_Unlock_Edge_

		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return false;
	}
}

bool GraphDB::removeObjectFromMap(boost::shared_ptr<Node> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_Node_
		if(rslt &= !!NodeMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<Node> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");
		_Unlock_Node_

		if(cascade){
			/**************** Edge Cascade Deleting ****************/
			vector <unsigned long> edgeVect = obj->getEdges();
			map<unsigned long, boost::shared_ptr<Edge>>::iterator it1;
			map<unsigned long, boost::shared_ptr<Edge>>::iterator it1End;

			for(unsigned int i = 0; i < edgeVect.size(); i++){
				_Lock_Edge_
				it1 = EdgeMap.find(edgeVect[i]);
				it1End = EdgeMap.end();
				_Unlock_Edge_
				if(it1 != it1End)
					rslt &= removeObjectFromMap(it1->second);	// The cascade parameter is useless for this class
			}

			/**************** Point Cascade Deleting ****************/
			vector <unsigned long> pointVect = obj->getPoints();
			map<unsigned long, boost::shared_ptr<Point>>::iterator it2;
			map<unsigned long, boost::shared_ptr<Point>>::iterator it2End;

			for(unsigned int i = 0; i < pointVect.size(); i++){
				_Lock_Point_
				it2 = PointMap.find(pointVect[i]);
				it2End = PointMap.end();
				_Unlock_Point_
				if(it2 != it2End)
					rslt &= removeObjectFromMap(it2->second);		// The cascade parameter is useless for this class
			}
		}

		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return false;
	}
}

bool GraphDB::removeObjectFromMap(boost::shared_ptr<Graph> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_Graph_
		if(rslt &= !!GraphMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<Graph> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");
		_Unlock_Graph_

		if(cascade){
			/**************** Node Cascade Deleting ****************/
			vector <unsigned long> nodeVect = obj->getNodes();
			map<unsigned long, boost::shared_ptr<Node>>::iterator it1;
			map<unsigned long, boost::shared_ptr<Node>>::iterator it1End;

			for(unsigned int i = 0; i < nodeVect.size(); i++){
				_Lock_Node_
				it1 = NodeMap.find(nodeVect[i]);
				it1End = NodeMap.end();
				_Unlock_Node_
				if(it1 != it1End)
					rslt &= removeObjectFromMap(it1->second, false); // No need to act on cascade, we will manage it at this level.
			}

			/**************** Edge Cascade Deleting ****************/
			vector <unsigned long> edgeVect = obj->getEdges();
			map<unsigned long, boost::shared_ptr<Edge>>::iterator it2;
			map<unsigned long, boost::shared_ptr<Edge>>::iterator it2End;

			for(unsigned int i = 0; i < edgeVect.size(); i++){
				_Lock_Edge_
				it2 = EdgeMap.find(edgeVect[i]);
				it2End = EdgeMap.end();
				_Unlock_Edge_
				if(it2 != it2End)
					rslt &= removeObjectFromMap(it2->second);	// The cascade parameter is useless for this class
			}

			/**************** Point Cascade Deleting ****************/
			vector <unsigned long> pointVect = obj->getPoints();
			map<unsigned long, boost::shared_ptr<Point>>::iterator it3;
			map<unsigned long, boost::shared_ptr<Point>>::iterator it3End;

			for(unsigned int i = 0; i < pointVect.size(); i++){
				_Lock_Point_
				it3 = PointMap.find(pointVect[i]);
				it3End = PointMap.end();
				_Unlock_Point_
				if(it3 != it3End)
					rslt &= removeObjectFromMap(it3->second);		// The cascade parameter is useless for this class
			}
		}
		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return EXIT_FAILURE;
	}
}

bool GraphDB::removeObjectFromMap(boost::shared_ptr<GraphClass> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_GraphClass_
		if(rslt &= !!GraphClassMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<GraphClass> obj, bool cascade)", "Error : The object's key ("+obj->getKey()+") can't be found");
		_Unlock_GraphClass_

		if(cascade){
			/**************** Graph Cascade Deleting ****************/
			vector <unsigned long> graphVect = obj->getGraphs();
			map<unsigned long, boost::shared_ptr<Graph>>::iterator it;
			map<unsigned long, boost::shared_ptr<Graph>>::iterator endIT;

			for(unsigned int i = 0; i < graphVect.size(); i++){
				_Lock_Graph_
				it = GraphMap.find(graphVect[i]);
				endIT = GraphMap.end();
				_Unlock_Graph_
				if(it != endIT)
					rslt &= removeObjectFromMap(it->second, true);
			}
		}

		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return EXIT_FAILURE;
	}
}

bool GraphDB::removeObjectFromMap(boost::shared_ptr<ObjectClass> obj, bool cascade) throw (StandardExcept){
	try{
		bool rslt = true;

		_Lock_ObjectClass_
		if(rslt &= !!ObjectClassMap.erase(obj->getKey()), !rslt)
			throw StandardExcept((string)__FUNCTION__+"(boost::shared_ptr<ObjectClass> obj, bool cascade)", "Error : The object's key ("+obj->getKey()+") can't be found");
		_Unlock_ObjectClass_

		if(cascade){
			/**************** Graph Cascade Deleting ****************/
			vector <unsigned long> graphVect = obj->getGraphs();
			map<unsigned long, boost::shared_ptr<Graph>>::iterator it;
			map<unsigned long, boost::shared_ptr<Graph>>::iterator endIT;

			for(unsigned int i = 0; i < graphVect.size(); i++){
				_Lock_Graph_
				it = GraphMap.find(graphVect[i]);
				endIT = GraphMap.end();
				_Unlock_Graph_
				if( it != endIT)
					rslt &= removeObjectFromMap(it->second, true);
			}
		}

		return rslt;
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		
		return EXIT_FAILURE;
	}
}