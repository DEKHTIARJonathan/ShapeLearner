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

namespace constants{

	// Constantes sur les données des Notes

	const unsigned int		SIZE_MAX_TYPE_DAG = 30;
	const unsigned int		SIZE_MAX_CLASS_NAME = 255;
	const unsigned int		SIZE_MAX_GRAPH_NAME = 255;
	const unsigned int		SIZE_MAX_OBJ_NAME = 255;
	const unsigned int		SIZE_MAX_PATH = 255;
	const unsigned int		MAX_DB_RETRY = 4;
	const unsigned int		DB_PGSQL = 1;
	const unsigned int		DB_MYSQL = 2;
	const unsigned int		DB_ORACLE = 3;

}

#endif // CONSTANTS_H