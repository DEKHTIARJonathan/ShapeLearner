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
string GraphManager::dbInitFile = "";
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
	if (DatabaseManager::Interface::isDbOpen())
		throw ShapeLearnerExcept("GraphManager::UserInterface::getDbCredentials", "Error : The Database has already been instantiated. It's impossible to modify the Database's parameters");
	else{
		#ifndef _DEBUG
			setDbType();
			setDbHost();
			setDbPort();
			setDbName();
			setDbUser();
			setDbPass();
		#endif
		if (dbInit)
			setDBInitFile();
	}
}

void GraphManager::UserInterface::test() throw(ShapeLearnerExcept){
	
		GraphClass shockGraph ("Shock Graph", true, true);
		
		ObjectClass rod ("Rod");
		
		Graph shock1( &shockGraph, &rod, "Rod001.ppm", 1);
		
		Node n1 (&shock1, 1,2,3,4,"#");
		Node n2 (&shock1, 5,4,3,2,"A");
		
		Point p1(&n1,&shock1,1,2,3);
		Point p2(&n2,&shock1,1,2,3);
		
		Edge e1(&n1, &n2, &shock1, 1);
		Edge e2(&n2, &n1, &shock1, 1);
		
		//system ("PAUSE");

		/*
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
		cout << "After update : shock1 key : "+ to_string((_ULonglong)shock1.getKey()) << endl;
		*/
		
		/*
		cout << "Before update : node key : "+ to_string((_ULonglong)n1.getKey()) <<endl;
		n1.setKey(3);
		cout << "After update : node key : "+ to_string((_ULonglong)n1.getKey()) << endl;
		*/

		/*
		cout << "Before update : edge key : "+ to_string((_ULonglong)e1.getKey()) <<endl;
		e1.setKey(3);
		cout << "After update : edge key : "+ to_string((_ULonglong)e1.getKey()) << endl;
		*/

		/*
		cout << "Before update : point key : "+ to_string((_ULonglong)p1.getKey()) <<endl;
		p1.setKey(3);
		cout << "After update : point key : "+ to_string((_ULonglong)p1.getKey()) << endl;
		*/

		/*
		cout << "Before update : GraphClass key : "+ shockGraph.getKey() <<endl;
		shockGraph.setKey("Shock Graph2");		
		cout << "After update : GraphClass name : "+ shockGraph.getKey() << endl;
		*/

		/*
		cout << "Before update : ObjectClass key : "+ rod.getKey() <<endl;
		rod.setKey("Rod2");		
		cout << "After update : ObjectClass name : "+ rod.getKey() << endl;
		*/


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

	cout << "Please enter the relative path to the file [DEFAULT = "+ tmp +"] : ";
	getline( std::cin, tmp );
		
	dbInitFile = tmp;
}