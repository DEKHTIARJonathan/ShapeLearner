/* ************* Begin file graphManager.cpp ***************************************/
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
*	\file graphManager.cpp
*	\brief Interface publique d'accès au logiciel. Elle concentre les accès à la BDD, le Génarateur de Graph et le Graph Matcher.
*	\version 1.1
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

/* *******************************************************************
*                        Static DB Parameters                        *
 ********************************************************************/

string GraphManager::dbName = "postgres";
string GraphManager::dbUser = "postgres";
string GraphManager::dbPass = "postgres";
unsigned int GraphManager::dbPort = 5432;
string GraphManager::dbHost = "localhost";
string GraphManager::dbInitFile = "sources/structure.sql";
unsigned int GraphManager::dbType = constants::DB_PGSQL;

/* *******************************************************************
*                             Object MAPS                            *
 ********************************************************************/

map<unsigned long, boost::shared_ptr<Node>>		GraphManager::NodeMap;
map<unsigned long, boost::shared_ptr<Point>>	GraphManager::PointMap;
map<unsigned long, boost::shared_ptr<Edge>>		GraphManager::EdgeMap;
map<unsigned long, boost::shared_ptr<Graph>>	GraphManager::GraphMap;
map<string, boost::shared_ptr<ObjectClass>>		GraphManager::ObjectClassMap;
map<string, boost::shared_ptr<GraphClass>>		GraphManager::GraphClassMap;

/* *******************************************************************
*                           USER INTERFACE                           *
******************************************************************* */

void GraphManager::UserInterface::openDatabase() {
	DatabaseManager::Interface::openDatabase(dbUser, dbPass, dbName, dbHost, dbPort, dbType, dbInitFile);
}

void GraphManager::UserInterface::closeDatabase() throw(ShapeLearnerExcept) {
	DatabaseManager::Interface::closeDatabase();
}

void GraphManager::UserInterface::getDbCredentials(const bool dbInit) throw(ShapeLearnerExcept){
	if (DatabaseManager::Interface::isDbOpen()){
		throw ShapeLearnerExcept("GraphManager::UserInterface::getDbCredentials", "Error : The Database has already been instantiated. It's impossible to modify the Database's parameters");
	}
	else{
		if (dbInit)
			setDBInitFile();
		else
			dbInitFile = "";
		#ifndef _DEBUG
		setDbType();
		setDbHost();
		setDbPort();
		setDbName();
		setDbUser();
		setDbPass();
		#endif
	}
}

void GraphManager::UserInterface::test() throw(ShapeLearnerExcept){
		//boost::weak_ptr<ObjectClass> OC1 ( CommonInterface::getObjectClass("Rod"));
		boost::weak_ptr<ObjectClass> OC2 ( CommonInterface::getObjectClass("Piston"));
		boost::weak_ptr<ObjectClass> OC3 ( CommonInterface::getObjectClass("Vilebrequin"));

		//boost::weak_ptr<GraphClass> GC1 ( CommonInterface::getGraphClass("Shock Graph", true, false));
		boost::weak_ptr<GraphClass> GC2 ( CommonInterface::getGraphClass("Reeb Graph", true, false));
		boost::weak_ptr<GraphClass> GC3 ( CommonInterface::getGraphClass("SmoothGraph", false, true));

		
		boost::weak_ptr<Graph> GR1 (CommonInterface::getGraph(1));
		boost::weak_ptr<Graph> GR2 ( CommonInterface::getGraph(GC3,OC2,"SmoothPiston", 2));
		
		boost::weak_ptr<GraphClass> GC1 = GR1.lock()->getParentGraphClass();
		boost::weak_ptr<ObjectClass> OC1 = GR1.lock()->getParentObjectClass();

		system("pause");
		

		/* // ================= START GRAPH TEST =================

		cout << "Before update : shock1 name : "+ shock1.getObjectName() <<endl;
		shock1.setObjectName("Rod002.ppm");
		cout << "After update : shock1 name : "+ shock1.getObjectName() << endl;

		system ("PAUSE");

		cout << "Before update : shock1 view : "+ to_string((_ULonglong)shock1.getView()) <<endl;
		shock1.setView(2);
		cout << "After update : shock1 view : "+ to_string((_ULonglong)shock1.getView()) << endl;

		system ("PAUSE");

		cout << "Before update : shock1 key : "+ to_string((_ULonglong)shock1.getKey()) <<endl;
		shock1.setKey(3);
		cout << "After update : shock1 key : "+ to_string((_ULonglong)shock1.getKey()) <<endl;

		// ================= END GRAPH TEST ================= */

		/* // ================= START NODE TEST  =================

		cout << "Before update : n1 index : "+ to_string((_ULonglong)n1.getIndex()) <<endl;
		n1.setIndex(7);
		cout << "After update : n1 index : "+ to_string((_ULonglong)n1.getIndex()) << endl;

		system ("PAUSE");

		cout << "Before update : n1 Level : "+ to_string((_ULonglong)n1.getLevel()) <<endl;
		n1.setLevel(7);
		cout << "After update : n1 Level : "+ to_string((_ULonglong)n1.getLevel()) << endl;

		system ("PAUSE");

		cout << "Before update : n1 mass : "+ to_string((_ULonglong)n1.getMass()) <<endl;
		n1.setMass(7);
		cout << "After update : n1 mass : "+ to_string((_ULonglong)n1.getMass()) << endl;

		system ("PAUSE");

		cout << "Before update : n1 type : "+ to_string((_ULonglong)n1.getType()) <<endl;
		n1.setType(7);
		cout << "After update : n1 type : "+ to_string((_ULonglong)n1.getType()) << endl;

		system ("PAUSE");

		cout << "Before update : n1 label : "+ n1.getLabel() <<endl;
		n1.setLabel("Test");
		cout << "After update : n1 label : "+ n1.getLabel() << endl;

		system ("PAUSE");

		cout << "Before update : node key : "+ to_string((_ULonglong)n1.getKey()) <<endl;
		n1.setKey(3);
		cout << "After update : node key : "+ to_string((_ULonglong)n1.getKey()) << endl;

		cout << "Number of points in Node 1 : "+ to_string((_ULonglong)n1.getPointCount()) <<endl;
		cout << "Number of points in Node 2 : "+ to_string((_ULonglong)n2.getPointCount()) <<endl;

		// ================= END NODE TEST ================= */

		/* // ================= START GRAPHCLASS TEST  =================

		cout << "Before update : shockGraph IsDirect : "+ shockGraph.getIsDirect() << endl;
		shockGraph.setIsDirect(false);
		cout << "After update : shockGraph IsDirect : "+ shockGraph.getIsDirect() << endl;

		system ("PAUSE");

		cout << "Before update : shockGraph label : "+ shockGraph.getIsAcyclicGraph() <<endl;
		shockGraph.setIsAcyclicGraph(false);
		cout << "After update : shockGraph label : "+ shockGraph.getIsAcyclicGraph() << endl;

		system ("PAUSE");

		cout << "Before update : shockGraph key : "+  shockGraph.getKey() <<endl;
		shockGraph.setKey("Shocking Graph");
		cout << "After update : shockGraph key : "+ shockGraph.getKey() << endl;

		// ================= END NODE TEST ================= */

		/* // ================= START OBJECTCLASS TEST  =================

		cout << "Before update : rod key : "+  rod.getKey() <<endl;
		rod.setKey("Bielle");
		cout << "After update : rod key : "+ rod.getKey() << endl;

		// ================= END NODE TEST ================= */

		/* // ================= START EDGE TEST  =================

		cout << "Before update : e1 weight : "+ to_string((_ULonglong)e1.getWeight()) <<endl;
		e1.setWeight(13);
		cout << "After update : e1 weight : "+ to_string((_ULonglong)e1.getWeight()) << endl;

		system ("PAUSE");

		cout << "Before update : edge key : "+ to_string((_ULonglong)e1.getKey()) <<endl;
		e1.setKey(3);
		cout << "After update : edge key : "+ to_string((_ULonglong)e1.getKey()) << endl;

		// ================= END EDGE TEST ================= */

		// ================= START POINT TEST  =================

		//p1 = loadObject<Point>(1);
		/*
		cout << "Before update : point xCoord : "+ to_string((_ULonglong)p1->getxCoord()) <<endl;
		p1->setxCoord(13);
		cout << "After update : point xCoord : "+ to_string((_ULonglong)p1->getxCoord()) << endl;

		system ("PAUSE");

		cout << "Before update : point yCoord : "+ to_string((_ULonglong)p1->getyCoord()) <<endl;
		p1->setyCoord(13);
		cout << "After update : point yCoord : "+ to_string((_ULonglong)p1->getyCoord()) << endl;

		system ("PAUSE");

		cout << "Before update : point radius : "+ to_string((_ULonglong)p1->getRadius()) <<endl;
		p1->setRadius(13);
		cout << "After update : point radius : "+ to_string((_ULonglong)p1->getRadius()) << endl;

		system ("PAUSE");

		cout << "Before update : point key : "+ to_string((_ULonglong)p1->getKey()) <<endl;
		p1->setKey(3);
		cout << "After update : point key : "+ to_string((_ULonglong)p1->getKey()) << endl;

		// ================= END EDGE TEST ================= */
}

/* *******************************************************************
*                          COMMON INTERFACE                          *
 ********************************************************************/

boost::weak_ptr<Point> GraphManager::CommonInterface::getPoint(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Point>>::const_iterator it = PointMap.find(keyDB);
	if(it != PointMap.end())
		return  boost::weak_ptr<Point>(it->second);
	else{
		boost::shared_ptr<Point> tmp (loadObject<Point> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept("GraphManager::CommonInterface::getPoint(const unsigned long  keyDB)", "Error : The Point referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(keyDB,tmp));
		return boost::weak_ptr<Point>(tmp);
	}
}
boost::weak_ptr<Point> GraphManager::CommonInterface::getPoint(const boost::weak_ptr<Node> _refNode, const boost::weak_ptr<Graph> _refGraph, const double _xCoord, const double _yCoord, const double _radius){
	boost::shared_ptr<Point> tmp (Point::Access::createPoint(_refNode, _refGraph, _xCoord, _yCoord, _radius));
	PointMap.insert(pair<unsigned long, boost::shared_ptr<Point>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Point>(tmp);
}

boost::weak_ptr<Node> GraphManager::CommonInterface::getNode(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Node>>::const_iterator it = NodeMap.find(keyDB);
	if(it != NodeMap.end())
		return  boost::weak_ptr<Node>(it->second);
	else{
		boost::shared_ptr<Node> tmp (loadObject<Node> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept("GraphManager::CommonInterface::getNode(const unsigned long keyDB)", "Error : The Node referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		NodeMap.insert(pair<unsigned long , boost::shared_ptr<Node>>(keyDB,tmp));
		return boost::weak_ptr<Node>(tmp);
	}
}
boost::weak_ptr<Node> GraphManager::CommonInterface::getNode(const boost::weak_ptr<Graph> _refGraph, const unsigned int _index, const unsigned int _level, const unsigned int _mass, const unsigned int _type, const string _label){
	boost::shared_ptr<Node> tmp (Node::Access::createNode(_refGraph, _index, _level, _mass, _type, _label));
	NodeMap.insert(pair<unsigned long, boost::shared_ptr<Node>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Node>(tmp);
}

boost::weak_ptr<Edge> GraphManager::CommonInterface::getEdge(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Edge>>::const_iterator it = EdgeMap.find(keyDB);
	if(it != EdgeMap.end())
		return  boost::weak_ptr<Edge>(it->second);
	else{
		boost::shared_ptr<Edge> tmp (loadObject<Edge> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept("GraphManager::CommonInterface::getEdge(const unsigned long keyDB)", "Error : The Edge referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(keyDB,tmp));
		return boost::weak_ptr<Edge>(tmp);
	}
}
boost::weak_ptr<Edge> GraphManager::CommonInterface::getEdge(const boost::weak_ptr<Node> _source, const boost::weak_ptr<Node> _target, const boost::weak_ptr<Graph> _refGraph, const unsigned int _weight){
	boost::shared_ptr<Edge> tmp (Edge::Access::createEdge(_source, _target, _refGraph, _weight));
	EdgeMap.insert(pair<unsigned long, boost::shared_ptr<Edge>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Edge>(tmp);
}

boost::weak_ptr<Graph> GraphManager::CommonInterface::getGraph(const unsigned long keyDB) throw(ShapeLearnerExcept){
	map<unsigned long, boost::shared_ptr<Graph>>::const_iterator it = GraphMap.find(keyDB);
	if(it != GraphMap.end())
		return boost::weak_ptr<Graph>(it->second);
	else{
		boost::shared_ptr<Graph> tmp (loadObject<Graph> (keyDB));
		if (!tmp)
			throw ShapeLearnerExcept("GraphManager::CommonInterface::getGraph(const unsigned long keyDB)", "Error : The Graph referenced by the id : "+ to_string((_ULonglong)keyDB) +" doesn't exist in the DB.");
		GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(keyDB,tmp));
		return boost::weak_ptr<Graph>(tmp);
	}
}
boost::weak_ptr<Graph> GraphManager::CommonInterface::getGraph(const boost::weak_ptr<GraphClass> _graphClass, const boost::weak_ptr<ObjectClass> _objectClass, const string _objectName, const unsigned int _viewNumber){
	boost::shared_ptr<Graph> tmp (Graph::Access::createGraph(_graphClass, _objectClass, _objectName, _viewNumber));
	GraphMap.insert(pair<unsigned long, boost::shared_ptr<Graph>>(tmp->getKey(), tmp));
	return boost::weak_ptr<Graph>(tmp);
}

boost::weak_ptr<GraphClass> GraphManager::CommonInterface::getGraphClass(const string& name, const bool isDirect, const bool isAcyclic) throw(ShapeLearnerExcept){
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

boost::weak_ptr<ObjectClass> GraphManager::CommonInterface::getObjectClass(const string& name) throw(ShapeLearnerExcept){
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

void GraphManager::setDbType() throw(ShapeLearnerExcept){
	string tmp;

	do{
		cout << "PostgreSQL : 1 // MySQL : 2 // Oracle : 3"<<endl;
		cout << "What type of database would you like to use : ";

		getline( std::cin, tmp );
	}while (stoul(tmp) < 1 ||  stoul(tmp) > 3);

	dbType = stoul(tmp);

	switch(dbType)
	{
		case constants::DB_MYSQL :
			dbPort = 3306;
			break;
		case constants::DB_PGSQL :
			dbPort = 5432;
			break;
		case constants::DB_ORACLE :
			dbPort = 1521;
			break;
	}
}

void GraphManager::setDbPort() throw(ShapeLearnerExcept){
	string tmp;

	do{
		cout << "Please enter the listening port of your database server [DEFAULT = "+ std::to_string((_ULonglong)dbPort) +"] : ";
		getline( std::cin, tmp );
	}while (!tmp.empty() && stoul(tmp) <=0);

	if (!tmp.empty()){
		dbPort =  stoul(tmp);
	}
}

void GraphManager::setDbName() throw(ShapeLearnerExcept){
	string tmp;

	do{
			cout << "Please enter the name of your database : ";
			getline( std::cin, tmp );
	}while (tmp.empty());

	if(!tmp.empty())
			dbName = tmp;
}

void GraphManager::setDbUser() throw(ShapeLearnerExcept){
	string tmp;

	do{
		cout << "Please enter your username : ";
		getline( std::cin, tmp );
	}while (tmp.empty());

	if(!tmp.empty())
			dbUser = tmp;
}

void GraphManager::setDbPass() throw(ShapeLearnerExcept){
	string tmp;

	cout << "Please enter your password : ";
	getline( std::cin, tmp );

	if(!tmp.empty())
			dbPass = tmp;
}

void GraphManager::setDbHost() throw(ShapeLearnerExcept){
	string tmp;

	cout << "Please enter the IP address of your database server [DEFAULT = "+ dbHost +"] : ";
	getline( std::cin, tmp );

	if (!tmp.empty())
		dbHost = tmp;
}

void GraphManager::setDBInitFile() throw(ShapeLearnerExcept){
	#ifndef _DEBUG
		string tmp;
		cout << "Please enter the relative path to the file [DEFAULT = "+ dbInitFile +"] : ";
		getline( std::cin, tmp );

		if (!tmp.empty())
			dbInitFile = tmp;
	#endif
}