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
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;

/* *******************************************************************
*                        Static DB Parameters                        *
 ********************************************************************/

QString GraphManager::dbName = "temp.shape";
QString GraphManager::dbPath = QDir::currentPath();
QString GraphManager::dbUser = "";
QString GraphManager::dbPass = "";
QString GraphManager::dbPort = 0;
QString GraphManager::dbHost = "localhost";
bool GraphManager::dbInit = false;
unsigned int GraphManager::dbType = constants::DB_SQLITE;

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

GraphManager::GraphManager() : dBManager(DatabaseManager::Key::getInstance(dbName, dbPath, dbUser, dbPass, dbHost, dbPort, dbType, dbInit)){}

/* *******************************************************************
*                        Command Line Parsing                        *
 ********************************************************************/

void GraphManager::parseCommandLine(int argc, char **argv) throw(GraphManagerExcept){
	
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
		getDbCredentials();
		
		openManager();
		/*
		dbName = QString::fromStdString(cmdLine.GetArgument("-c", 0));
		cout << "db Name = " << dbName.toStdString() << endl;
		*/
		
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

void GraphManager::getDbCredentials() throw(GraphManagerExcept){
			setDbType();
			setDbHost();
			setDbPort();
			setDbPath();
			setDbName();
			setDbUser();
			setDbPass();
}

void GraphManager::setDbType() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbType", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp = "0";
	QTextStream qtin(stdin); 

	while ( tmp.toInt() < 1 ||  tmp.toInt() > 4)
	{
		
		cout << "SQLite : 1 // PostgreSQL : 2 // MySQL : 3 // Oracle : 4"<<endl;
		cout << "What type of database would you like to use : ";
		qtin >> tmp;

		dbType = tmp.toUInt();
	}

	switch(dbType)
	{
		case constants::DB_SQLITE :
			dbPort = "0";
			break;
		case constants::DB_MYSQL :
			dbPort = "3306";
			break;
		case constants::DB_PGSQL :
			dbPort = "5432";
			break;
		case constants::DB_ORACLE :
			dbPort = "1521";
			break;
	}
}

void GraphManager::setDbPort() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbPort", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	if (dbType != constants::DB_SQLITE)
	{
		string tmp;

		do{
			cout << "Please enter the listening port of your database server [DEFAULT = "+ dbPort.toStdString() +"] : ";
			getline( std::cin, tmp );
		}while (!tmp.empty() && atoi(tmp.c_str()) <=0);

		if (!tmp.empty()){
			dbPort =  QString::fromStdString(tmp);
		}
	}

	
}

void GraphManager::setDbName() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbName", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	string tmp;

	do{
			cout << "Please enter the name of your database : ";
			getline( std::cin, tmp );
	}while (tmp.empty());

	if(!tmp.empty())
			dbName = QString::fromStdString(tmp);

}

void GraphManager::setDbPath() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbPath", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");

	if (dbType == constants::DB_SQLITE)
	{
		string tmp;

		cout << "Entrez le path de votre bdd: [Default = "+ dbPath.toStdString() +"] : ";
		getline( std::cin, tmp );

		if(!tmp.empty())
			dbPath = QString::fromStdString(tmp);

	}
	
	QString tmp;
	QTextStream qtin(stdin); 
	

	if (tmp.compare(""))
		dbPath = tmp;
}

void GraphManager::setDbUser() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbUser", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	if (dbType != constants::DB_SQLITE)
	{

		string tmp;

		do{
			cout << "Please enter your username : ";
			getline( std::cin, tmp );
		}while (tmp.empty());

		if(!tmp.empty())
				dbUser = QString::fromStdString(tmp);

	}

}

void GraphManager::setDbPass() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbPass", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	if (dbType != constants::DB_SQLITE)
	{
		string tmp;

		cout << "Please enter your password : ";
		getline( std::cin, tmp );

		if(!tmp.empty())
				dbUser = QString::fromStdString(tmp);
	}
}

void GraphManager::setDbHost() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbHost", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	if (dbType != constants::DB_SQLITE)
	{
		dbHost = "127.0.0.1";
		
		string tmp;

		cout << "Please enter the IP address of your database server [DEFAULT = "+dbHost.toStdString()+"] : ";
		getline( std::cin, tmp );
		
		if (!tmp.empty())
			dbHost = QString::fromStdString(tmp);
	}
}


/* *******************************************************************
*                            Destructor                              *
 ********************************************************************/

GraphManager::~GraphManager(){};
