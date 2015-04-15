/* ************* Begin file infoStructures.h ***************************************/
/*
** 2015 April 13
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file infoStructures.h
*	\brief Structure header file.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#ifndef _INFO_STRUCTURES_H_
#define _INFO_STRUCTURES_H_

#include "stdafx.h"

struct ShapeMatchingParams
{
	double idxtau, matchtau, idxrange, modelSimWeight;
	int idxKBest, asyncCompu, showResults, saveResults, saveNodeSim;
	int computeStats; // 0 => no stats, 1 is UofT format, 2 is PBS format
	int selectionMethod;
	int firstModelId, lastModelId;
	const char* statsOutDir;
};

struct ShapeRepresentationParams
{
	enum RepType {ShockGraphRep = 1, GestureGraphRep, BoneGraphRep};

	dml::SkeletalGraphParams skelparams;
	dml::ShockGraphParams sgparams;
	dml::BoneGraphParams bgparams;

	int shapeRepType;
	int timeout;
	int nBumpSize, nNotchSize;
	const char* outDir;
};

struct TrainingParams
{
	int queryID;
	int modelID;
	double similarity;
	double param0;
};

typedef std::list<TrainingParams> TrainingParamList;

#define SGShapeRep ShapeRepresentationParams::ShockGraphRep
#define GGShapeRep ShapeRepresentationParams::GestureGraphRep
#define BGShapeRep ShapeRepresentationParams::BoneGraphRep

#endif // _INFO_STRUCTURES_H_