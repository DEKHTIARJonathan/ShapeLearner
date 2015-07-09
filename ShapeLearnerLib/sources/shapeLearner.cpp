/* ************* Begin file ShapeLearner.cpp ***************************************/
/*
** 2015 February 23
**
** In place of a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************/

/**
*	\file ShapeLearner.cpp
*	\brief ShapeLearner source file. Public Interface and Central point in the software. It realizes the task distribution to the different instances of the software and make them communicate without knowing of each other.
*	\version 1.1
*	\author Jonathan DEKHTIAR - contact@jonathandekhtiar.eu - @born2data - http://www.jonathandekhtiar.eu
*/

#include "stdafx.h"
#include "allHeaders.h"

using namespace std;

/* *******************************************************************
*                             Multi Threading                        *
 ********************************************************************/

boost::threadpool::pool ShapeLearner::Pool (constants::nbMaxThread);

/*!
*	\Brief Local Function used to get the ThreadID.
*/

void ShapeLearner::createShockGraph (const vector<const img2Parse> &imgVect) throw(StandardExcept){
   //Random Init
   std::srand(std::time(0));

   // Create fifo thread pool container with two threads.
   for (vector<const img2Parse>::const_iterator it = imgVect.begin(); it != imgVect.end(); it++){
      Pool.schedule(boost::bind(&ShapeLearner::createShockGraphWorker, *it));
      JobManager::Log(it->jobID,Waiting,0,WaitingGen,it->filepath);
   }
   //  Wait until all tasks are finished
   Pool.wait();
}

unsigned int ShapeLearner::getActiveThread () throw(StandardExcept){
   return Pool.active();
}

void ShapeLearner::createShockGraph (const img2Parse &img) throw(StandardExcept){
   //Random Init
   std::srand(std::time(0));
   JobManager::Log(img.jobID,Waiting,0,WaitingGen, img.filepath);
   Pool.schedule(boost::bind(&ShapeLearner::createShockGraphWorker, img));
}


void ShapeLearner::loadShockGraph (/*const img2Parse &img*/) throw(StandardExcept){
   //Random Init
   std::srand(std::time(0));
   JobManager::Log(img.jobID,Waiting,0,WaitingGen, img.filepath);
   Pool.schedule(boost::bind(&ShapeLearner::loadShockGraphWorker, img));
}



void ShapeLearner::waitForComputation () throw(StandardExcept){
   //  Wait until all tasks are finished
   Pool.wait();
}

bool ShapeLearner::createShockGraphWorker (const img2Parse& imgInfo) throw(StandardExcept){
   shockGraphsGenerator worker(imgInfo.filepath, imgInfo.objClass, imgInfo.jobID);
   worker.taskExecute();
   graphDBLib::GraphDB::closeThreadConnection();
   return true;
}

bool ShapeLearner::loadShockGraphWorker (/*const img2Parse& imgInfo*/) throw(StandardExcept){
   shockGraphsGenerator worker(imgInfo.filepath, imgInfo.objClass, imgInfo.jobID);
   worker.taskExecute();
   graphDBLib::GraphDB::closeThreadConnection();
   return true;
}




   img2Parse::
img2Parse( const string _filepath, 
           const string _objClass, 
           const unsigned int _jobID )
   : filepath(_filepath), 
     objClass(_objClass), 
     jobID(_jobID) 
{
}

