/* ************* Begin file shapeLearner.cpp ***************************************/
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
*	\file shapeLearner.cpp
*	\brief ShapeLearner source file. Public Interface and Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

 
#include "stdafx.h"
#include "allHeaders.h"

using namespace std;


/* *******************************************************************
*                             Object MAPS                            *
 ********************************************************************/

map<unsigned long, boost::shared_ptr<Node>>		ShapeLearner::NodeMap;
map<unsigned long, boost::shared_ptr<Point>>	ShapeLearner::PointMap;
map<unsigned long, boost::shared_ptr<Edge>>		ShapeLearner::EdgeMap;
map<unsigned long, boost::shared_ptr<Graph>>	ShapeLearner::GraphMap;
map<string, boost::shared_ptr<ObjectClass>>		ShapeLearner::ObjectClassMap;
map<string, boost::shared_ptr<GraphClass>>		ShapeLearner::GraphClassMap;

/* *******************************************************************
*                             Multi Threading                        *
 ********************************************************************/

boost::threadpool::pool ShapeLearner::Pool (constants::nbMaxThread);

/*!
*	\Brief Local Function used to get the ThreadID.
*/

const unsigned long getThreadId(){
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    unsigned long threadNumber = 0;
    sscanf(threadId.c_str(), "%lx", &threadNumber);
    return threadNumber;
}

/* *******************************************************************
*                           USER INTERFACE                           *
******************************************************************* */

void ShapeLearner::closeDatabase() throw(ShapeLearnerExcept) {
	if (!DatabaseManager::Interface::isDbOpen())
		throw ShapeLearnerExcept((string)__FUNCTION__, "Error : The database is not opened yet. Impossible to close it.");
	DatabaseManager::Interface::closeDatabase();
}

void ShapeLearner::openDatabase(const string& _dbUser, const string& _dbPass, const string& _dbName, const string& _dbHost, const unsigned int& _dbPort, const string& _dbInit)   throw(ShapeLearnerExcept) {
	if (DatabaseManager::Interface::isDbOpen())
		throw ShapeLearnerExcept((string)__FUNCTION__, "Error : The Database has already been instantiated. It's impossible to modify the Database's parameters");
	else
		DatabaseManager::Interface::openDatabase(_dbUser, _dbPass, _dbName, _dbHost, _dbPort, _dbInit);
}

/* *******************************************************************
*                          COMMON INTERFACE                          *
 ********************************************************************/

boost::weak_ptr<Point> ShapeLearner::CommonInterface::getPoint(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Point>>::const_iterator it = PointMap.find(keyDB);
	if(it != PointMap.end())
		return  boost::weak_ptr<Point>(it->second);
	else{
		boost::shared_ptr<Point> tmp (loadObject<Point> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept((string)__FUNCTION__+"(const unsigned long  keyDB)", "Error : The Point referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(keyDB,tmp));
		return boost::weak_ptr<Point>(tmp);
	}
}
boost::weak_ptr<Point> ShapeLearner::CommonInterface::getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph, const double _xCoord, const double _yCoord, const double _radius){
	boost::shared_ptr<Point> tmp (Point::Access::createPoint(_refNode, _refGraph, _xCoord, _yCoord, _radius));
	PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Point>(tmp);
}

boost::weak_ptr<Node> ShapeLearner::CommonInterface::getNode(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Node>>::const_iterator it = NodeMap.find(keyDB);
	if(it != NodeMap.end())
		return  boost::weak_ptr<Node>(it->second);
	else{
		boost::shared_ptr<Node> tmp (loadObject<Node> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Node referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		NodeMap.insert(pair<unsigned long , boost::shared_ptr<Node>>(keyDB,tmp));
		return boost::weak_ptr<Node>(tmp);
	}
}
boost::weak_ptr<Node> ShapeLearner::CommonInterface::getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned long _index, const unsigned long _level, const unsigned long _mass, const unsigned long _type, const string _label){
	boost::shared_ptr<Node> tmp (Node::Access::createNode(_refGraph, _index, _level, _mass, _type, _label));
	NodeMap.insert(pair<unsigned long, boost::shared_ptr<Node>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Node>(tmp);
}

boost::weak_ptr<Edge> ShapeLearner::CommonInterface::getEdge(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Edge>>::const_iterator it = EdgeMap.find(keyDB);
	if(it != EdgeMap.end())
		return  boost::weak_ptr<Edge>(it->second);
	else{
		boost::shared_ptr<Edge> tmp (loadObject<Edge> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Edge referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(keyDB,tmp));
		return boost::weak_ptr<Edge>(tmp);
	}
}
boost::weak_ptr<Edge> ShapeLearner::CommonInterface::getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned long _weight){
	boost::shared_ptr<Edge> tmp (Edge::Access::createEdge(_source, _target, _refGraph, _weight));
	EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Edge>(tmp);
}

boost::weak_ptr<Graph> ShapeLearner::CommonInterface::getGraph(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Graph>>::const_iterator it = GraphMap.find(keyDB);
	if(it != GraphMap.end())
		return boost::weak_ptr<Graph>(it->second);
	else{
		boost::shared_ptr<Graph> tmp (loadObject<Graph> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept((string)__FUNCTION__+"(const unsigned long keyDB)", "Error : The Graph referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(keyDB,tmp));
		return boost::weak_ptr<Graph>(tmp);
	}
}
boost::weak_ptr<Graph> ShapeLearner::CommonInterface::getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName, const unsigned long _viewNumber){
	boost::shared_ptr<Graph> tmp (Graph::Access::createGraph(_graphClass, _objectClass, _objectName, _viewNumber));
	GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Graph>(tmp);
}

boost::weak_ptr<GraphClass> ShapeLearner::CommonInterface::getGraphClass(const string& name, const bool isDirect, const bool isAcyclic) throw(ShapeLearnerExcept){
	map<string, boost::shared_ptr<GraphClass>>::const_iterator it = GraphClassMap.find(name);
	if(it != GraphClassMap.end())
		return  boost::weak_ptr<GraphClass>(it->second);
	else{
		boost::shared_ptr<GraphClass> tmp (loadObject<GraphClass>(name));
		if (!tmp)
			tmp.swap(boost::shared_ptr<GraphClass> (GraphClass::Access::createGraphClass(name, isDirect, isAcyclic)));
		GraphClassMap.insert(pair<string, boost::shared_ptr<GraphClass>>(name,tmp));
		return boost::weak_ptr<GraphClass>(tmp);
	}
}

boost::weak_ptr<ObjectClass> ShapeLearner::CommonInterface::getObjectClass(const string& name) throw(ShapeLearnerExcept){
	map<string, boost::shared_ptr<ObjectClass>>::const_iterator it = ObjectClassMap.find(name);
	if(it != ObjectClassMap.end())
		return  boost::weak_ptr<ObjectClass>(it->second);
	else{
		boost::shared_ptr<ObjectClass> tmp (loadObject<ObjectClass>(name));
		if (!tmp)
			tmp.swap(boost::shared_ptr<ObjectClass> (ObjectClass::Access::createObjectClass(name)));
		ObjectClassMap.insert(pair<string, boost::shared_ptr<ObjectClass>>(name, tmp));
		return boost::weak_ptr<ObjectClass>(tmp);
	}
}

/* *******************************************************************
*                          Private FUNCTIONS                         *
 ********************************************************************/

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<Point> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!PointMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<Point> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");

	return rslt;
}

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<Edge> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!EdgeMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<Edge> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");

	return rslt;
}

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<Node> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!NodeMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<Node> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");

	if(cascade){

		/**************** Edge Cascade Deleting ****************/
		vector <unsigned long> edgeVect = obj->getEdges();
		map<unsigned long, boost::shared_ptr<Edge>>::iterator it1;

		for(unsigned int i = 0; i < edgeVect.size(); i++){
			if(it1 = EdgeMap.find(edgeVect[i]), it1 != EdgeMap.end())
				rslt &= removeObjectFromMap(it1->second);	// The cascade parameter is useless for this class
		}

		/**************** Point Cascade Deleting ****************/
		vector <unsigned long> pointVect = obj->getPoints();
		map<unsigned long, boost::shared_ptr<Point>>::iterator it2;

		for(unsigned int i = 0; i < pointVect.size(); i++){			
			if(it2 = PointMap.find(pointVect[i]), it2 != PointMap.end())
				rslt &= removeObjectFromMap(it2->second);		// The cascade parameter is useless for this class	
		}
	}

	return rslt;
}

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<Graph> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!GraphMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<Graph> obj, bool cascade)", "Error : The object's key ("+to_string((_ULonglong)obj->getKey())+") can't be found");

	if(cascade){

		/**************** Node Cascade Deleting ****************/
		vector <unsigned long> nodeVect = obj->getNodes();
		map<unsigned long, boost::shared_ptr<Node>>::iterator it1;

		for(unsigned int i = 0; i < nodeVect.size(); i++){
			if(it1 = NodeMap.find(nodeVect[i]), it1 != NodeMap.end())
				rslt &= removeObjectFromMap(it1->second, false); // No need to act on cascade, we will manage it at this level.		
		}

		/**************** Edge Cascade Deleting ****************/
		vector <unsigned long> edgeVect = obj->getEdges();
		map<unsigned long, boost::shared_ptr<Edge>>::iterator it2;

		for(unsigned int i = 0; i < edgeVect.size(); i++){
			if(it2 = EdgeMap.find(edgeVect[i]), it2 != EdgeMap.end())
				rslt &= removeObjectFromMap(it2->second);	// The cascade parameter is useless for this class
		}

		/**************** Point Cascade Deleting ****************/
		vector <unsigned long> pointVect = obj->getPoints();
		map<unsigned long, boost::shared_ptr<Point>>::iterator it3;

		for(unsigned int i = 0; i < pointVect.size(); i++){
			if(it3 = PointMap.find(pointVect[i]), it3 != PointMap.end())
				rslt &= removeObjectFromMap(it3->second);		// The cascade parameter is useless for this class	
		}
	}
	return rslt;
}

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<GraphClass> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!GraphClassMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<GraphClass> obj, bool cascade)", "Error : The object's key ("+obj->getKey()+") can't be found");

	if(cascade){
		/**************** Graph Cascade Deleting ****************/
		vector <unsigned long> graphVect = obj->getGraphs();
		map<unsigned long, boost::shared_ptr<Graph>>::iterator it;

		for(unsigned int i = 0; i < graphVect.size(); i++){
			if(it = GraphMap.find(graphVect[i]), it != GraphMap.end())
				rslt &= removeObjectFromMap(it->second, true);		
		}
	}

	return rslt;
}

bool ShapeLearner::removeObjectFromMap(boost::shared_ptr<ObjectClass> obj, bool cascade) throw (ShapeLearnerExcept){
	bool rslt = true;

	if(rslt &= !!ObjectClassMap.erase(obj->getKey()), !rslt)
		throw ShapeLearnerExcept((string)__FUNCTION__+"(boost::shared_ptr<ObjectClass> obj, bool cascade)", "Error : The object's key ("+obj->getKey()+") can't be found");

	if(cascade){
		/**************** Graph Cascade Deleting ****************/
		vector <unsigned long> graphVect = obj->getGraphs();
		map<unsigned long, boost::shared_ptr<Graph>>::iterator it;

		for(unsigned int i = 0; i < graphVect.size(); i++){
			if(it = GraphMap.find(graphVect[i]), it != GraphMap.end())
				rslt &= removeObjectFromMap(it->second, true);		
		}
	}

	return rslt;
}

void ShapeLearner::createShockGraph (const vector<const string> &imgVect) throw(ShapeLearnerExcept){	
	//Random Init
	std::srand(std::time(0));
	
	// Create fifo thread pool container with two threads.
	for (vector<const string>::const_iterator it = imgVect.begin(); it != imgVect.end(); it++){
		Pool.schedule(boost::bind(&ShapeLearner::createShockGraphWorker, *it));
	} 
	//  Wait until all tasks are finished
	Pool.wait();
}

bool ShapeLearner::createShockGraphWorker (const string& imgPath) throw(ShapeLearnerExcept){
	const unsigned long idThread = getThreadId();
	shockGraphsGenerator worker(imgPath, idThread);
	worker.taskExecute();
	return true;
}



