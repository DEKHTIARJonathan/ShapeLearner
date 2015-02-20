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
*	\brief Ces constantes sont essentiellement des restrictictions de taille de QString nécessaire à l'utilisation de SQLite
*	\version 1.0
*	\author Jonathan Dekhtiar
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants{

	// Constantes sur les données des Notes

	extern const unsigned int		SIZE_MAX_TYPE_DAG;
	extern const unsigned int		SIZE_MAX_CLASS_NAME;
	extern const unsigned int		SIZE_MAX_GRAPH_NAME;
	extern const unsigned int		SIZE_MAX_OBJ_NAME;
	extern const unsigned int		SIZE_MAX_PATH;

}

#endif // CONSTANTS_H