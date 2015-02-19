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

#ifndef DISCRETE_DIVERGENCE_SKELETON_EDGE_H
#define DISCRETE_DIVERGENCE_SKELETON_EDGE_H

#include <iostream>
#include "Curve.h"
#include "FluxPoint.h"
#include "BoundaryInfo.h"
#include "BoundarySegmentLists.h"
#include "DiscreteDivergenceSkeletonNode.h"

namespace sg {

typedef std::vector<FluxPoint> FluxPointList;
typedef std::vector<BoundaryInfo> BoundaryInfoList;
typedef std::vector<BoundarySegment> BoundarySegments;

/*!
	Edge of a DiscreteDivergenceSkeleton graph 
*/
class DiscreteDivergenceSkeletonEdge
{
public: //should be protected but AFMMSkeleton.h must be corrected first
	DiscreteDivergenceSkeleton *dds;        //<! Parent DDSGraph
	DiscreteDivergenceSkeletonNode* n1;     //<! branch terminal node 1
	DiscreteDivergenceSkeletonNode* n2;     //<! branch terminal node 2

	FluxPointList flux_points;              //<! Branch skeletal points
	BoundaryInfoList boundary_info_list;    //<! Boundary info associated with each flux point
	BoundarySegmentLists boundary_segments; //<! List of boundary gaps. Empty if no gaps.

	void* pEdgeData;                        //<! Used by the GeneralizedSkeletalGraph class

public:
	Curve* curve;
	bool has_right;
	double right_t1, right_t2;              //<! contour segment limits on the right side

	bool has_left;
	double left_t1, left_t2;                //<! contour segment limits on the right side

	/*!
		This value is used in the AFMMSkeleton to know the reconstruction error of
		a given edge during the simplification step. It must be initialized to -1.
	*/
	double m_dEgdeRecError;

	double getSegDist(double st1, double st2)
	{
		double t1, t2;
		Point p1 = flux_points.front().p;
		Point p2 = flux_points.back().p;

		Distance dst = curve->computeDistance(p1, st1, st2);
		t1 = dst.t;
		dst = curve->computeDistance(p2, st1, st2);
		t2 = dst.t;

		// std::cerr << "\nst1: " <<st1 << " st2: " << st2;
		// std::cerr << " t1: " <<t1 << " t2: " << t2 << "\n";

		if (st1 <= st2)
		{
			return fabs(t1-t2);
		}
		else // st1 > st2
		{ 

			if(t1 >= st1 && t2 < st1)
				return curve->getLength() - t1 + t2;

			if(t2 >= st1 && t1 < st1)
				return curve->getLength() - t2 + t1;

			return fabs(t1-t2); // otherwise, both t1 and t2 are on the same side
		}
	}

public:
	DiscreteDivergenceSkeletonEdge(DiscreteDivergenceSkeleton *dds,
		FluxPointList flux_points,
		DiscreteDivergenceSkeletonNode *n1,
		DiscreteDivergenceSkeletonNode *n2) 
	{
			this->dds = dds;
			this->flux_points = flux_points;

			this->n1 = n1; 
			n1->addEdge(this);

			this->n2 = n2; 
			n2->addEdge(this);

			has_right = false;
			right_t1 = -1; 
			right_t2 = -1;

			has_left = false;
			left_t1 = -1; 
			left_t2 = -1;

			m_dEgdeRecError = -1;
	}

	//! Used by dml::GSG only
	DiscreteDivergenceSkeletonEdge(void* pData)
	{
		dds = NULL;
		n1  = NULL;
		n2  = NULL;
		pEdgeData = pData;
	}

	bool hasRight() { return has_right; }

	void setRightSegment(double t1, double t2)
	{
		right_t1 = t1;
		right_t2 = t2;

		has_right=true;
	}

	bool hasLeft() { return has_left; }

	void setLeftSegment(double t1, double t2)
	{
		left_t1 = t1;
		left_t2 = t2;

		has_left = true;
	}

	double leftT1() { return left_t1; }
	double leftT2() { return left_t2; }
	double rightT1() { return right_t1; }
	double rightT2() { return right_t1; }

	double getRightSegLength() 
	{
		return getSegDist(right_t1, right_t2);
	}

	double getLeftSegLength() 
	{
		return getSegDist(left_t1, left_t2);
	}

	void print_left_right_seg(Curve *c)
	{
		Point lp1 = c->atT(left_t1);
		Point lp2 = c->atT(left_t2);
		Point rp1 = c->atT(right_t1);
		Point rp2 = c->atT(right_t2);

		std::cerr << "left: " << lp1 << " to " << lp2;
		std::cerr << " [" << left_t1 << ", " << left_t2 << "] " <<getLeftSegLength()<< " \n";
		std::cerr << "right: " << rp1 << " to " << rp2;
		std::cerr << " [" << right_t1 << ", " << right_t2 << "] "<<getRightSegLength()<< "\n";
	}

	void setCurve(Curve* c){ curve = c; }

	void setSkeleton(DiscreteDivergenceSkeleton* sk){ dds = sk; }

	const FluxPointList& getFluxPoints() const { return flux_points; }
	FluxPointList& getFluxPoints() { return flux_points; }

	DiscreteDivergenceSkeletonNode* getN1() { return n1; }
	DiscreteDivergenceSkeletonNode* getN2() { return n2; }
	DiscreteDivergenceSkeleton* getSkeleton() { return dds; }

	void computeBoundaryInfoList();
	const BoundaryInfoList& getBoundaryInfoList() const { return boundary_info_list; }
	BoundaryInfoList& getBoundaryInfoList() { return boundary_info_list; }
	void setBoundaryInfoList(const BoundaryInfoList& bil) { boundary_info_list = bil; }

	const BoundarySegmentLists& getBoundarySegments() const { return boundary_segments; }
	BoundarySegmentLists& getBoundarySegments() { return boundary_segments; }

	void computeValueApproximation(FluxPointList& fpl, BoundaryInfoList& bil);
	void computeTangents(FluxPointList& fpl, BoundaryInfoList& bil);

	unsigned int size() const { return flux_points.size(); }

	//! Inits boundary infor array. Only first and last points are "cleared"
	void initBoundaryInfoList()
	{
		boundary_info_list.resize(flux_points.size());

		// Make sure that at least the first and last points are cleared
		boundary_info_list.front().clear();
		boundary_info_list.back().clear();
	}

	const FluxPoint& getFluxPoint(unsigned int i) const { return flux_points[i]; }

	const FluxPoint& getFirstFluxPoint() const { return flux_points[0]; }
	const FluxPoint& getLastFluxPoint() const { return flux_points[flux_points.size() - 1]; }

	const Point& getFirstXYPoint() const { return flux_points[0].p; }
	const Point& getLastXYPoint() const { return flux_points[flux_points.size() - 1].p; }

	const BoundaryInfo& getBoundaryInfo(unsigned int i) const { return boundary_info_list[i]; }

	const BoundaryInfo& getFirstBndryInfo() const { return boundary_info_list[0]; }

	const BoundaryInfo& getLastBndryInfo() const 
	{ 
		return boundary_info_list[boundary_info_list.size() - 1]; 
	}

	//! Checks if its a terminal branch. ie, least one endpoint node has degree 1
	bool isTerminal() const
	{
		return (n1->degree() == 1 || n2->degree() == 1);
	}
};

} //namespace sg

#endif // DISCRETE_DIVERGENCE_SKELETON_EDGE_H
