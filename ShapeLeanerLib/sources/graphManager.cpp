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
bool GraphManager::dbInit = false;
unsigned int GraphManager::dbType = constants::DB_PGSQL;

/* *******************************************************************
*                            Singleton                               *
 ********************************************************************/

GraphManager* GraphManager::s_inst = NULL;

GraphManager& GraphManager::openManager(){
	if( GraphManager::s_inst == NULL )
		GraphManager::s_inst =  new GraphManager();
	return (*GraphManager::s_inst);

};

void GraphManager::destroy(){
	
	DatabaseManager::Key::destroy();
	if( s_inst != NULL ){
		delete GraphManager::s_inst;
		s_inst = NULL;
	}
}

GraphManager::GraphManager() : dBManager(DatabaseManager::Key::getInstance(dbUser, dbPass, dbName, dbHost, dbPort, dbType, dbInit)){}


void GraphManager::test() throw(ShapeLearnerExcept){
	
		GraphClass shockGraph ("Shock Graph", true, true);
		
		ObjectClass rod ("Rod");
		
		Graph shock1 (&shockGraph, &rod, 1, "Rod001.ppm");
		
		Node n1 (1,2,3,4,5,6, &shock1);
		Node n2 (6,5,4,3,2,1, &shock1);
		
		Point p1(1,2,3,&shock1,&n1);
		Point p2(1,2,3,&shock1,&n2);
		
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
*                        Command Line Parsing                        *
 ********************************************************************/

void GraphManager::parseCommandLine(int argc, char **argv) throw(ShapeLearnerExcept){
	
	//We initialize the command Line Parser.
	CCmdLine cmdLine;
	
	try{
		//We parse the arguments given by command line.
		if (cmdLine.SplitLine(argc,argv) < 2){ // If not enough arguments are given, we show the help.

			#ifndef _DEBUG
				cmdLine.ShowHelp("help.txt");
				system ("PAUSE");
				exit(EXIT_FAILURE);
			#endif
		}


		// Did the user requested any 'help' ?
		if (cmdLine.HasSwitch("-h")|| cmdLine.HasSwitch("-help") || cmdLine.HasSwitch("--help")){
			cmdLine.ShowHelp("help.txt");
			system ("PAUSE");
			exit(EXIT_SUCCESS);
		}

		// Did the user asked to use a previously created database ?
		if (cmdLine.HasSwitch("--init")){
			dbInit = true;
		}

		// On récupère les identifiants de connexion à la BDD
		#ifndef _DEBUG
			getDbCredentials(); // En Debug les identifiants sont HardCoded.
		#endif
		
		openManager();		
	}
	catch (const std::exception &e ) 
	{ 
		cerr << e.what(); 

		system ("PAUSE");
		exit(EXIT_FAILURE);
	}
}

/* *******************************************************************
*                              Setters                               *
 ********************************************************************/

void GraphManager::getDbCredentials() throw(ShapeLearnerExcept){
	setDbType();
	setDbHost();
	setDbPort();
	setDbName();
	setDbUser();
	setDbPass();
}

void GraphManager::setDbType() throw(ShapeLearnerExcept){
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbType", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
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
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbPort", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
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
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbName", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	string tmp;

	do{
			cout << "Please enter the name of your database : ";
			getline( std::cin, tmp );
	}while (tmp.empty());

	if(!tmp.empty())
			dbName = tmp;

}

void GraphManager::setDbUser() throw(ShapeLearnerExcept){
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbUser", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	string tmp;

	do{
		cout << "Please enter your username : ";
		getline( std::cin, tmp );
	}while (tmp.empty());

	if(!tmp.empty())
			dbUser = tmp;

}

void GraphManager::setDbPass() throw(ShapeLearnerExcept){
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbPass", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	string tmp;

	cout << "Please enter your password : ";
	getline( std::cin, tmp );

	if(!tmp.empty())
			dbPass = tmp;
}

void GraphManager::setDbHost() throw(ShapeLearnerExcept){
	if (GraphManager::s_inst != NULL)
		throw ShapeLearnerExcept("GraphManager::setDbHost", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
		
	string tmp;

	cout << "Please enter the IP address of your database server [DEFAULT = "+ dbHost +"] : ";
	getline( std::cin, tmp );
		
	if (!tmp.empty())
		dbHost = tmp;
}

/* *******************************************************************
*                            Destructor                              *
 ********************************************************************/

GraphManager::~GraphManager(){};
