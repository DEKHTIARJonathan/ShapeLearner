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
//#define _AWS_ // Amazon Web Service BDD

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

				exit(EXIT_FAILURE);
			#endif
		}

		// Did the user request any 'help' ?
		if (cmdLine.HasSwitch("-h")|| cmdLine.HasSwitch("-help") || cmdLine.HasSwitch("--help")){
			cmdLine.ShowHelp("help.txt");

			exit(EXIT_SUCCESS);
		}

		if (cmdLine.HasSwitch("--init")){
			#ifdef _CITUS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10026, "sources/structure.sql");
			#else
				#ifdef _AWS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "52.17.230.141", 10003, "sources/structure.sql");
				#else
					GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10024, "sources/structure.sql");
				#endif
			#endif

			dml::DAGMatcherLib::InitDAGMatcherLib();
		}
		else
		{
			#ifdef _CITUS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10026);
			#else
				#ifdef _AWS_
				GraphDB::openDatabase("postgres", "postgres", "postgres", "52.17.230.141", 10003);
				#else
					GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10024);
				#endif
			#endif
		}

		if (cmdLine.HasSwitch("--generate")){
			vector<const img2Parse> imgVect;

			#ifdef _DEBUG
				imgVect.push_back(img2Parse("img/rod.ppm", "Rod"));
			#else
				for (int i = 1; i <= 82; i++){
					string seq = "";
					if (i < 10)
						seq = "00"+ to_string((_Longlong)i);
					else if (i <100)
						seq = "0"+to_string((_Longlong)i);
					else
						seq = to_string((_Longlong)i);
					string imgPath = "img/PPM/LearningSet/GEAR/gear" + seq + ".ppm";
					imgVect.push_back(img2Parse(imgPath, "Gear"));
				}

				for (int i = 1; i <= 68; i++){
					string seq = "";
					if (i < 10)
						seq = "00"+ to_string((_Longlong)i);
					else if (i <100)
						seq = "0"+to_string((_Longlong)i);
					else
						seq = to_string((_Longlong)i);
					string imgPath = "img/PPM/LearningSet/PISTON/piston" + seq + ".ppm";
					imgVect.push_back(img2Parse(imgPath, "Piston"));
				}

				for (int i = 1; i <= 82; i++){
					string seq = "";
					if (i < 10)
						seq = "00"+ to_string((_Longlong)i);
					else if (i <100)
						seq = "0"+to_string((_Longlong)i);
					else
						seq = to_string((_Longlong)i);
					string imgPath = "img/PPM/LearningSet/PistonAssembly/PistonAssembly" + seq + ".ppm";
					imgVect.push_back(img2Parse(imgPath, "PistonAssembly"));
				}

				for (int i = 1; i <= 65; i++){
					string seq = "";
					if (i < 10)
						seq = "00"+ to_string((_Longlong)i);
					else if (i <100)
						seq = "0"+to_string((_Longlong)i);
					else
						seq = to_string((_Longlong)i);
					string imgPath = "img/PPM/LearningSet/ROD/rod" + seq + ".ppm";
					imgVect.push_back(img2Parse(imgPath, "Rod"));
				}
			#endif

			ShapeLearner::createShockGraph(imgVect);
		}
		else
			Logger::Log("No action specified", constants::LogError);

		GraphDB::closeDatabase(); // We disconnect to the DB
	}
	catch (const std::exception& e)
	{
		Logger::Log(e.what (), constants::LogError);
		return EXIT_FAILURE;
	}

	cout<<endl<<endl;
	#ifdef _DEBUG
		system("PAUSE");
	#endif
	return EXIT_SUCCESS;
}