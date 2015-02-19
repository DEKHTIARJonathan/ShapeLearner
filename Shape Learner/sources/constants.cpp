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
 * \file constants.cpp
 * \brief Ces constantes sont essentiellement des restrictictions de taille de QString n�cessaire � l'utilisation de SQLite
 * \version	1.0
 * \author Jonathan Dekhtiar
 */

#include "allHeaders.h"

/******************************************************************************************************************************
 *******************************************************************************************************************************
 ********************************************************************************************************************************
 *********************************************************************************************************************************
 **********************************************************************************************************************************
 *******                                                                                                                ************
 *******       ATTENTION :Taille d'une String SQLite ne peut pas �tre plus grand que 255 => sizeMax(Varchar) = 255      *************
 *******                                                                                                                ************
 **********************************************************************************************************************************
 *********************************************************************************************************************************
 ********************************************************************************************************************************
 *******************************************************************************************************************************
 ******************************************************************************************************************************/


const unsigned int		constants::SIZE_MAX_TYPE_DAG = 30;
const unsigned int		constants::SIZE_MAX_CLASS_NAME = 255;
const unsigned int		constants::SIZE_MAX_GRAPH_NAME = 255;
const unsigned int		constants::SIZE_MAX_PATH = 255;