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

 
#include "stdafx.h"
#include "allHeaders.h"

using namespace std;
using namespace dml;

/*
_Longlong fact(_Longlong nbr){
	_Longlong tmp = nbr;
	_Longlong rslt = 1;
	while(tmp > 0)
	{
		rslt *= tmp--;
	}

	return rslt;
}
*/

string genFileName(const unsigned int len = 10) 
{
    const string alphanum = "0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	string rslt;

    for (unsigned int i = 0; i < len; ++i)
		rslt += alphanum[rand() % (int)(alphanum.size() -1)];

    return rslt+".ppm";
}


 shockGraphsGenerator::shockGraphsGenerator(const string& _imgPath, const unsigned long _taskNum) : imgPath(_imgPath), taskNum(_taskNum) {
	m_matchInfo.asyncCompu = 0;
	m_matchInfo.computeStats = 0;
	m_matchInfo.firstModelId = -1;
	m_matchInfo.idxKBest = 101;
	m_matchInfo.idxrange = 0.40000000000000002;
	m_matchInfo.idxtau = 0.29999999999999999;
	m_matchInfo.lastModelId = -1;
	m_matchInfo.matchtau = 0;
	m_matchInfo.modelSimWeight = 0.50000000000000000;
	m_matchInfo.saveNodeSim = 0;
	m_matchInfo.saveResults = 0;
	m_matchInfo.selectionMethod = 0;
	m_matchInfo.statsOutDir = ".";
	m_matchInfo.showResults = 1;

	m_shapeInfo.nBumpSize = 0;
	m_shapeInfo.nNotchSize = 0;
	m_shapeInfo.outDir = "./NoisyImgs/";
	m_shapeInfo.shapeRepType = 1;
	m_shapeInfo.timeout = 800;
	
	//m_shapeInfo.skelparams
	//m_shapeInfo.sgparams
	//m_shapeInfo.bgparams
}


bool shockGraphsGenerator::taskExecute()
{
	Logger::Log("Adding object ("+imgPath+")to database...", constants::LogCore);

	//ProcessFile(imgPath, m_shapeInfo, m_matchInfo.asyncCompu != 0);

	Logger::Log("Object ("+imgPath+") has been added to database...", constants::LogCore);

	return true;
}


void shockGraphsGenerator::processFile(bool bAsyncProcessing)
{
	DAGPtr pDag;
	bool bIsRead;
	const char* szFileExt;

	szFileExt = DirWalker::FindFileExtension(imgPath.c_str());

	if (!strcmp(szFileExt, "ppm") || !strcmp(szFileExt, "pgm") ||
		!strcmp(szFileExt, "bmp") || !strcmp(szFileExt, "tif") ||
		!strcmp(szFileExt, "jpg") || !strcmp(szFileExt, "png"))
	{
		ImageInfo imgInfo(imgPath.c_str());

		// Add artificial noise if requested
		if (m_shapeInfo.nBumpSize > 0 || m_shapeInfo.nNotchSize > 0)
		{
			ShowMsg("Adding noise to the image...");
			AddBumpsAndNotches(&imgInfo);
		}

		// Choose the appropriate shape representation
		if (m_shapeInfo.shapeRepType == SGShapeRep)
		{
			ShockGraph* pSG = new ShockGraph;
			pDag = pSG;

			bIsRead = pSG->Create(imgInfo, m_shapeInfo.sgparams, m_shapeInfo.skelparams);
		}
		else if (m_shapeInfo.shapeRepType == BGShapeRep)
		{
			BoneGraph* pBG = new BoneGraph;
			pDag = pBG;

			bIsRead = pBG->Create(imgInfo, m_shapeInfo.bgparams, m_shapeInfo.skelparams);
		}
		else
		{
			bIsRead = false;
			ShowError("Only ShockGraph or BoneGraph are valid options for image files.");
		}
	}
	else if (!strcmp(szFileExt, "gg"))
	{
		GestureGraph* pGG = new GestureGraph;
		pDag = pGG;

		bIsRead = pGG->Read(imgPath.c_str());
	}
	else
	{
		cerr << "\nUnknown file extension: " << szFileExt << endl;
		bIsRead = false;
	}

	if(bIsRead)
	{
		Logger::Log("DONE! (" + to_string((_Longlong)pDag->GetNodeCount()) + " nodes)... ", constants::LogCore);

		//dagDB.AddDAG(pDag, true);

		Logger::Log("WRITTEN TO DB!", constants::LogCore);
	}
	else
		Logger::Log("ERROR: Can't read dag.", constants::LogCore);
}

bool shockGraphsGenerator::AddBumpsAndNotches(dml::ImageInfo* pImgInfo)
{
	return true;
}