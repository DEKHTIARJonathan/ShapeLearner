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
*                           USER INTERFACE                           *
 ********************************************************************/

void GraphManager::UserInterface::openDatabase() {
	DatabaseManager::Interface::openDatabase(dbUser, dbPass, dbName, dbHost, dbPort, dbType, dbInitFile);
}

void GraphManager::UserInterface::closeDatabase() throw(ShapeLearnerExcept) {
	DatabaseManager::Interface::closeDatabase();
} 

void GraphManager::UserInterface::getDbCredentials(const bool dbInit) throw(ShapeLearnerExcept){
	//cout << "Hello";
	//cout << " Brotherhuhu";
	//bool a = true;
	//a &= false;

	if (DatabaseManager::Interface::isDbOpen()){
		throw ShapeLearnerExcept("GraphManager::UserInterface::getDbCredentials", "Error : The Database has already been instantiated. It's impossible to modify the Database's parameters");
	}	
	else{
		if (dbInit){
			setDBInitFile();
		}
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
	
		GraphClass shockGraph ("Shock Graph", true, true);
		
		ObjectClass rod ("Rod");
		
		Graph shock1( &shockGraph, &rod, "Rod001.ppm", 1);
		
		Node n1 (&shock1, 1,2,3,4,"#");
		Node n2 (&shock1, 5,4,3,2,"A");
		
		Point p1(&n1,&shock1,1,2,3);
		Point p2(&n1,&shock1,1,2,3);
		
		Edge e1(&n1, &n2, &shock1, 1);
		Edge e2(&n2, &n1, &shock1, 1);
		
		system ("PAUSE");

		DatabaseManager::Interface::test();

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

		/* // ================= START POINT TEST  =================


		cout << "Before update : point xCoord : "+ to_string((_ULonglong)p1.getxCoord()) <<endl;
		p1.setxCoord(13);
		cout << "After update : point xCoord : "+ to_string((_ULonglong)p1.getxCoord()) << endl;

		system ("PAUSE");

		cout << "Before update : point yCoord : "+ to_string((_ULonglong)p1.getyCoord()) <<endl;
		p1.setyCoord(13);
		cout << "After update : point yCoord : "+ to_string((_ULonglong)p1.getyCoord()) << endl;

		system ("PAUSE");

		cout << "Before update : point radius : "+ to_string((_ULonglong)p1.getRadius()) <<endl;
		p1.setRadius(13);
		cout << "After update : point radius : "+ to_string((_ULonglong)p1.getRadius()) << endl;

		system ("PAUSE");

		cout << "Before update : point key : "+ to_string((_ULonglong)p1.getKey()) <<endl;
		p1.setKey(3);
		cout << "After update : point key : "+ to_string((_ULonglong)p1.getKey()) << endl;


		// ================= END EDGE TEST ================= */


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
	string tmp = "sources/structure.sql";

	#ifdef _DEBUG
		dbInitFile = "sources/structure.sql";
	#else 
		cout << "Please enter the relative path to the file [DEFAULT = "+ tmp +"] : ";
		getline( std::cin, tmp );
	#endif
		
	dbInitFile = tmp;
}