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
	//We initialize the command Line Parser.
	CCmdLine cmdLine;


	try{
		//We parse the arguments given by command line.
		if (cmdLine.SplitLine(argc,argv) < 1)
		{
			assert(0);
			system ("PAUSE");
			return EXIT_FAILURE;
		}

		// test for the 'help' case
		if (cmdLine.HasSwitch("-h")|| cmdLine.HasSwitch("-help") || cmdLine.HasSwitch("--help"))
		{
			cmdLine.ShowHelp("help.txt");
			system ("PAUSE");
			return EXIT_SUCCESS;
		}

		// test to get argument for a determined switch
		string p1 = cmdLine.GetArgument("-p1", 0);
		cout << "Arguments for '-p1' : "<< p1 <<endl;

		// test to get argument for a determined switch
		string p2 = cmdLine.GetArgument("-p2", 0);
		cout << "Arguments for '-p2' : "<< p2 <<endl;

		string p3_0 = cmdLine.GetArgument("-p3", 0);
		string p3_1 = cmdLine.GetArgument("-p3", 1);
		cout << "Arguments for '-p2' : "<< p3_0 << " && " << p3_1 << endl;

		
	}
	catch (const std::exception &e ) 
	{ 
		cerr << e.what(); 
		system ("PAUSE");
		return EXIT_FAILURE;
	}


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

	
	system ("PAUSE");
	
	return EXIT_SUCCESS;
}