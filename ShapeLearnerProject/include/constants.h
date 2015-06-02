/* ************* Begin file constants.h ***************************************/
/*
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
*	\file constants.h
*	\brief The constants are used by the whole library : Functions' arguments, Attributes' size, ...
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef _MSC_VER
	#pragma message("Compiling GraphDBLib::constants.h  - this should happen just once per project.\n")
#endif
/*!
*	\namespace constants
*	\brief Namespace containing the constant variables used by the whole library
*/
namespace constants{
	/*!
	*	Maximum size of a string in the database.
	*/
	const unsigned int		SIZE_MAX_STRING = 255;

	/*!
	*	Retries before declaring a transaction failed.
	*/
	const unsigned int		MAX_DB_RETRY = 4;

	/*!
	*	Constant referencing to the Application's Database log.
	*/
	const unsigned int		LogDB = 1;

	/*!
	*	Constant referencing to the Application's Core log.
	*/
	const unsigned int		LogCore = 2;

	/*!
	*	Constant referencing to the Application's Execution log.
	*/
	const unsigned int		LogExec = 3;

	/*!
	*	Constant referencing to the Application's Erros log.
	*/
	const unsigned int		LogError = 4;

	/*!
	*	Constant referencing to the number of threads used by the GraphDB Library.
	*/
	#ifdef _DEBUG
		const int		nbMaxThread = 2;
	#else
		const int		nbMaxThread = 30;
	#endif
}

#endif // CONSTANTS_H