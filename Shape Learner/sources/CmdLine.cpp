/*------------------------------------------------------
   CCmdLine

   A utility for parsing command lines.

   Copyright (C) 1999 Chris Losinger, Smaller Animals Software.
   http://www.smalleranimals.com

   This software is provided 'as-is', without any express
   or implied warranty.  In no event will the authors be
   held liable for any damages arising from the use of this software.

   Permission is granted to anyone to use this software
   for any purpose, including commercial applications, and
   to alter it and redistribute it freely, subject to the
   following restrictions:

	 1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but is not required.

	 2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

	 3. This notice may not be removed or altered from any source
   distribution.

   See SACmds.h for more info.
------------------------------------------------------*/

/**
*	\file CmdLine.cpp
*	\brief Classe de gestion des argument en ligne de commande.
*	\version 1.1
*	\author Chris Losinger, Smaller Animals Software.
*	\author Modified by Jonathan DEKHTIAR
*/

#include "allHeaders.h"
//#include "CmdLine.h"

using namespace std;

int CCmdLine::SplitLine(int argc, char **argv) throw(CCmdLineException)
{
	clear();
	pair<CCmdLine::iterator, bool> res;

	#ifndef _DEBUG
	if (argc == 1)
		throw CCmdLineException("CCmdLine::SplitLine", "Error : No argument has been given by command line.");
	#endif

	// skip the exe name (start with i = 1)
	for (int i = 1; i < argc; i++)
	{
		// if it's a switch, start a new CCmdLine
		if (IsSwitch(argv[i]))
		{
			CCmdParam cmd;

			// add the CCmdParam to 'this'
			res = insert(CCmdLine::value_type( argv[i], cmd));

			if (!res.second)
				throw CCmdLineException("CCmdLine::SplitLine", QString("%1; %2").arg("Error while parsing the switch : ").arg(argv[i]));
		}
		else
		// it's not a switch, it's a parameter for the last switch => let's add it
		res.first->second.m_strings.push_back(argv[i]);
	}

	return size();
}

bool CCmdLine::IsSwitch(const char *pParam) throw(CCmdLineException)
{
   if (pParam==NULL)
	  throw CCmdLineException("CCmdLine::IsSwitch", "Error : The parameter pParam given is NULL.");

   // switches must non-empty
   // must have at least one character after the '-'
   if (strlen(pParam) <= 1)
	  throw CCmdLineException("CCmdLine::IsSwitch", QString("%1; %2, %3").arg("Error : The parameter pParam (").arg(pParam).arg(") must have at least one character after the '-'."));

   // switches always start with '-'
   if (pParam[0]=='-')
   {
	  // allow negative numbers as arguments.
	  // ie., don't count them as switches
	  return (!isdigit(pParam[1]));
   }
   else
   {
	  return false;
   }
}

bool CCmdLine::HasSwitch(const char *pSwitch) throw(CCmdLineException)
{
	if (pSwitch==NULL)
	  throw CCmdLineException("CCmdLine::HasSwitch", "Error : The parameter pSwitch given is NULL.");

   // switches must non-empty
   // must have at least one character after the '-'
   if (strlen(pSwitch) <= 1)
	  throw CCmdLineException("CCmdLine::HasSwitch", QString("%1; %2, %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") must have at least one character after the '-'"));
	
	CCmdLine::iterator theIterator;
	theIterator = find(pSwitch);
	return (theIterator!=end());
}

string CCmdLine::GetSafeArgument(const char *pSwitch,unsigned int iIdx, const char *pDefault) throw(CCmdLineException)
{
	if (pSwitch==NULL)
	  throw CCmdLineException("CCmdLine::GetSafeArgument", "Error : The parameter pSwitch given is NULL.");

   // switches must non-empty
   // must have at least one character after the '-'
   if (strlen(pSwitch) <= 1)
	  throw CCmdLineException("CCmdLine::GetSafeArgument", QString("%1 %2 %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") must have at least one character after the '-'"));

	string sRet;

   if (pDefault!=NULL)
	  sRet = pDefault;
   else
	   throw CCmdLineException("CCmdLine::GetSafeArgument", "Error : The parameter pDefault = NULL.");

   try
   {
	  sRet = GetArgument(pSwitch, iIdx);
   }
   catch ( const std::exception& )
   {
	   // Do nothing here. Ignore the exception.
   }

   return sRet;
}

string CCmdLine::GetArgument(const char *pSwitch, unsigned int iIdx) throw(CCmdLineException)
{
	
	if (pSwitch==NULL)
	  throw CCmdLineException("CCmdLine::GetSafeArgument", "Error : The parameter pSwitch given is NULL.");

   // switches must non-empty
   // must have at least one character after the '-'
   if (strlen(pSwitch) <= 1)
	  throw CCmdLineException("CCmdLine::GetSafeArgument", QString("%1 %2 %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") must have at least one character after the '-'"));
	
	CCmdLine::iterator theIterator;

	theIterator = find(pSwitch);
	if (theIterator!=end())
	{
		if ((*theIterator).second.m_strings.size() > iIdx)
			return (*theIterator).second.m_strings[iIdx];
		else
			throw CCmdLineException("CCmdLine::GetArgument", QString("%1, %2, %3, %4").arg("Error : The argument number ").arg(iIdx).arg(" doesn't exist for the pSwitch given : ").arg(pSwitch));
	}
	else
		throw CCmdLineException("CCmdLine::GetArgument", QString("%1 %2 %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") does not exist."));
	
}

int CCmdLine::GetArgumentCount(const char *pSwitch) throw(CCmdLineException)
{
	if (pSwitch==NULL)
	  throw CCmdLineException("CCmdLine::GetArgumentCount", "Error : The parameter pSwitch given is NULL.");

	// switches must non-empty
	// must have at least one character after the '-'
	if (strlen(pSwitch) <= 1)
		throw CCmdLineException("CCmdLine::GetArgumentCount", QString("%1 %2 %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") must have at least one character after the '-'"));

	int iArgumentCount = -1;

	CCmdLine::iterator theIterator;

	theIterator = find(pSwitch);
	
	if (theIterator==end())
		throw CCmdLineException("CCmdLine::GetArgumentCount", QString("%1 %2 %3").arg("Error : The parameter pSwitch (").arg(pSwitch).arg(") does not exist."));
		
	iArgumentCount = (*theIterator).second.m_strings.size();
	return iArgumentCount;
}

void CCmdLine::ShowHelp(const char* filepath)
{
	string line;

	ifstream myfile(filepath, std::ifstream::in);

	if (!myfile)
		throw CCmdLineException("CCmdLine::ShowHelp", QString("%1 %2 %3").arg("Error : The Help File ( ").arg(filepath).arg(") doesn't exist."));

	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			cout << line << '\n';
		}
		myfile.close();
	}
	else
		throw CCmdLineException("CCmdLine::ShowHelp", QString("%1 %2").arg("Error : Unable to open the file : ").arg(filepath));
}