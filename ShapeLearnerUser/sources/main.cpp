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
#include "DAGMatcherLib.h"

using namespace std;
using namespace graphDBLib;

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

			dml::DAGMatcherLib::InitDAGMatcherLib();
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
			vector<const img2Parse> imgVect;
			imgVect.push_back(img2Parse("img/rod1.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod2.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod3.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod4.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod5.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod6.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod7.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod8.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod9.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod10.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod11.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod12.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod13.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod13.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod14.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod15.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod16.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod17.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod18.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod19.ppm", "Rod"));
			imgVect.push_back(img2Parse("img/rod20.ppm", "Rod"));

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