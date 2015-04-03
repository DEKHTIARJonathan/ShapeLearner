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
*	\brief Program calling the ShapeLearnerLib 
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
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
		//We parse the arguments given by command line.
		if (cmdLine.SplitLine(argc,argv) < 1){ // If not enough arguments are given, we show the help.
			#ifndef _DEBUG
				cmdLine.ShowHelp("help.txt");
				system ("PAUSE");
				exit(EXIT_FAILURE);
			#endif
		}

		// Did the user request any 'help' ?
		if (cmdLine.HasSwitch("-h")|| cmdLine.HasSwitch("-help") || cmdLine.HasSwitch("--help")){
			cmdLine.ShowHelp("help.txt");
			system ("PAUSE");
			exit(EXIT_SUCCESS);
		}

		// Did the user asked to use a previously created database ?
		if (cmdLine.HasSwitch("--init"))
			ShapeLearner::openDatabase("postgres", "postgres", "postgres", "localhost", 5432, "sources/structure.sql");
		else		
			ShapeLearner::openDatabase("postgres", "postgres", "postgres", "localhost", 5432);

		ShapeLearner::closeDatabase(); // We disconnect to the DB

	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		system ("PAUSE");
		return EXIT_FAILURE;
	}

	cout<<endl<<endl;
	system ("PAUSE");
	return EXIT_SUCCESS;
}