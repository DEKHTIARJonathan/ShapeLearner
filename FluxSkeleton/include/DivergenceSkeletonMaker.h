/**************************************************************************

File:                DivergenceSkeletonMaker.h

Author(s):           Pavel Dimitrov

Created:             27 Jun 2002

Last Revision:       $Date: 2002/07/25 20:50:47 $

Description: 

$Revision: 1.5 $

$Log: DivergenceSkeletonMaker.h,v $
Revision 1.5  2002/07/25 20:50:47  pdimit
Making release 0.1

Revision 1.4  2002/07/23 21:02:51  pdimit
The branch segmentation of the thinned DivMap is improved --
the two pixel branches are now included. Still, the degenerate case
of the square of junction-points is not taken care of.

Also, a DiscreteDivergenceSkeleton is created which still does not know
of left and right for the branches, i.e. the contour has not been cut.

Revision 1.3  2002/07/01 07:03:09  pdimit
Now the thinned discrete divergence array is disected properly
into branches; that is, the algorithm seems to be sound.

Revision 1.2  2002/06/30 05:58:43  pdimit
Just implemented the colouring of the thinned DivergenceMap.
Also, fixed a bug in the way of counting the number of intersection
in the 8-nbhd of a pt.

Revision 1.1  2002/06/30 01:22:30  pdimit
Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
thinning of the DivergenceMap is implemented.



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

**************************************************************************/

#ifndef DIVERGENCESKELETONMAKER_H
#  define DIVERGENCESKELETONMAKER_H

#include "DivergenceMap.h"
#include "DiscreteDivergenceSkeleton.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#define DEBUG_DIVERGENCE_SKELETON_MAKER

namespace sg {

	const char FOREGROUND_COL       =0;
	const char BACKGROUND_COL       =1;
	const char SKELETON_COL         =2;
	const char SK_POINT_COL         =3;
	const char END_POINT_COL        =4;
	const char BRANCHING_POINT_COL  =5;


	struct DivPt {
		Point p;
		double val; // divergence value
		double dist; // closest distance

		char col; // for the thinning and skeleton

		bool visited; // for the skeleton creation (branches)

		int x, y; // array position

		DivPt():visited(false), x(0),y(0) { 
			dist = 0;
			col=FOREGROUND_COL; 
		}

		DivPt(Point pt, double div):visited(false),x(0),y(0){
			dist=0;
			p = pt;
			val = div;
			col=FOREGROUND_COL;
		}

		bool operator<(const DivPt &dp) const{
			return val<dp.val; // since max-heap
		}
	};

	////
	//  typedef std::vector<DivPt> DivPtVector;

	class DivPtVector : public std::vector<DivPt> 
	{
	public:
		DiscreteDivergenceSkeletonNode *n1, *n2;

		DivPtVector () { n1=0; n2=0; }

	};


	////
	class DivArr 
	{
		int xSize, ySize;
		DivPt *buf;

		DivPt deflt;


	public:

		DivArr(int xs, int ys){
			xSize = xs;
			ySize = ys;

			buf = new DivPt[xSize*ySize];

			deflt.val = 10000;
			deflt.col = BACKGROUND_COL;
		}

		DivArr(std::vector<DivPt> pts, int xs){
			xSize = xs;
			ySize = pts.size()/xs;

			buf = new DivPt[pts.size()];
			deflt.col = BACKGROUND_COL;

			std::vector<DivPt>::iterator I;
			int i;
			for(i=0, I=pts.begin(); I!=pts.end(); I++, i++){
				buf[i] = *I;
				buf[i].x = i % xs;
				buf[i].y = i / xs;
			}

			deflt.val = 10000;
		}

		~DivArr() 
		{ 
			delete[] buf; 
		}

		void writeColToFile(char* fname) const
		{
			//FILE* fp = fopen(fname, "w");
			FILE* fp = NULL;
			fopen_s(&fp, fname, "w");
			if(!fp) return;
			int index = 0;
			for(int i = 0; i < ySize; i++)
			{
				for(int j = 0; j < xSize; j++)
				{
					if(int(buf[index].col) == 1)
						fprintf(fp,"_");
					else
						fprintf(fp,"%d",int(buf[index].col));
					//std::cerr << "Col " << int(buf[index].col) << std::endl;
					index++;
				}
				fprintf(fp,"\n");
			}
			fclose(fp);
		}

		void writeValToFile(char* fname) const
		{
			//FILE* fp = fopen(fname, "w");
			FILE* fp = NULL;
			fopen_s(&fp, fname, "w");
			if(!fp) return;
			int index = 0;
			for(int i = 0; i < ySize; i++)
			{
				for(int j = 0; j < xSize; j++)
				{
					double dValue = buf[index].val;
					int nColor =int(buf[index].col);
					if(nColor == 1) dValue = 0;
					fprintf(fp,"%6.2lf",dValue);
					//std::cerr << "Col " << int(buf[index].col) << std::endl;
					index++;
				}
				fprintf(fp,"\n");
			}
			fclose(fp);
		}

		void writeDistToFile(char* fname) const
		{
			//FILE* fp = fopen(fname, "w");
			FILE* fp = NULL;
			fopen_s(&fp, fname, "w");
			if(!fp) return;
			int index = 0;
			for(int i = 0; i < ySize; i++)
			{
				for(int j = 0; j < xSize; j++)
				{
					fprintf(fp,"%2d",int(buf[index].dist));
					//std::cerr << "Col " << int(buf[index].col) << std::endl;
					index++;
				}
				fprintf(fp,"\n");
			}
			fclose(fp);
		}



		DivPt &operator()(int x, int y){
			if(x>=0 && x<xSize && y>=0 && y<ySize)
				return buf[y*xSize + x];    
			else{
				//std::cerr << "BTTD ";
				return deflt;
			}
		}

		int getXSize() const { return xSize; }
		int getYSize() const { return ySize; }

	};

	////////////////////////////////////////////////
	////////////////////////////////////////////////
	class DivergenceSkeletonMaker{
	protected:
		//DivergenceMap *dm;
		//DivArr *da;

	public:
		/*DivergenceSkeletonMaker(const DivergenceMap &dm){
		this->dm = dm.clone();
		}
		DivergenceSkeletonMaker(const DivergenceMap *dm){
		this->dm = dm->clone();
		}*/

		//DivArr& getThinDivArr(double step, double thresh);

		DiscreteDivergenceSkeleton* getDiscreteDivergenceSkeleton(
			Shape* pShape, int resolution, const double& step, const double& thresh);

		DiscreteDivergenceSkeleton* getDiscreteDivergenceSkeleton(
			DivergenceMap* dm, const double& step, const double& thresh);

		void make_skeleton(DivergenceMap* dm, double step = 1.0, 
			double thresh = -4);

		void colour_skeleton_array(DivArr &da);
	};

	DiscreteDivergenceSkeleton* create_skeleton(
		DivArr &da, DivergenceMap* dm, 
		DiscreteDivergenceSkeleton* skeleton = NULL);

}

#endif  /* DIVERGENCESKELETONMAKER_H */
