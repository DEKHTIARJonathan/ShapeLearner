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

#include <iostream>
#include "CmdLine.h"
#include "shapeLearner.h"

using namespace std;

int main(int argc, char **argv)
{
	
	try
	{
		
		//We initialize the command Line Parser.
		CmdLine cmdLine;
	
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
			if (cmdLine.HasSwitch("--init"))
				ShapeLearner::getDbCredentials(true); // En Debug les identifiants sont HardCoded.
			else
				ShapeLearner::getDbCredentials(); // En Debug les identifiants sont HardCoded.

			// On récupère les identifiants de connexion à la BDD
			
			ShapeLearner::openDatabase(); // We connect to the DB

			ShapeLearner::test(); // We act on the program

			ShapeLearner::closeDatabase(); // We disconnect to the DB


		}
		catch (const std::exception &e ) 
		{ 
			cerr << e.what(); 
			system ("PAUSE");
			exit(EXIT_FAILURE);
		}
		
		
		
		//GraphManager::parseCommandLine(argc,argv);
		//GraphManager::openManager().test();
		
		/*
		auto_ptr<database> db (new odb::pgsql::database ("postgres","postgres","postgres","127.0.0.1", 5433));

		// Create a few persistent person objects.
		//
		{
			GraphClass graphType1 ("testGraph1", true, true);
			GraphClass graphType2 ("testGraph2", true, true);
			GraphClass graphType3 ("testGraph3", true, true);
			GraphClass graphType4 ("testGraph4", true, true);
			GraphClass graphType5 ("testGraph5", true, true);

			transaction t (db->begin ());

			t.tracer (stderr_tracer);

			// Make objects persistent and save their ids for later use.
			//
			db->persist (graphType1);
			db->persist (graphType2);
			db->persist (graphType3);
			db->persist (graphType4);
			db->persist (graphType5);

			t.commit ();
		}
		*/
	}
	catch (const std::exception& e)
	{
		cerr << e.what () << endl;
		system ("PAUSE");
		return 1;
	}

	//

	/*
	try{
		GraphManager::setDbName();
	}
	catch (const std::exception &e ) 
	{ 
		cerr << e.what(); 
		system ("PAUSE");
		return EXIT_FAILURE;
	}

	GraphManager& software = GraphManager::openManager();
	*/
	
	system ("PAUSE");
	
	return EXIT_SUCCESS;
}