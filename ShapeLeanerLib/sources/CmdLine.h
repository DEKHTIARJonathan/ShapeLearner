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

------------------------------------------------------*/

/**
*	\file CmdLine.h
*	\brief Classe de gestion des argument en ligne de commande.
*	\version 1.1
*	\author Chris Losinger, Smaller Animals Software.
*	\author Modified by Jonathan DEKHTIAR
*/

#ifndef _CMD_LINE_PARSER_
#define _CMD_LINE_PARSER_

#include "allHeaders.h"

/*

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "CmdLineException.h"

#pragma warning( disable : 4290 )

*/

using namespace std;

class CCmdLineException; //Forward Declaration of the class contained in CmdLineException.h

// handy little container for our argument vector
struct CCmdParam
{
   vector<string> m_strings;
};

// this class is actually a map of strings to vectors
typedef map<string, CCmdParam> _CCmdLine;

// the command line parser class
class CCmdLine : public _CCmdLine
{

public:
	/*!
	*	\brief Parse the command line into switches and arguments.
	*	\param argc : Given by the main().
	*	\param argv : Given by the main().
	*	\return Returns number of switches found.
	*/
	int SplitLine(int argc, char **argv) throw(CCmdLineException);

	/*!
	*	\brief Was the switch found on the command line ?
	*	\param pSwitch : The Switch we are looking for.
	*	\return True // False => Found // Not Found.
	*	\example 
		if the command line is : app.exe -a p1 p2 p3 -b p4 -c -d p5

		call                          return
		----                          ------
		cmdLine.HasSwitch("-a")       true
		cmdLine.HasSwitch("-z")       false
	*/
	bool HasSwitch(const char *pSwitch) throw(CCmdLineException);

	/*!
	*	\brief Fetch an argument associated with a switch . if the parameter at index iIdx is not found, this will return the default that you provide.
	*	\param pSwitch : The Switch we are looking for.
	*	\param iIdx : The index of the parameter for a given switch
	*	\param pDefault : The default value returned if the parameter at for the given switch at the given index is not found.
	*	\return Return the parameter found or the default value given in 3rd argument if not found.
	*	\example 
		command line is : app.exe -a p1 p2 p3 -b p4 -c -d p5

		call                                      return
		----                                      ------
		cmdLine.GetSafeArgument("-a", 0, "zz")    p1
		cmdLine.GetSafeArgument("-a", 1, "zz")    p2
		cmdLine.GetSafeArgument("-b", 0, "zz")    p4
		cmdLine.GetSafeArgument("-b", 1, "zz")    zz
	*/
	string  GetSafeArgument(const char *pSwitch, unsigned int iIdx, const char *pDefault) throw(CCmdLineException);

	/*!
	*	\brief Fetch an argument associated with a switch. throws an exception of (int)0, if the parameter at index iIdx is not found.
	*	\param pSwitch : The Switch we are looking for.
	*	\param iIdx : The index of the parameter for a given switch
	*	\return Return the parameter found or the default value given in 3rd argument if not found.
	*	\example 
		command line is : app.exe -a p1 p2 p3 -b p4 -c -d p5

		call                             return
		----                             ------
		cmdLine.GetArgument("-a", 0)     p1
		cmdLine.GetArgument("-b", 1)     throws (int)0, returns an empty string
	*/
	string GetArgument(const char *pSwitch, unsigned int iIdx) throw(CCmdLineException); 

	/*!
	*	\brief Count the number of arguments available for a given switch.
	*	\param pSwitch : The Switch we are looking for.
	*	\return Returns the number of arguments found for a given switch // Returns -1 if the switch was not found.
	*/
	int GetArgumentCount(const char *pSwitch) throw(CCmdLineException);

	/*!
	*	\brief Print Help on screen, Throw an exception if the file is not found.
	*	\param filepath : Path of the help file.
	*/
	void ShowHelp(const char* filepath = "help.txt") throw(CCmdLineException);

protected:

	/*!
	*	\brief Test a parameter to see if it's a switch.
	*	\param pParam : The parameter we are testing.
	*	\return Returns True // False.
	*	\details Switches are of the form : -x, where 'x' is one or more characters. The first character of a switch must be non-numeric!
	*/
   bool IsSwitch(const char *pParam);
};

#endif // _CMD_LINE_PARSER_