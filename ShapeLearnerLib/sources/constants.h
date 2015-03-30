/* ************* Begin file dbexception.cpp ***************************************/
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
*	\brief Ces constantes sont essentiellement des restrictictions de taille de string nécessaire à l'utilisation de SQLite
*	\version 1.0
*	\author Jonathan Dekhtiar
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H


/*!	
*	\namespace constants
*	\brief Namespace used to save constants values used by the whole the library.
*/
namespace constants{

	const unsigned int		SIZE_MAX_STRING = 255;
	const unsigned int		MAX_DB_RETRY = 4;
	const unsigned int		LogDB = 1;
	const unsigned int		LogCore = 2;
	const unsigned int		LogExec = 3;
	const unsigned int		LogError = 4;

}

#endif // CONSTANTS_H