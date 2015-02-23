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
QString GraphManager::user = "";
QString GraphManager::pass = "";
QString GraphManager::hostname = "localhost";

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

GraphManager::GraphManager() : dBManager(DatabaseManager::Key::getInstance(dbName, dbPath, user, pass, hostname)){}

/* *******************************************************************
*                              Setters                               *
 ********************************************************************/

void GraphManager::setDbName() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbName", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp;
	QTextStream qtin(stdin);
	cout << "Entrez le nom de votre bdd: ";
	qtin >> tmp;

	if (tmp.compare(""))
		dbName = tmp;
}

void GraphManager::setDbPath() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbPath", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp;
	QTextStream qtin(stdin); 
	cout << "Entrez le path de votre bdd: ";
	qtin >> tmp;

	if (tmp.compare(""))
		dbPath = tmp;
}

void GraphManager::setDbUser() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbUser", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp;
	QTextStream qtin(stdin); 
	cout << "Entrez le nom d'utilisateur de votre bdd: ";
	qtin >> tmp;

	if (tmp.compare(""))
		user = tmp;
}

void GraphManager::setDbPass() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbPass", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp;
	QTextStream qtin(stdin); 
	cout << "Entrez le mot de passe de votre bdd: ";
	qtin >> tmp;

	if (tmp.compare(""))
		pass = tmp;
}

void GraphManager::setDbHost() throw(GraphManagerExcept){
	if (GraphManager::s_inst != NULL)
		throw GraphManagerExcept("GraphManager::setDbHost", "Error : The GraphManager has already been instantiated. It's impossible to modify the Database's parameters");
	
	QString tmp;
	QTextStream qtin(stdin); 
	cout << "Entrez l'adresse du serveur de votre bdd: ";
	qtin >> tmp;

	if (tmp.compare(""))
		hostname = tmp;
}

/* *******************************************************************
*                            Destructor                              *
 ********************************************************************/

GraphManager::~GraphManager(){};
