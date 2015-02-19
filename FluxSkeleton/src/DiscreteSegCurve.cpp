/************************************************************************

File:		DiscreteSegCurve.cpp

Author(s):		Pavel Dimitrov

Created:		12 Jun 2002

Last Revision:	$Date: 2002/07/25 20:50:49 $

Description:	

$Revision: 1.3 $

$Log: DiscreteSegCurve.cpp,v $
Revision 1.3  2002/07/25 20:50:49  pdimit
Making release 0.1

Revision 1.2  2002/06/26 04:30:44  pdimit
After the failed attempt at getting the spline smoothing to work

Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
Initial import



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

***********************************************************************/

#define DISCRETESEGCURVE_CPP

#include "DiscreteSegCurve.h"
#include <stdio.h>

#include "LineSeg.h"

//#include "../../dagmatcher/BasicUtils.h"

using namespace sg;

DiscreteSegCurve::DiscreteSegCurve(std::vector<CurveSeg*> &segs, bool closed){
	shortest_seg = -1;

	this->closed = closed;
	length = 0;

	std::vector<CurveSeg*>::iterator I;

	for(I = segs.begin(); I != segs.end(); I++){
		CurveSeg *cs = (*I); 
		cs->startLength = length;  // define start position in curve
		length += cs->getLength(); // update total curve length

		if((shortest_seg < 0) || 
			(shortest_seg > cs->getLength()))
			shortest_seg = cs->getLength();

		segments.push_back(cs);    // put in storage;
	}
}

void DiscreteSegCurve::drawCurve(Drawable &d){

	std::vector<CurveSeg*>::iterator I;
	for(I=segments.begin(); I!=segments.end(); I++){
		(*I)->drawSeg(d);
	}

}

Distance 
DiscreteSegCurve::computeDistance(const Point &p, double t1, double t2){

	std::vector<CurveSeg*>::iterator I=segments.begin();
	std::vector<CurveSeg*>::iterator II;
	double dist = -1;
	CurveSeg* cs=*I;//, *cs1;

	Distance distance;
	double tval = 0;

	//std::cerr << "\ncomputeDistance t1 "<<t1<<" t2 "<<t2<<"\n";

	for(I=segments.begin(); I!=segments.end(); I++){

		II = I;

		double d;

		double st1 = (*I)->startLength;
		double st2 = st1 + (*I)->getLength();

		if(t1 < t2){


			if(st1 <= t2 &&
				st2 >= t1) // intersects [t1, t2]
			{
				double tt1=0, tt2=(*I)->getLength();

				if (st1 <= t1)
					tt1 = t1 - (*I)->getLength();

				if (st2 >= t2)
					tt2 = t2 - (*I)->getLength();

				Distance dst = (*I)->computeDistance(p, tt1, tt2);
				d = dst.dist;

				if(dist < 0 || dist > d){
					cs = *I; 

					dist = d;
					distance = dst;
					tval = dst.t+cs->startLength;

					//std::cerr << d << "\t" << dst.t+cs->startLength << " here\n";
				}
			}
		}
		else { // t1 > t2

			if((st2 >= t1) ||
				(st1 <= t2)){ // int. [t1, length] or intersects [0, t2]
					cs = *I; 

					double tt1=0, tt2=cs->getLength();

					if(t1 <= cs->startLength){
						tt1 = 0;
						tt2 = cs->getLength();
					}

					if(t1 > cs->startLength){
						tt1 = t1 - cs->startLength;
						tt2 = cs->getLength();
					}


					if(t2 >= cs->startLength + cs->getLength()){
						tt1 = 0;
						tt2 = cs->getLength();	    
					}

					if(t2 < cs->startLength + cs->getLength()){
						tt1 = t2 - cs->startLength;
						tt2 = cs->getLength();	    
					}

					Distance dst = cs->computeDistance(p);//, tt1, tt2);
					d = dst.dist;
					d = cs->distToPt(p);

					if(dist < 0 || dist > d){
						dist = d;
						distance = dst;	    
						tval = dst.t+cs->startLength;

					}

			}

		}

	}

	// computing the signed distance now. see Curve.h for a description.
	Distance d = distance; //cs->computeDistance(p);
	double t = d.t; //cs->closestToPt(p);

	Vector TQ = cs->tangent(t);

	Point Q = cs->atT(t);
	Vector v(p.x-Q.x, p.y-Q.y);

	//return TQ.x*v.y - TQ.y*v.x; 
	if( TQ.x*v.y - TQ.y*v.x > 0)
		d.dist = d.dist;
	else
		d.dist = -d.dist;

	d.t = tval;
	//std::cerr << "\nd.t : " << d.t <<"\n"; 
	return d;


}

Distance DiscreteSegCurve::computeDistance(const Point &p){

	//double DiscreteSegCurve::distToPt(const Point &p){
	std::vector<CurveSeg*>::iterator I=segments.begin();
	std::vector<CurveSeg*>::iterator II;
	double dist = -1;
	CurveSeg* cs=*I;//, *cs1;

	for(I=segments.begin(); I!=segments.end(); I++){

		II = I;
		double d = (*I)->distToPt(p);

		if(dist < 0 || dist > d){
			cs = *I;
			dist = d;
		}
	}

	// computing the signed distance now. see Curve.h for a description.
	Distance d = cs->computeDistance(p);
	double t = d.t; //cs->closestToPt(p);
	Vector TQ = cs->tangent(t);
	//Vector vv(0,0);

	//    if (t == cs->getLength()){ // in case t is at the end of the segment we
	//                               // need to look at the beginning of the next one
	//      II++;
	//      if(II == segments.end())
	//        II=segments.begin();

	//      cs1 = *II; 
	//      vv = cs1->tangent(0);

	//      Vector mv((TQ.x + vv.x)/2, (TQ.y + vv.y)/2);

	//      if (vv.x !=0 || vv.y!=0){
	//        if ((TQ.x*vv.y - TQ.y*vv.x) *(TQ.x*mv.y - TQ.y*mv.x) > 0)
	//  	TQ = mv;
	//        else{
	//  	TQ.x = -mv.x; 
	//        TQ.y = -mv.y;
	//        }

	//      }
	//    }

	//    if (t == 0){ // in case t is at the beginning of the segment we
	//                 // need to look at the end of the previous one
	//      if(II == segments.begin()){
	//        II=segments.end();
	//      }
	//      else
	//        II--;

	//      cs1 = *II; 
	//      vv = cs1->tangent(cs1->getLength());

	//      Vector mv((TQ.x + vv.x)/2, (TQ.y + vv.y)/2);

	//      if (vv.x !=0 || vv.y!=0){
	//        if ((-TQ.x*vv.y + TQ.y*vv.x) *(-TQ.x*mv.y + TQ.y*mv.x) > 0)
	//  	TQ = mv;
	//        else{
	//  	TQ.x = -mv.x; 
	//  	TQ.y = -mv.y;
	//        }

	//      }
	//    }


	Point Q = cs->atT(t);
	Vector v(p.x-Q.x, p.y-Q.y);

	//return TQ.x*v.y - TQ.y*v.x; 
	if( TQ.x*v.y - TQ.y*v.x > 0)
		d.dist = dist;
	else
		d.dist = -dist;

	d.t = d.t + cs->startLength;

	return d;
}

double DiscreteSegCurve::distToPt(const Point &p){

	std::vector<CurveSeg*>::iterator I=segments.begin();
	std::vector<CurveSeg*>::iterator II;
	double dist = -1;
	CurveSeg* cs=*I;//, *cs1;

	for(I=segments.begin(); I!=segments.end(); I++){

		II = I;
		double d = (*I)->distToPt(p);

		if(dist < 0 || dist > d){
			cs = *I;
			dist = d;
		}
	}

	// computing the signed distance now. see Curve.h for a description.
	Distance d = cs->computeDistance(p);
	double t = cs->closestToPt(p);
	Vector TQ = cs->tangent(t);
	//  Vector vv(0,0);



	Point Q = cs->atT(t);
	Vector v(p.x-Q.x, p.y-Q.y);

	//return TQ.x*v.y - TQ.y*v.x; 
	if( TQ.x*v.y - TQ.y*v.x > 0)
		d.dist = dist;
	else
		d.dist = -dist;

	return d.dist;

}

//  double DiscreteSegCurve::distToPt(const Point &p){
//    Distance d =  computeDistance(p);

//    return d.dist;
//  }

// returns the t value for which curve(t) is closest to p
// NOTE: ineficient !!!
double DiscreteSegCurve::closestToPt(const Point &p){
	std::vector<CurveSeg*>::iterator I;
	double dist = -1;
	double t = 0;
	for(I=segments.begin(); I!=segments.end(); I++){
		double d = (*I)->distToPt(p);

		if(dist < 0 || dist > d){
			dist = d;
			double st = (*I)->closestToPt(p); 
			t = st + (*I)->startLength;
		}
	}
	return t;
}

Point DiscreteSegCurve::atT(double t){

	if(t > length) return Point(-1,-1); // this should really be an exception or smthg...

	std::vector<CurveSeg*>::iterator I;

	for(I=segments.begin(); I!=segments.end(); I++){
		CurveSeg *cs = *I;

		if (cs->startLength <= t &&
			cs->getLength() + cs->startLength >= t)
			return cs->atT(t - cs->startLength);
	}
	return Point(-1,-1);
}

Vector DiscreteSegCurve::tangent(double t){

	//if(t > length) return Vector(-1,-1); // this should really be an exception or smthg...

	ASSERT(t >= 0 && t <= length);

	std::vector<CurveSeg*>::iterator I;

	for(I = segments.begin(); I != segments.end(); I++)
	{
		CurveSeg* cs = *I;

		if (cs->startLength <= t &&	cs->getLength() + cs->startLength >= t)
			return cs->tangent(t - cs->startLength);
	}

	return Vector(-1,-1);
}

Vector DiscreteSegCurve::tangent(unsigned int idx, Point* pPoint /*=NULL*/)
{
	ASSERT(idx >= 0 && idx < segments.size());

	CurveSeg* cs = segments[idx];

	/*DBG_PRINTL4("Num segments =", segments.size(), "idx =", idx);

	if (pPoint)
	{
	LineSeg* ls = dynamic_cast<LineSeg*>(cs);
	if (ls)
	if (ls->startPt != *pPoint)
	std::cerr << ls->startPt << " != " << *pPoint << "... next pt = " << ls->endPt << std::endl;
	else
	std::cerr << "We do not have a LineSeg pointer as we thought!!!!" << std::endl;
	}*/

	//ASSERT(!pPoint || *pPoint == dynamic_cast<LineSeg*>(cs)->startPt);

	return cs->tangent(cs->startLength);
}

Curve* DiscreteSegCurve::clone() const
{
	DiscreteSegCurve *dsc = new DiscreteSegCurve();

	dsc->length = length;
	dsc->closed = closed;  
	dsc->shortest_seg = shortest_seg;

	std::vector<CurveSeg*>::const_iterator I;

	for(I = segments.begin(); I != segments.end(); I++)
	{
		CurveSeg *cs = *I;

		dsc->segments.push_back(cs->clone());
	}  

	return dsc;

}
