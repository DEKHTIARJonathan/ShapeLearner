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
using namespace odb::core;

int main(int argc, char **argv)
{
	
	try
	{
		
		GraphManager::parseCommandLine(argc,argv);
		GraphManager::openManager().test();
		
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