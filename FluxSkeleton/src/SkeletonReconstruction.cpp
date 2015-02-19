/************************************************************************

File:		testDivergenceSkeletonMaker.cpp

Author(s):		Pavel Dimitrov with modifications by Diego Macrini

Created:		27 Jun 2002

Last Revision:	$Date: 2003/06/05 04:44:54 $

Description:	

$Revision: 1.8 $

$Log: SkeletonReconstruction.cpp,v $

Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

***********************************************************************/

// The code in this file needs some extra variables in the boundary
// info structure associated with each flux point. At this point,
// we don't use the code here anymore, and so the define USE_EXTRA_BOUNDARY_INFO
// is created to omit the code and all the associated declarations of variables
#ifdef USE_EXTRA_BOUNDARY_INFO

#include "sg.h"
//#include "FastDistanceTransform.h"
#include "../../dagmatcher/PolyLineApprox.h"
#include "../../dagmatcher/PolyBezierApprox.h"

#ifdef WIN32
#include <float.h>
#define isnan  _isnan
#define finite _finite
#endif

using namespace sg;

Vector VectorFromPOINT(const dml::POINT& pt)
{
	return Vector(pt.x, pt.y);
}

/*!
	@brief Computes alpha prime (d alpha / ds) for boundary-to-axis ratio

	It assumes that the "val" values in each flux pount are smooth!
*/
double computeAlphaPrime(const FluxPoint& fpm2, 
					     const FluxPoint& fpm1, 
					     const FluxPoint& fpm0, 
					     const FluxPoint& fpp1, 
					     const FluxPoint& fpp2)
{
	// do not consider "bad points"
	if (fabs(fpm2.val) > 4.0 || 
		fabs(fpm1.val) > 4.0 || 
		fabs(fpm0.val) > 4.0 || 
		fabs(fpp1.val) > 4.0 || 
		fabs(fpp2.val) > 4.0)
		return 0.0;

	const int i = 2;
	double a[5]= { asin(fabs(fpm2.val) / 4.0),  // alpha at i-2
		asin(fabs(fpm1.val) / 4.0),  // alpha at i-1
		asin(fabs(fpm0.val) / 4.0),  // alpha at i
		asin(fabs(fpp1.val) / 4.0),  // alpha at i+1
		asin(fabs(fpp2.val) / 4.0),  // alpha at i+2
	};
	double h = fpm2.p.distanceToPt(fpp2.p) / 4.0;

	double alpha_p = (-a[i+2] + 8*a[i+1] - 8*a[i-1] + a[i-2]) / (12*h);
	//double alpha_p = (a[i+1] - a[i-1]) / (2*h)

	double tmpap = sin(a[i]) / fabs(fpm0.dist);

	if(alpha_p > tmpap)
		alpha_p = tmpap;

	return alpha_p;
}

/*!
	@brief Computes the boundary-to-axis ratio. The formula is as follows:
	dB / ds  = r(s) (alpha'(s) +- kappa(s)) - sin(alpha)

	but we return the combined ratio for both sides, i.e.
	2 r(s) alpha'(s) - 2 sin(alpha)

	NOTE: Must have computed alpha_p on the FluxPoints !
*/
double computeFluxPointBAR(const FluxPoint& fp, const double& alpha_p)
{
	if (fabs(fp.val) > 4.0)
		return 2.0;

	double alpha = asin(fabs(fp.val) / 4.0);
	double r = fabs(fp.dist);

	return fabs(r * 2 * fabs(alpha_p) - 2 * sin(alpha));
}

/*!
	@brief Computes the boundary-to-axis ratio (BAR), alpha_p, and boundary point
	       pair information for every flux point in the branch.

	Check if boundary info 'bi' defines ligature
	   if (fabs(bi->bar) < threshold) => ligature

	Check if boundary info 'bi' has valid boundary points
	   if (bi.first.valid && fabs(bi.first.dtValue) < 1)   => first point is good
	   if (bi.second.valid && fabs(bi.second.dtValue) < 1) => second point is good
*/
void DDSEdge::computeBoundaryInfoList()
{
	BoundaryInfoList& bil = boundary_info_list;
	FluxPointList fpl = flux_points; // copy for smoothing "val"
	FluxPointList::iterator I;
	BoundaryInfoList::iterator II;

	ASSERT(bil.size() == 0);

	bil.resize(fpl.size());

	//computeValueApproximation(fpl, bil);
	computeTangents(fpl, bil);

	if (fpl.size() >= 3)
	{
		FluxPointList::iterator oneBeforeLast = fpl.end() - 1;

		// Compute boundary point pairs BEFORE smoothing the flux values
		for(I = fpl.begin() + 1, II = bil.begin() + 1; I != oneBeforeLast; I++, II++)
			dds->computeBoundaryPointPair(*(I-1), *I, *(I+1), II->first, II->second, II->tangent);

		// smooth the flux values BEFORE computing derivatives (ie, alpha_p)
		for(I = fpl.begin() + 1; I != oneBeforeLast; I++)
			(*I).val = (0.3*(*I).val + 0.4*(*(I-1)).val + 0.3*(*(I+1)).val);
	}

	// initialize da/ds to 0 and BAR to 2 for all FluxPoint's 
	for(II = bil.begin(); II < bil.end(); II++)
	{
		II->alpha_p = 0.0;
		II->first.bar = 2.0;
	}

	if (fpl.size() >= 5)
	{
		FluxPointList::iterator twoBeforeLast = fpl.end() - 2;

		// calculate initial da/ds and boundary-to-axis ratio
		for(I = fpl.begin() + 2, II = bil.begin() + 2; I != twoBeforeLast; I++, II++)
		{
			II->alpha_p = fabs(computeAlphaPrime(*(I-2), *(I-1), *I, *(I+1), *(I+2)));
			II->first.bar = computeFluxPointBAR(*I, II->alpha_p);
		}
	}
}

/*!
	@brief Computes the two contour points associated with flux point fp1

	@param fp0 flux point before fp1
	@param fp2 flux point after fp1

	Note: a good boundary point 'bp' satisfies: fabs(dt->getValue(bp.pt)) < 1
	Then, to test for a good boundary point 'bp' check
	if (bp.valid && fabs(bp.dtValue) < 1) ...
*/
void DDSkeleton::computeBoundaryPointPair(const FluxPoint& fp0, const FluxPoint& fp1, 
						const FluxPoint& fp2, BoundaryPoint& bp1, BoundaryPoint& bp2,
						Vector& tangent)
{
	bp1.index = bp2.index = -1;

	if (tangent.x == 0 && tangent.y == 0)
	{
		tangent.x = fp2.p.x - fp0.p.x;
		tangent.y = fp2.p.y - fp0.p.y;
	}

	// is the direction of the tangent good ?
	if (fabs(fp2.dist) > fabs(fp0.dist))
	{
		tangent.x = -tangent.x;
		tangent.y = -tangent.y;
		//alpha = PI - alpha;
	}
	
	// the tangent must be a unit vector
	tangent.normalize();

	const double Pi = 3.141592654;
	double alpha;

	if (fabs(fp1.val) > 4.1)
		return;

	if (fabs(fp1.val) > 4.0)
		alpha = Pi / 2.0;
	else
		alpha = asin(fabs(fp1.val) / 4.0);  

	// do not reconstruct sensitive points
	if(fabs(fp1.val) < 0.50) 
		return;

	DistanceTransform* dt = getDivergenceMap()->getDT();

	Vector t = tangent;

	// rotate the t by alpha
	t.rotate(alpha);

	// the reconstructed contour pt
	bp1.pt = fp1.p;
	bp1.pt.x += fabs(fp1.dist) * t.x;
	bp1.pt.y += fabs(fp1.dist) * t.y;

	bp1.index = (!isnan(bp1.pt.x) && !isnan(bp1.pt.y)) ? 0:-2;

	if (bp1.index >= 0)
		bp1.dtValue = dt->getValue(bp1.pt);

	// the other side of the skeletal branch now
	t.rotate(-2 * alpha); 
	bp2.pt = fp1.p;
	bp2.pt.x += fabs(fp1.dist) * t.x;
	bp2.pt.y += fabs(fp1.dist) * t.y;

	bp2.index = (!isnan(bp2.pt.x) && !isnan(bp2.pt.y)) ? 0:-2;

	if (bp2.index >= 0)
		bp2.dtValue = dt->getValue(bp2.pt);
}

/*!
	@brief Computes the two contour points associated with each FluxPoint
	along with the BAR and alpha_p values.
*/
void DDSkeleton::computeFluxPointBoundaryInfo()
{
	DDSEdgeVect& edges = getEdges();
	DDSEdgeVect::iterator I;

	for(I = edges.begin(); I != edges.end(); I++)
		(*I)->computeBoundaryInfoList();
}

void DDSEdge::computeTangents(FluxPointList& fpl, BoundaryInfoList& bil)
{
	const int nSize = flux_points.size();
	int i;

	dml::POINTS dataXY(nSize);

	for (i = 0; i < nSize; i++)
		dataXY[i].Set(flux_points[i].p.x, flux_points[i].p.y);

	dml::PolyBezierApprox polyXY(dataXY.GetSize() / 8.0, 4);

	polyXY.Fit(dataXY);

	for (i = 0; i < nSize; i++)
		bil[i].tangent = VectorFromPOINT(polyXY.GetTangent(i));
}

void DDSEdge::computeValueApproximation(FluxPointList& fpl, BoundaryInfoList& bil)
{
	int i, nSize = flux_points.size() - 2;
	double x, dx, dy;

	dml::POINTS dataVal(nSize), dataRad(nSize);

	for (i = 0; i < nSize; i++)
	{
		const FluxPoint& fp = flux_points[i + 1];

		if (i == 0)
			x = 0;
		else
		{
			dx = fp.p.x - flux_points[i].p.x;
			dy = fp.p.y - flux_points[i].p.y;

			x += sqrt(dx * dx + dy * dy);
		}

		dataVal[i].Set(x, fabs(fp.val));
		dataRad[i].Set(x, fabs(fp.dist));
	}

	dml::PolyLineApprox polyVal(dataVal.GetSize() / 50.0, 0.0005, 10, -1);
	dml::PolyLineApprox polyRad(dataRad.GetSize() / 4.0, 0.05, 10, 0.1);
	
	//polyVal.SetDbgMode(true);

	polyVal.Fit(dataVal);
	polyRad.Fit(dataRad);

	ASSERT(fpl.size() == nSize + 2);

	// First and last flux points keep ther original values
	fpl[0] = flux_points[0];
	fpl[nSize + 1] = flux_points[nSize + 1];

	// Read new flux values
	/*for (i = 0, j = 0; i < nSize;)
	{
		ASSERT(j < polyVal.m_knots.GetSize());

		if (i <= polyVal.m_knots[j].nIndex)
		{
			fpl[i + 1].val = polyVal.m_knots[j].seg.Y(dataVal[i].x);

			// If the point is a knot, we comp average using the next segment too
			if (i == polyVal.m_knots[j].nIndex && j < polyVal.m_knots.GetSize() - 1)
			{
				fpl[i + 1].val = (fpl[i + 1].val + polyVal.m_knots[j + 1].seg.Y(dataVal[i].x)) / 2.0;
			}
			
			i++;
		}
		else
			j++;
	}*/

	// Read new flux values
	for (i = 0; i < nSize; i++)
		fpl[i + 1].val = polyVal.GetPointSegment(i).Y(dataVal[i].x);

	// Read new radius values
	for (i = 0; i < nSize; i++)
		fpl[i + 1].dist = polyRad.GetPointSegment(i).Y(dataRad[i].x);
}

#endif