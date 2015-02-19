/*
	This class will implement the method necessary for 
	simplifying the skeleton structure

	author: Matthijs von Eede
*/

#ifndef _AFMMSKELETON_H_
#define _AFMMSKELETON_H_

#include <list>

#include "FluxSkeleton/include/DiscreteDivergenceSkeleton.h"
#include "FluxSkeleton/include/DivergenceSkeletonMaker.h"
#include "SkelCompParams.h"
#include "ShapeDiff.h"
#include "dagmatcher/SharedPtr.h"
#include "dagmatcher/BasicUtils.h"

//typedef sg::DiscreteDivergenceSkeletonEdge sgDDSEdge;
//typedef std::pair< sg::DiscreteDivergenceSkeletonEdge*, double> DDSEdgeAndError;


struct DDSEdgeAndError : public std::pair<sg::DiscreteDivergenceSkeletonEdge*, double>
{
	bool operator < (const DDSEdgeAndError& rhs) const
	{
		return (second < rhs.second);
	}
};

//first argument will hold the index of the edge in 'edges' the
//second argument will hold the number of points in that edge
//which will be used as a measure for the length of an edge
struct IndexAndLength : public std::pair<int,int>
{
	bool operator < (const IndexAndLength& rhs) const
	{
		return (second < rhs.second);
	}
};

typedef std::list<IndexAndLength> InternalEdgeList;
typedef FIELD<float> DFIELD;
typedef std::list<sg::DDSEdge*> DDSEdgeList;
typedef std::pair<DDSEdgeList, double> DDSkelAndError;
typedef std::list<DDSEdgeAndError> DDSEdgeAndErrorList;
typedef std::list<DDSkelAndError> DDSkelAndErrorList;
typedef std::list<sg::DDSNode*> DDSNodeList;
typedef sg::DivergenceMap DMap;
typedef SharedPtr<DDSNodeList> DDSNodeListPtr;
typedef SharedPtr<DDSEdgeList> DDSEdgeListPtr;
typedef SharedPtr<DDSEdgeAndErrorList> DDSEdgeAndErrorListPtr;
typedef SharedPtr<DDSkelAndErrorList> DDSkelAndErrorListPtr;

class Skeleton;


/*!
	The AFMMSkeleton class has the same functionality than
	the DiscreteDivergenceSkeleton class but it is created from
	AFMM Star skeletons instead of FluxSkeletons
*/
class AFMMSkeleton : public sg::DiscreteDivergenceSkeleton
{
private:
	//! member variables for the weights of the simplification
	double m_xmin, m_xmax, m_ymin, m_ymax;
	
	//! variable which will hold the reconstruction error of a skeleton at a given moment in time
	double m_dRecError;
	
	double m_dRecErrorWeightBnd, m_dRecErrorWeightStr;
	
	DFIELD* m_pOriginalDTMap; //!< stores the original distance transform map
	
	DFIELD* m_pCurrentSkeletonField; //!< stores the current skeleton field
	
	FLAGS* m_pOriginalAFMMFlags; //!< stores the raw output of the AFMM algorithm
	
	Skeleton* m_pAlexSkeleton; //!< stores an object of the old skeleton code
	
	ShapeDiff* m_pSD; //!< computes diff between orig and reconstructed skeleton
		
	bool ExternalEdgesSimplified(); //!< tries to simplify external branches
	
	bool InternalEdgesSimplified(); //!< tries to simplify internal branches
	
	DDSEdgeListPtr GetExternalEdges();
	
	void CreateFieldWithoutExternalEdge
		(sg::DDSEdge* pExternalEdge, 
		DFIELD* pFieldWithoutEdge);
		
	void MarkSkeletonWithoutEdge
		(sg::DDSEdge* pExternalEdge,
		DFIELD* pFieldWithoutEdge);
		
	unsigned int GetEdgeCount()
	{
		return getEdges().size();
	}
		
	void Init();
	
	void InitializeSkeletonField();
	
	void CreateShapeDiff();
	
	void RemoveExternalEdgeFromField(const sg::DDSEdge* pExternalEdge);
	
	void AddExternalEdgeToField(const sg::DDSEdge* pExternalEdge);
	
	int DetermineEdgesToBeRemovedCount(DDSEdgeAndErrorListPtr ptrEdgeErrorValues);
	
	void RemoveEdge(sg::DDSEdge* pExternalEdge);
	
	void RemoveNode(sg::DDSNode* pNode);
	
	void RemoveReferenceToEdge(sg::DDSEdge* pExternalEdge);
	
	int RemoveReferenceFromNode(sg::DDSNode* pNode, sg::DDSEdge* pExternalEdge);
	
	void UpdateSkeleton();
	
	void EnsureFlowAway(sg::DDSEdge* pEdge, sg::DDSNode* pFlowAwayNode);
	
	void EnsureMergedEdges(sg::DDSEdge* pEdge);
	
	void MergeEdge(sg::DDSEdge* pEdge);
	
	void FindLigatureEdges(InternalEdgeList& ligatureEdges);
	
	bool IsInternalEdge(sg::DDSEdge* pEdge);
	
	bool IsExternalEdge(sg::DDSEdge* pEdge);
	
	bool IsLigature(sg::DDSEdge* pEdge);
	
	bool IsOuterInsideEdge(sg::DDSEdge* pEdge);
	
public:
	AFMMSkeleton();
	AFMMSkeleton(DMap* dm); //constructor

	virtual void Create(DMap* dm);
	
	static AFMMSkeleton* MakeSkeleton(const SkelCompParams& skelParams);

	virtual ~AFMMSkeleton(); //destructor

	void SimplifyExternal(); //simplify the external branches
	
	void SimplifyInternal(); //simplify the internal branches

	void Simplify(); //simplify both external and internal branches
	
	void SetRecErrorWeightBnd(double dRecErrorWeightBnd)
	{
		m_dRecErrorWeightBnd = dRecErrorWeightBnd;
	}
	
	void SetRecErrorWeightStr(double dRecErrorWeightStr)
	{
		m_dRecErrorWeightStr = dRecErrorWeightStr;
	}
	
	void SetDistanceTransformMap(DFIELD* pDTMap)
	{
		m_pOriginalDTMap = pDTMap;
	}
	
	void SetOriginalAFMMFlags(FLAGS* pFLAGS)
	{
		m_pOriginalAFMMFlags = pFLAGS;
	}
	
	void SetDimensions(double xmin, double xmax, double ymin, double ymax)
	{
		m_xmin = xmin;
		m_xmax = xmax;
		m_ymin = ymin;
		m_ymax = ymax;
	}
	
	const void GetDimensions(double* xmin, double* xmax, double* ymin, double* ymax)
	{
		*xmin = m_xmin;
		*xmax = m_xmax;
		*ymin = m_ymin;
		*ymax = m_ymax;
	}
};

class AFMMDivergenceSkeletonMaker : public sg::DivergenceSkeletonMaker
{
public:
	/*AFMMDivergenceSkeletonMaker(const sg::DivergenceMap& dm) : 
	  sg::DivergenceSkeletonMaker(dm) { }

	AFMMDivergenceSkeletonMaker(const sg::DivergenceMap* dm) : 
	  sg::DivergenceSkeletonMaker(dm) { }*/

	AFMMSkeleton* getAFMMDDSkeleton(sg::DivArr& da, sg::DivergenceMap* dm)
	{
		colour_skeleton_array(da);
		
		AFMMSkeleton* skeleton = new AFMMSkeleton(dm);
		
		return dynamic_cast<AFMMSkeleton*>(sg::create_skeleton(da, dm, skeleton));
	}
};

#endif //_AFMMSKELETON_H_
