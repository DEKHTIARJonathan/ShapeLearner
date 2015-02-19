/**************************************************************************

   File:                DiscreteDivergenceSkeleton.h

   Author(s):           Pavel Dimitrov

   Created:             22 Jul 2002

   Last Revision:       $Date: 2002/07/25 20:50:47 $

   Description: 

   $Revision: 1.2 $

   $Log: DiscreteDivergenceSkeleton.h,v $
   Revision 1.2  2002/07/25 20:50:47  pdimit
   Making release 0.1

   Revision 1.1  2002/07/23 21:02:51  pdimit
   The branch segmentation of the thinned DivMap is improved --
   the two pixel branches are now included. Still, the degenerate case
   of the square of junction-points is not taken care of.

   Also, a DiscreteDivergenceSkeleton is created which still does not know
   of left and right for the branches, i.e. the contour has not been cut.

   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef DISCRETE_DIVERGENCE_SKELETON_NODE_H
#define DISCRETE_DIVERGENCE_SKELETON_NODE_H

#include "FluxPoint.h"
#include "defines.h"

namespace sg {
	
class DiscreteDivergenceSkeleton;
class DiscreteDivergenceSkeletonEdge;

// Create synonyms of DiscreteDivergenceSkeleton that are simpler or
// that carry more meaning in different contexts
typedef DiscreteDivergenceSkeleton DDSkeleton;
typedef DiscreteDivergenceSkeleton DDSGraph;

typedef std::vector<DiscreteDivergenceSkeletonEdge*> DDSEdgeVect;

/*!
	Node of a DiscreteDivergenceSkeleton graph 
*/
class DiscreteDivergenceSkeletonNode 
{
public: // TODO: should be protected but AFMMSkeleton.h must be corrected first
	DDSkeleton *dds;

public:
	FluxPoint fp;
	std::vector<DiscreteDivergenceSkeletonEdge*> edges;

	void* pNodeData;  //<! Used by the GeneralizedSkeletalGraph class

public:
	DiscreteDivergenceSkeletonNode(FluxPoint p)
	{
		fp = p;
	}

	void addEdge(DiscreteDivergenceSkeletonEdge* e)
	{
		edges.push_back(e);
	}

	DDSEdgeVect& getEdges() 
	{ 
		return edges; 
	}

	void setEdge(unsigned int i, DiscreteDivergenceSkeletonEdge* e) 
	{ 
		edges[i] = e; 
	}
	
	const DiscreteDivergenceSkeletonEdge* getEdge(unsigned int i) const
	{
		return edges[i];
	}

	DiscreteDivergenceSkeletonEdge* getEdge(unsigned int i)
	{
		return edges[i];
	}

	void setSkeleton(DDSkeleton* sk) { dds = sk; }
	DDSkeleton* getSkeleton()        { return dds; }

	double getFlux() { return fp.val;  }
	double getDist() { return fp.dist; }
	char   getCol()  { return fp.col;  }
	Point  getPoint(){ return fp.p;    }

	unsigned int degree() const { return edges.size(); }
};

} //namespace sg

#endif // DISCRETE_DIVERGENCE_SKELETON_NODE_H
