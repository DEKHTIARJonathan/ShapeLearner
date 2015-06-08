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

//#define _LOW_
//#define _MEDIUM_
//#define _HIGH_
//#define _EXTREME_
#define _ULTIMATE_

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
			#ifndef _TESTDATA_
				#ifdef _LOW_
					GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10100, "sources/structure.sql");
				#else
					#ifdef _MEDIUM_
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10101, "sources/structure.sql");
					#else
						#ifdef _HIGH_
							GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10102, "sources/structure.sql");
						#else
							#ifdef _EXTREME_
								GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10103, "sources/structure.sql");
							#else
								#ifdef _ULTIMATE_
									GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10104, "sources/structure.sql");
								#endif
							#endif
						#endif
					#endif
				#endif
			#else
				GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10200, "sources/structure.sql");
			#endif

			dml::DAGMatcherLib::InitDAGMatcherLib();
		}
		else
		{
			#ifndef _TESTDATA_
				#ifdef _LOW_
					GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10100);
				#else
					#ifdef _MEDIUM_
						GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10101);
					#else
						#ifdef _HIGH_
							GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10102);
						#else
							#ifdef _EXTREME_
								GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10103);
							#else
								#ifdef _ULTIMATE_
									GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10104);
								#endif
							#endif
						#endif
					#endif
				#endif
			#else
				GraphDB::openDatabase("postgres", "postgres", "postgres", "54.77.188.25", 10200);
			#endif
		}

		if (cmdLine.HasSwitch("--generate")){
			#ifndef _TESTDATA_
				#ifdef _LOW_
					string rootFolder  = ".\\Data\\Training\\Low";
				#else
					#ifdef _MEDIUM_
						string rootFolder  = ".\\Data\\Training\\Medium";
					#else
						#ifdef _HIGH_
							string rootFolder  = ".\\Data\\Training\\High";
						#else
							#ifdef _EXTREME_
								string rootFolder  = ".\\Data\\Training\\Extreme";
							#else
								#ifdef _ULTIMATE_
									string rootFolder  = ".\\Data\\Training\\Ultimate";
								#endif
							#endif
						#endif
					#endif
				#endif
			#else
				#ifdef _LOW_
					string rootFolder  = ".\\Data\\Testing\\Low";
				#else
					#ifdef _MEDIUM_
						string rootFolder  = ".\\Data\\Testing\\Medium";
					#else
						#ifdef _HIGH_
							string rootFolder  = ".\\Data\\Testing\\High";
						#else
							#ifdef _EXTREME_
								string rootFolder  = ".\\Data\\Testing\\Extreme";
							#else
								#ifdef _ULTIMATE_
									string rootFolder  = ".\\Data\\Testing\\Ultimate";
								#endif
							#endif
						#endif
					#endif
				#endif
			#endif

			vector <string> dirVect;
			vector<const img2Parse> imgVect;

			/*
			#ifdef _DEBUG
				imgVect.push_back(img2Parse("Data/Training/Ultimate/Transmission/TransmissionChaine00.ppm", "Amortisseur"));
				imgVect.push_back(img2Parse("Data/Training/Ultimate/Transmission/TransmissionChaine10.ppm", "Amortisseur"));
			#else
			*/
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
			//#endif

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