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
#define _AWS_ // Amazon Web Service BDD
//#define _TESTDATA_

#include <iostream>
#include "boost/filesystem.hpp"
#include "CmdLine.h"
#include "graphDB.h"
#include "StandardException.h"
#include "ShapeLearner.h"
#include "DAGMatcherLib.h"

using namespace std;
using namespace boost::filesystem;
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
					#ifndef _TESTDATA_
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10000, "sources/structure.sql");
					#else
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10001, "sources/structure.sql");
					#endif
				#else
					GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10023, "sources/structure.sql");
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
					#ifndef _TESTDATA_
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10000);
					#else
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10001);
					#endif
				#else
					GraphDB::openDatabase("postgres", "postgres", "postgres", "localhost", 10023);
				#endif
			#endif
		}

		if (cmdLine.HasSwitch("--generate")){
			#ifndef _TESTDATA_
				string rootFolder  = "Data\\Training\\Ultimate";
			#else
				string rootFolder  = "Data\\Testing\\Ultimate";
			#endif

			vector <string> dirVect;
			vector<const img2Parse> imgVect;

			#ifdef _DEBUG
				imgVect.push_back(img2Parse("Data/rod.ppm", "Rod"));
			#else
				// ================== Listing all the subdirectories
				path p (rootFolder);
				directory_iterator end_itr;

				for (directory_iterator itr(p); itr != end_itr; ++itr)
				{
					if (is_directory(itr->path()))
						dirVect.push_back(itr->path().string());
				}

				// ================= Iterate over all subdirectories.

				for (vector <string>::iterator it = dirVect.begin(); it != dirVect.end(); it++){
					// ==============   Getting className
					string className = *it;
					string str2remove = rootFolder + "\\";

					string::size_type i = className.find(str2remove);

					if (i != std::string::npos)
					   className.erase(i, str2remove.length());

					// ==============   cycle all files in the class : className
					path p (*it);
					directory_iterator end_itr;

					for (directory_iterator itr(p); itr != end_itr; ++itr)
					{
						if (is_regular_file(itr->path()))
							imgVect.push_back(img2Parse(itr->path().string(), className));
					}
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
	system("PAUSE");
	return EXIT_SUCCESS;
}