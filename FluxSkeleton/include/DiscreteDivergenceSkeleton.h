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

#ifndef DISCRETE_DIVERGENCE_SKELETON_H
#define DISCRETE_DIVERGENCE_SKELETON_H

#include <stdlib.h>
#include "defines.h"
#include "DivergenceMap.h"
#include "DiscreteDivergenceSkeletonEdge.h"
#include "SimpleShape.h"

namespace sg {

// Create synonyms of the graphs m_nodes and m_edges
typedef DiscreteDivergenceSkeletonEdge DDSEdge;
typedef DiscreteDivergenceSkeletonNode DDSNode;
	
typedef std::vector<DDSNode*> DDSNodeVect;

/*!
	Graph of skeletal points in which the m_nodes represent
	the terminal point of skeletal branches, and the m_edges
	represent all the points in each branch (including
	the terminal points).
*/
class DiscreteDivergenceSkeleton
{
protected:
	DivergenceMap* m_pDivMap;
	Shape* m_pShape;
	DDSEdgeVect m_edges;
	DDSNodeVect m_nodes;

public:
	DiscreteDivergenceSkeleton(){
		m_pDivMap = NULL;
		m_pShape = NULL;
	}

	DiscreteDivergenceSkeleton(SimpleShape* pSimpleShape) 
	{
		//DistanceTransform* pDT = new DistanceTransform(pSimpleShape);

		//DT owns the shape now

		//Create(new DivergenceMap(pDT));

		m_pDivMap = NULL;
		m_pShape = pSimpleShape;
	}

	DiscreteDivergenceSkeleton(DivergenceMap* dm)
	{
		Create(dm);      
	}

	virtual void Create(DivergenceMap* dm) 
	{
		m_pDivMap = dm;
		
		m_pShape = dm->getShape();
	}

	void addNode(DiscreteDivergenceSkeletonNode *n)
	{
		m_nodes.push_back(n);
		n->setSkeleton(this);
	}

	void addEdge(DiscreteDivergenceSkeletonEdge *e)
	{
		m_edges.push_back(e);
		e->setSkeleton(this);
		e->setCurve(m_pShape->getCurves()->front());
	}

	void eraseEmptyEdge(DDSNode *src, DDSNode *tgt);

	const DDSEdgeVect& getEdges() const { return m_edges; }
	DDSEdgeVect& getEdges()             { return m_edges; }

	const DDSNodeVect& getNodes() const { return m_nodes; }
	DDSNodeVect& getNodes()             { return m_nodes; }

	const Shape* getShape() const       { return m_pShape; }
	Shape* getShape()                   { return m_pShape; }

	unsigned int edgeCount() const      { return m_edges.size(); }

	unsigned int nodeCount() const      { return m_nodes.size(); }

	const DivergenceMap* getDivergenceMap() { return m_pDivMap; }

	virtual ~DiscreteDivergenceSkeleton()
	{
		DDSEdgeVect::iterator edgeIt;
		DDSNodeVect::iterator nodeIt;

		for(edgeIt = m_edges.begin(); edgeIt != m_edges.end(); edgeIt++)
			delete (*edgeIt);

		for(nodeIt = m_nodes.begin(); nodeIt != m_nodes.end(); nodeIt++)
			delete (*nodeIt);

		delete m_pDivMap;
		delete m_pShape;
	}

	void computeBoundaryPointPair(const FluxPoint& fp0, const FluxPoint& fp1, 
				const FluxPoint& fp2, BoundaryPoint& bp1, BoundaryPoint& bp2, Vector& t);

	void computeFluxPointBoundaryInfo();
};

} // namespace sg

#endif // DISCRETE_DIVERGENCE_SKELETON_H
