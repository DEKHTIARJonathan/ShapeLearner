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
*	\brief Program calling the GraphDBLib 
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

//#define _CITUS_

#include <iostream>
#include "CmdLine.h"
#include "graphDB.h"
#include "StandardException.h"
#include "ShapeLearner.h"

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

		
		if (cmdLine.HasSwitch("--init")){
			#ifdef _CITUS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10026, "sources/structure.sql");
			#else
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10024, "sources/structure.sql");
			#endif
		}
		else
		{
			#ifdef _CITUS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10026);
			#else
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10024);
			#endif
		}
			
		if (cmdLine.HasSwitch("--generate")){
			vector<const string> imgVect;

			imgVect.push_back("img/rod1.ppm");
			imgVect.push_back("img/rod2.ppm");
			imgVect.push_back("img/rod3.ppm");
			imgVect.push_back("img/rod4.ppm");
			imgVect.push_back("img/rod5.ppm");
			imgVect.push_back("img/rod6.ppm");
			imgVect.push_back("img/rod7.ppm");
			imgVect.push_back("img/rod8.ppm");
			imgVect.push_back("img/rod9.ppm");
			imgVect.push_back("img/rod10.ppm");
			imgVect.push_back("img/rod11.ppm");
			imgVect.push_back("img/rod12.ppm");
			imgVect.push_back("img/rod13.ppm");
			imgVect.push_back("img/rod13.ppm");
			imgVect.push_back("img/rod14.ppm");
			imgVect.push_back("img/rod15.ppm");
			imgVect.push_back("img/rod16.ppm");
			imgVect.push_back("img/rod17.ppm");
			imgVect.push_back("img/rod18.ppm");
			imgVect.push_back("img/rod19.ppm");
			imgVect.push_back("img/rod20.ppm");
			ShapeLearner::createShockGraph(imgVect);
		}
		else
			Logger::Log("No action specified", constants::LogError);

		GraphDB::closeDatabase(); // We disconnect to the DB
		
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