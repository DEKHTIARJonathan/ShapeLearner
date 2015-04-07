/* ************* Begin file shockGraphsGenerator.h ***************************************/
/*
** 2015 April 03
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file shockGraphsGenerator.h
*	\brief ShockGraphGenerator Header
*	\version 1.0
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "allHeaders.h"
using namespace std;

_Longlong fact(_Longlong nbr){
	_Longlong tmp = nbr;
	_Longlong rslt = 1;
	while(tmp > 0)
	{
		rslt *= tmp--;
	}

	return rslt;
}

bool shockGraphsGenerator::taskExecute()
{
	Logger::Log("Debut de la tache numero " + to_string((_Longlong)taskNum));
	for (int i = 1000000000; i <= 10000000000; i++)
		fact(i);
	Logger::Log("Fin de la tache numero " + to_string((_Longlong)taskNum));
	return true;
}