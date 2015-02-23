/* ************ Begin file main.cpp **************************************
**
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
*	\file main.cpp
*	\brief Fichier contenant la fonction centrale : main()
*	\version 1.0
*	\author DEKHTIAR Jonathan
*/

#include "allHeaders.h"
using namespace std;


int main(int argc, char* argv[])
{
	
	try{
		GraphManager::setDbName();
	}
	catch (const std::exception &e ) 
	{ 
		cerr << e.what(); 
		system ("PAUSE");
		return EXIT_FAILURE;
	}

	//GraphManager& software = GraphManager::openManager();

	/*
	
	try {
		DatabaseManager& db = DatabaseManager::getInstance(dbName);
		
	}
	catch (const std::exception &e ) 
	{ 
		cerr << e.what(); 
		system ("PAUSE");
		return EXIT_FAILURE;
	}
	
	QString dbPath = QDir::currentPath() + "/" + dbName;
	
	*/
	
	system ("PAUSE");
	
	return EXIT_SUCCESS;
}