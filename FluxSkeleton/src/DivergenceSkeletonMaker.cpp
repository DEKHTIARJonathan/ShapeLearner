/************************************************************************

File:		DivergenceSkeletonMaker.cpp

Author(s):		Pavel Dimitrov

Created:		27 Jun 2002

Last Revision:	$Date: 2002/07/25 20:50:49 $

Description:	

$Revision: 1.6 $

$Log: DivergenceSkeletonMaker.cpp,v $
Revision 1.6  2002/07/25 20:50:49  pdimit
Making release 0.1

Revision 1.5  2002/07/23 21:02:53  pdimit
The branch segmentation of the thinned DivMap is improved --
the two pixel branches are now included. Still, the degenerate case
of the square of junction-points is not taken care of.

Also, a DiscreteDivergenceSkeleton is created which still does not know
of left and right for the branches, i.e. the contour has not been cut.

Revision 1.4  2002/07/01 07:03:30  pdimit
Now the thinned discrete divergence array is disected properly
into branches; that is, the algorithm seems to be sound.

Revision 1.3  2002/06/30 11:13:38  pdimit
There are now skeleton creation functions that are able to compute
the list of branches. Seems to be working fine, but more testing may be
needed.

Revision 1.2  2002/06/30 05:58:47  pdimit
Just implemented the colouring of the thinned DivergenceMap.
Also, fixed a bug in the way of counting the number of intersection
in the 8-nbhd of a pt.

Revision 1.1  2002/06/30 01:22:31  pdimit
Added DivergenceSkeletonMaker and a test for it. Incomplete functionality
and the testing is mostly in DivergenceSkeletonMaker.cpp. Only the
thinning of the DivergenceMap is implemented.



Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
McGill University, Montreal, QC.  Please see the copyright notice
included in this distribution for full details.

***********************************************************************/

#define DIVERGENCESKELETONMAKER_CPP

#include "defines.h"
#include "DivergenceSkeletonMaker.h"
#include "DiscreteDivergenceSkeleton.h"
#include <vector>
#include <algorithm>

#include "BasicUtils.h"

using namespace sg;
#include "NeighborhoodArray.cpp"



///////////////////////////////////////////////////////////////
/// Private types
///////////////////////////////////////////////////////////////


struct NodeOrder{
	DiscreteDivergenceSkeletonNode *n;
	double t;

	NodeOrder(DiscreteDivergenceSkeletonNode *node, double ct){
		n = node; t = ct;
	}

	bool operator<(const NodeOrder &no1) const{
		return t < no1.t;
	}
};


//! BEGIN: Matthijs 14-04-2006
struct SpecialNeighbors
{
	const int* m_nbrs;
	int m_i0, m_i1;

	SpecialNeighbors(int* nbrs) :m_nbrs(nbrs) { m_i0 = -1; m_i1 = -1; }

	void SetSwap(int i0, int i1) { m_i0 = i0; m_i1 = i1; }

	const int* operator[](int i) const
	{
		i = (i == m_i0) ? m_i1:((i == m_i1) ? m_i0:i);
		return m_nbrs + i * 2; // same as &m_nbrs[i * 2] == &(*(m_nbrs + i * 2))
	}
};
//! END: Matthijs 14-04-2006


///////////////////////////////////////////////////////////////
/// Private Global Variables
///////////////////////////////////////////////////////////////


// the neighbourhood is
// 8 1 2
// 7 P 3
// 6 5 4
// and these are the offsets
int nbrs[][2] = {{0,1},  {1,1},  {1,0},   // 1 2 3
{1,-1}, {0,-1}, {-1,-1}, // 4 5 6
{-1,0}, {-1,1}           // 7 8
};

SpecialNeighbors specialNbrs((int*)nbrs);
/*int specialNbrs[][2] = {{0,1},  {1,1},  {1,0},   // 1 2 3
{1,-1}, {0,-1}, {-1,-1}, // 4 5 6
{-1,0}, {-1,1}           // 7 8
};*/


///////////////////////////////////////////////////////////////
/// Private functions
///////////////////////////////////////////////////////////////

void thin_div_arr(const DivergenceMap &dm);
void manage_nbr(int x, int y, DivArr &da, std::vector<DivPt> &the_heap);
bool is_removable(int x, int y, DivArr &da);
bool is_removable(DivPt dp, DivArr &da);
bool is_end_pt(int x, int y, DivArr &da);
int get_nbrs_index(int x, int y, DivArr &da);
bool is_on_contour(int x, int y, DivArr &da);
DivArr *create_shape_DivArr(const DivergenceMap &dm, double step=1.0);
int num_of_intersections(int x, int y, DivArr &da);
void colour_skeleton_array(DivArr &da);
void thin_div_arr(DivArr &da, double thresh, double step=1.0);

int assign_contour_seg_to_branches(DiscreteDivergenceSkeleton *sk,
								   NodeOrder no1, NodeOrder no2,
								   DDSEdge *edge, DDSNode *n_from);

///////////////////////////////////////////////////////////////
/// Implementations
///////////////////////////////////////////////////////////////

///////// is_in_contour():
//
// A point is on the contour if one of its 8-nbrs is of BACKGROUND_COL
//
/////////
bool is_on_contour(int x, int y, DivArr &da)
{
	if(da(x,y).col != BACKGROUND_COL){
		for(int i=0; i<8; i++)
			if(da(x+nbrs[i][0], y+nbrs[i][1]).col == BACKGROUND_COL)
				return true;
	}

	return false;
}


///////// get_nbrs_index():
//
// Computes the index of the 256 entry NeighborhoodArray.
// That is, the table of truth values for preservation of 
// topology.
//
/////////
int get_nbrs_index(int x, int y, DivArr &da)
{
	int nForegroundNeighbors = 0;

	if(x < 0 || x >= da.getXSize() || 
		y < 0 || y >= da.getYSize())
		return 0;

	if(x == 0 || x == (da.getXSize() - 1) || 
		y == 0 || y == (da.getYSize() - 1)) /* on the edge of the picture */
		return 0;//xffff;

	// beyond the edges of the picture DivArr will return the default point
	// which has BACKGROUND_COL

	if(da(x - 1, y).col != BACKGROUND_COL) nForegroundNeighbors += 8;
	if(da(x + 1, y).col != BACKGROUND_COL) nForegroundNeighbors += 16;
	if(da(x, y - 1).col != BACKGROUND_COL) nForegroundNeighbors += 2;
	if(da(x, y + 1).col != BACKGROUND_COL) nForegroundNeighbors += 64;

	if(da(x - 1, y + 1).col != BACKGROUND_COL) nForegroundNeighbors += 32;
	if(da(x + 1, y + 1).col != BACKGROUND_COL) nForegroundNeighbors += 128;
	if(da(x - 1, y - 1).col != BACKGROUND_COL) nForegroundNeighbors += 1;
	if(da(x + 1, y - 1).col != BACKGROUND_COL) nForegroundNeighbors += 4;

	return nForegroundNeighbors;
}

///////// num_of_intersections():
//
// computes the number of intersections with the foreground in the
// neighbourhood of (x,y)
//
/////////
int num_of_intersections(int x, int y, DivArr &da)
{
	// we only count the background to foreground transitions
	int num_trans = 0;

	// start with the last point going to the first
	if(da(x+nbrs[7][0], y+nbrs[7][1]).col == BACKGROUND_COL &&     //current
		da(x+nbrs[0][0], y+nbrs[0][1]).col != BACKGROUND_COL)       //next
		num_trans++;
	// then do the rest
	for(int i=0; i< 8-1; i++)
		if(da(x+nbrs[i][0], y+nbrs[i][1]).col == BACKGROUND_COL &&   //current
			da(x+nbrs[i+1][0], y+nbrs[i+1][1]).col != BACKGROUND_COL) //next
			num_trans++;

	return num_trans;
}

//////// num_of_nbrs():
//
// Count the number of 8-nbrs of (x,y) which are not in the
// BACKGROUND_COL
//
///////
int num_of_nbrs(int x, int y, DivArr &da)
{
	int n=0;
	for(int i=0; i<8; i++)
		if(da(x + nbrs[i][0], y + nbrs[i][1]).col != BACKGROUND_COL)
			n++;  

	return n;
}

///////// is_end_pt():
//
// We have an end_pt if it has a single neighbour or
// if it has 2 neighbours and there is a single intersection
//
/////////
bool is_end_pt(int x, int y, DivArr &da, double thresh)
{


	if(da(x,y).val > thresh)
		return false;

	int nbr_count = 0;
	for(int i=0; i < 8; i++)
		if (da(x+nbrs[i][0], y+nbrs[i][1]).col != BACKGROUND_COL)
			nbr_count++;

	if(nbr_count == 1)
		return true;

	int num_trans = num_of_intersections(x,y,da);  

	if((nbr_count == 2) && num_trans == 1)
		return true;
	else
		return false;
}


///////// is_removable():
//
//
//
/////////
bool is_removable(DivPt dp, DivArr &da)
{
	return is_removable(dp.x, dp.y, da);
}

///////// is_removable():
//
// We must not break the skeleton
//
/////////
bool is_removable(int x, int y, DivArr &da)
{

	if(da(x,y).col == BACKGROUND_COL)
		return true;

	// NeighborhoodArray is the precomputed table for the 8-nbrs
	if (NeighborhoodArray[get_nbrs_index(x,y,da)] == 1)
		return true;
	else 
		return false;
}


///////// manage_nbr():
//
// Decide wheather to put the given point into the heap or not, and do
// it.  
//
/////////
void manage_nbr(int x, int y, DivArr &da, std::vector<DivPt> &the_heap)
{
	if(x>0 && x<da.getXSize()&& // within bounds
		y>0 && y<da.getYSize()) {

			DivPt dp = da(x, y);
			if(dp.col == FOREGROUND_COL && is_removable(x,y, da)) {

				// enter it in the heap
				the_heap.push_back(da(x,y)); 
				std::push_heap(the_heap.begin(), the_heap.end());
			}
	}

}

///////// thin_div_arr():
//
// Thin the discrete sampling of the plane (divergence array) to
// obtain an approximation of the skeleton. The process begins by
// putting all contour points (that is, contour in the array) in a
// heap where the key is the divergence value. It is assumed that the
// lower (more negative) the value the more likely it is for the point
// to be on the skeleton. Hence, we start removing (i.e. blanking)
// points on the divergence array with highest divergence value; that
// it, on the top of the heap. It may be that the point cannot be
// removed. This occurs only if the point satisfies the criterion for
// being an end-point or if removig the point would disconnect the
// skeleton. An end point is defined as one that has divergence value
// below the threshold thresh and has eihter a single neighbour
// (non-blank, that is) or it has exactly two neighbours and its
// 8-neighbourhood has exactly two connected regions -- blank and
// non-blank. Removing a point disconnects the skeleton if the graph
// of non-blank 8-neighbours of it do not form a tree.
//
// If a point is "removable," we push its removable neighbours into
// the heap. Thus, the algorithm goes around the shape peeling layers
// according to the divergence values. Its running time is
// less than O(n log n) where n is the number of pixels in the
// shape. (In fact, it is O(n log m) where m is the maximal size of
// the heap, which, of course, is m<n.)
//
/////////
void thin_div_arr(DivArr &da, double thresh, double step)
{


	//std::cerr << "preparing heap ... ";
	std::vector<DivPt> the_heap(2*da.getXSize() + 2*da.getYSize());

	//    for(int x=0; x<da.getXSize(); x++){
	//      the_heap.push_back(da(x,0));
	//      the_heap.push_back(da(x,da.getYSize()-1));
	//    }
	//    for(int y=1; y<da.getYSize()-1; y++){
	//      the_heap.push_back(da(0,y));
	//      the_heap.push_back(da(da.getXSize()-1,y));
	//    }

	for(int y=0; y < da.getYSize(); y++)
		for(int x=0; x < da.getXSize(); x++)
			if(is_on_contour(x,y,da))
				the_heap.push_back(da(x,y));



	std::make_heap(the_heap.begin(), the_heap.end());
	//std::cerr << "done\n";

	while(the_heap.size()>0)
	{
		// get point from heap
		DivPt dp = the_heap.front();
		std::pop_heap(the_heap.begin(), the_heap.end());
		the_heap.pop_back();

		// decide wheather to blank it or not
		if(is_end_pt(dp.x, dp.y, da, thresh)) // an end_point anchors the skeleton
			da(dp.x, dp.y).col=SKELETON_COL;
		else
			if(is_removable(dp, da)){

				// manage only not previously erased points
				bool manage = (da(dp.x, dp.y).col != BACKGROUND_COL);

				da(dp.x, dp.y).col=BACKGROUND_COL;

				// add neighbours to the heap if appropriate
				// 8 1 2
				// 7 P 3
				// 6 5 4
				if(manage){
					manage_nbr(dp.x, dp.y+1,   da, the_heap);    // 1
					manage_nbr(dp.x+1, dp.y+1, da, the_heap);  // 2
					manage_nbr(dp.x+1, dp.y,   da, the_heap);    // 3
					manage_nbr(dp.x+1, dp.y-1, da, the_heap);  // 4
					manage_nbr(dp.x, dp.y-1,   da, the_heap);    // 5
					manage_nbr(dp.x-1, dp.y-1, da, the_heap);  // 6
					manage_nbr(dp.x-1, dp.y,   da, the_heap);    // 7
					manage_nbr(dp.x-1, dp.y+1, da, the_heap);  // 8
				}
			}
	}

#ifdef DEBUG_DIVERGENCE_SKELETON_MAKER
	if(0)  {
		double scale = 20;
		int maxval = 255;
		std::cout << "P3\n" << da.getXSize() << " " << da.getYSize() << "\n";
		std::cout << maxval << "\n";

		for(int y=0; y<da.getYSize(); y++)
			for(int x=0; x<da.getXSize(); x++){
				int v = (int)(da(x,y).val*scale);

				//std::cerr << da(x,y).col << "\n";

				if(da(x,y).col != BACKGROUND_COL)
					if(v<0)
						std::cout << -v <<" "<< -v<<" 0 ";
					else
						std::cout << " 0 0 " << v << " ";
				else
					std::cout << " 100 100 100 ";
			}
	}
#endif
}


////////////
// creates the discrete array that will be used for the thinning
DivArr *create_shape_DivArr(const DivergenceMap &dm, 
							double step)
{
	double x, y;
	double bdry = 2;
	int xs=0;

	std::vector<DivPt> pts;
	//std::cerr << "computing DivArr ... ";

	DistanceTransform *dt = dm.getDT();


	double xmin,xmax, ymin,ymax;

	dm.getShapeBounds(&xmin,&xmax, &ymin,&ymax);

	for(y=(ymin-bdry); y < (ymax+bdry); y+=step)
	{
		for(x=(xmin-bdry), xs=0; x <  (xmax+bdry); x+=step, xs++)
		{
			DivPt dp;
			double dist = dt->getValue(x,y);
			// blank all points outside the shape

			if(dist > 0){
				dp = DivPt(Point(x,y), 10000);
				dp.col = BACKGROUND_COL;
				dp.dist = dist;
			}
			else{ // only points inside the shape are considered
				dp = DivPt(Point(x,y), dm.getValue(x,y,step));
				dp.dist = dist;
			}

			pts.push_back(dp);
		}
	}

	delete dt;

	// now create the array
	DivArr *da = new DivArr(pts, xs);

	return da;
}

///////////
// colour all points into one of end_pt|branching_pt|skeleton_pt
void DivergenceSkeletonMaker::colour_skeleton_array(DivArr &da)
{
	for (int y=0; y < da.getYSize(); y++)
	{
		for(int x=0; x < da.getXSize(); x++) 
		{
			if (da(x,y).col != BACKGROUND_COL) 
			{
				int nints = num_of_intersections(x, y, da);

				//        std::cerr << nints << " " ;

				//        if(nints == 1 ){
				//  	std::cerr << "\n";

				//  	std::cerr << da(x-1,y+1).col << " " << da(x,y+1).col << " ";
				//  	std::cerr << da(x+1,y+1).col << " "<< "\n";

				//  	std::cerr << da(x-1,y).col << " " << da(x,y).col<< " " << da(x+1,y).col << "\n";

				//  	std::cerr << da(x-1,y-1).col << " " << da(x,y-1).col << " ";
				//  	std::cerr << da(x+1,y-1).col << " "<< "\n";
				//        }

				if(nints == 1)
					da(x,y).col = END_POINT_COL;
				else if(nints == 2)
					da(x,y).col = SK_POINT_COL;
				else
					da(x,y).col = BRANCHING_POINT_COL;
			}
		}
	}

	// testing
#ifdef DEBUG_DIVERGENCE_SKELETON_MAKER
	if(0)
	{
		double scale = 20;
		int maxval = 255;
		std::cout << "P3\n" << da.getXSize() << " " << da.getYSize() << "\n";
		std::cout << maxval << "\n";

		for(int y=0; y<da.getYSize(); y++)
			for(int x=0; x<da.getXSize(); x++){
				int v = (int)(da(x,y).val*scale);

				switch(da(x,y).col){

case END_POINT_COL:
	std::cout << " 0 250 0 ";
	break;

case SK_POINT_COL:
	std::cout << -v <<" "<< -v<<" 0 ";
	break;

case BRANCHING_POINT_COL:
	std::cout << " 250 0 0 ";
	break;

default:
	std::cout << " 100 100 100 ";
	break;
				}


			}
	}
#endif

}

////// create_branch():
//
// Recursive. 
//
// Traverses the thinned divergence array and creates the
// skeleton-braches. Pass an empty vector, to indicate the beginning
// of the traversal. The array is assumed to be properly initialized;
// that is, at the beginning all skeletal points are not da().visited.
//
// This assumes that each strictly skeletal point (i.e. not a branching
// point and not an end-point) has at most one neighbour which is a
// branching point or an end-point. Also, there must be more than a
// single skeletal point for this function to work.
//
// Here is the algorithm. The function creates branches
// recursively. It needs an anchor point and a direction point. It
// starts creating the branch from the anchor point toward the
// direction point. If it reaches an end point, it finishes the
// current branch and returns. If it reaches a branching point, it
// finishes the current branch, looks at branching-point's
// neioghbourhood and finds all unvisited nbrs. It marks them as
// visited and calls itself on each pair (branch-pt, nbr). The
// blanking of the nbrs is important since it is possible that some
// nbrs of a point are also nbrs among them; hence, marking those as
// visited prevents a branch from flowing into another branch.
//
// This algorithm does not care about the one degenerate case of the
// divergence thinning -- the creation of a square of branching
// points. In such a case, it would simply enter the square from one
// of the branching points creating branches from it to the other
// three. After that, the branch creation flows away from the square
// departing from each of the other three branching points. If so desired,
// one could post-process the output of this function to collapse such
// squares into a single branching point...
//
////// 
void create_branch(int x, int y, int nextx, int nexty,
				   std::vector<DivPtVector*> &branches, 
				   DivArr &da)
{

	DivPtVector *dpv = new DivPtVector;

	// minimal error-check
	if(da(x,y).col == BACKGROUND_COL){

		std::cerr << "error: create_branch() in DivergenceSkeletonMaker\n";
		std::cerr << "error: if(da(x,y).col == BACKGROUND_COL)\n";

		return;   // something is very wrong if this happens
	}

	// insert the first point into the branch
	dpv->push_back(da(x,y));

	bool found_nbr = true;

	if(x!=nextx || y!=nexty){
		da(nextx, nexty).visited = true;
		dpv->push_back(da(nextx,nexty));

		if(da(nextx,nexty).col == END_POINT_COL){
			found_nbr = false;
			branches.push_back(dpv); // insert new branch
		}

		if(da(nextx,nexty).col == BRANCHING_POINT_COL) {
			found_nbr = false; // a short branch -- don't look for a new nbr
			branches.push_back(dpv); // insert new branch

			int nx = nextx, ny = nexty;
			int i;

			//! BEGIN: Matthijs 14-04-2006
			//we should not claim a branch as ours, if one of our
			//nbrs is a branching point and the distance
			//from that branching point to the point we want
			//to claim is smaller
			//so lets first check whether we have a branching
			//point as nbr
			bool bBrPntAsNbr = false;
			int nPosX = 0, nPosY = 0;
			for(i = 0; i < 8; i++)
			{
				int vx = nx + nbrs[i][0];
				int vy = ny + nbrs[i][1];
				if(da(vx,vy).col == BRANCHING_POINT_COL)
				{
					if((abs(nx-vx) + abs(ny-vy)) == 1)
					{
						nPosX = vx;
						nPosY = vy;
						bBrPntAsNbr = true;
					}
				}
			}
			//! END: Matthijs 14-04-2006

			// first, find the appropriate neighbours and claim them
			// for different branches by marking them visited
			int num_nbrs = 0;  // number of nbrs in nextpts
			int nextpts[8][2]; // there are never 8 or more

			for(i=0; i<8; i++)
			{	
				int vx = nx + nbrs[i][0];
				int vy = ny + nbrs[i][1];

				if((da(vx,vy).visited == false && // only go to new places
					da(vx,vy).col != BACKGROUND_COL)){ // foreground
						// point 

						//! BEGIN: Matthijs 14-04-2006
						//skip a skelpointnbr if we have a branching point as nbr
						//which is closer to this skelpointnbr
						if(bBrPntAsNbr && 
							( (abs(nPosX-vx) + abs(nPosY-vy)) < 
							(abs(nx-vx) + abs(ny-vy)) ) &&
							da(vx,vy).col == SK_POINT_COL)
						{
							continue;
						}
						//! END: Matthijs 14-04-2006

						da(vx,vy).visited = true; // so that a nearby branch
						// does not get confused 

						nextpts[num_nbrs][0] = vx;
						nextpts[num_nbrs][1] = vy;
						num_nbrs++;		
				}
			}

			// then, recursively build the branches supplying as next
			// points the neighbours found above
			for(i=0; i<num_nbrs; i++){
				create_branch(nx, ny, 
					nextpts[i][0], // next x
					nextpts[i][1], // next y
					branches, da); 
			}
		}
	}  //endif(x!=nextx || y!=nexty)

	int xprev = x, yprev = y;
	x = nextx; y = nexty;

	while(found_nbr)
	{
		//! BEGIN: Matthijs 14-04-2006
		bool bSPECIALCASE = false;
		//! END: Matthijs 14-04-2006

		found_nbr = false;

		da(x,y).visited = true;

		int nx = -1;
		int ny = -1;
		int i;

		//! BEGIN: Matthijs 14-04-2006
		bool bBranchNbr = false;
		bool bTwoBranchNbr = false;
		bool bEndNbr = false;
		int nEndPos = -1, nBranch1Pos = -1, nBranch2Pos = -1;
		for(i = 0; i < 8; i++)
		{
			int vx = x + nbrs[i][0];
			int vy = y + nbrs[i][1];
			if(da(vx,vy).col == END_POINT_COL &&
				!(xprev == vx && yprev == vy))
			{
				bEndNbr = true;
				nEndPos = i;
			}
			if(da(vx,vy).col == BRANCHING_POINT_COL &&
				!(xprev == vx && yprev == vy))
			{
				//if first branch point
				if(!bBranchNbr)
				{
					bBranchNbr = true;
					nBranch1Pos = i;
				}
				if(bBranchNbr && ( abs(nbrs[nBranch1Pos][0]) +
					abs(nbrs[nBranch1Pos][1]) ) > 
					( abs(nbrs[i][0]) +
					abs(nbrs[i][1]) ) )
				{
					bTwoBranchNbr = true;
					nBranch2Pos = nBranch1Pos;
					nBranch1Pos = i; 
				}
			}

			if(bTwoBranchNbr && (nBranch1Pos > nBranch2Pos))
			{
				specialNbrs.SetSwap(nBranch1Pos, nBranch2Pos);
				bSPECIALCASE = true;
			}
			else if(bBranchNbr && bEndNbr && (nBranch1Pos > nEndPos))
			{
				specialNbrs.SetSwap(nBranch1Pos, nEndPos);
				bSPECIALCASE = true;
			}
			else if(bTwoBranchNbr && bEndNbr)
			{
				DBG_PRINT1("\nLAST SPECIAL CASE!!! FIX IT!!\n")
			}
		}
		//! END: Matthijs 14-04-2006*/

		for(i=0; i < 8 && (found_nbr==false); i++){ // go around the nbhd

			//! BEGIN: Matthijs 14-04-2006
			int tx = 0, ty = 0;
			if(!bSPECIALCASE)
			{
				//! END: Matthijs 14-04-2006
				tx = x + nbrs[i][0];
				ty = y + nbrs[i][1];
				//! BEGIN: Matthijs 14-04-2006
			}
			else
			{
				tx = x + specialNbrs[i][0];
				ty = y + specialNbrs[i][1];
			}
			//! END: Matthijs 14-04-2006  

			found_nbr = false;

			// if destination is a foreground point not the previous one
			if (da(tx,ty).col != BACKGROUND_COL &&
				!(xprev==tx && yprev==ty)){// must not be coming from destination


					if(da(tx,ty).visited == false){

						if(da(tx,ty).col == SK_POINT_COL){
							// prefer the,closer, non-diagonal points -- this is
							// actually necessary 
							nx = tx;
							ny = ty;

						}

						if(da(tx,ty).col == END_POINT_COL){
							nx = tx;
							ny = ty;
							found_nbr = true;
						}
					} // endif (tx,ty) not visited

					// We prefer a branching-point to a mere skeletal pt when coming
					// from a skeletal point. This is to prevent from flowing into a
					// neighbouring branch at a branching point.

					if(!(tx==x && ty==y))
						if(da(tx,ty).col == BRANCHING_POINT_COL)
						{
							if(da(xprev, yprev).col != BRANCHING_POINT_COL ||
								SQR(tx-xprev) > 1 || SQR(ty-yprev) > 1) // we do not want to go
								// to a branching point which is next to our starting
								// branching point. if we do, then the branch will not be
								// traced completely
							{
								nx = tx;
								ny = ty;
								found_nbr = true;
							}
						}
			} //endif foreground dest not same as previous
		} // end of for loop -- nbrs traversal

		if(nx<0){
			found_nbr = false;
			std::cerr << "\n(" << dpv->size() << "):["<<x<<","<<y<<"] ";
			std::cerr << "did not find a nbr\n";

			DivPt dp;

			dp = da(x-1,y+1); std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x,y+1);   std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x+1,y+1); std::cerr << dp.col <<"("<<dp.visited<<")\n";

			dp = da(x-1,y); std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x,y);   std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x+1,y); std::cerr << dp.col <<"("<<dp.visited<<")\n";

			dp = da(x-1,y-1); std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x,y-1);   std::cerr << dp.col <<"("<<dp.visited<<")  ";
			dp = da(x+1,y-1); std::cerr << dp.col <<"("<<dp.visited<<")\n";

		}
		else{


			bool bBrPntAsNbr2 = false;
			int nPosX2 = 0, nPosY2 = 0;

			switch(da(nx,ny).col){

case END_POINT_COL:
	if(da(nx,ny).visited == false){ // since we may start at an end-point
		da(nx,ny).visited = true;  // update visited status
		dpv->push_back(da(nx,ny));
		branches.push_back(dpv);   // insert new branch
		return;                    // nothing else left to do
	} 
	found_nbr = false;
	break;

case BRANCHING_POINT_COL:

	//! BEGIN: Matthijs 14-04-2006
	//we should not claim a branch as ours, if one of our
	//nbrs is a branching point and the distance
	//from that branching point to the point we want
	//to claim is smaller
	//so lets first check whether we have a branching
	//point as nbr
	for(i = 0; i < 8; i++)
	{
		int vx = nx + nbrs[i][0];
		int vy = ny + nbrs[i][1];
		if(da(vx,vy).col == BRANCHING_POINT_COL && da(nx,ny).col == BRANCHING_POINT_COL)
		{
			if((abs(nx-vx) + abs(ny-vy)) == 1)
			{
				nPosX2 = vx;
				nPosY2 = vy;
				bBrPntAsNbr2 = true;
			}
		}
	}
	//! END: Matthijs 14-04-2006 

	if(xprev != nx || yprev != ny){ // we're not coming from
		// this BRANCHING_POINT

		da(nx,ny).visited = true;  // update visited status
		dpv->push_back(da(nx,ny));
		branches.push_back(dpv); // insert new branch

		// first, find the appropriate neighbours and claim them
		// for different branches by marking them visited
		int num_nbrs = 0;  // number of nbrs in nextpts
		int nextpts[8][2]; // there are never 8 or more

		for(i=0; i<8; i++){

			int vx = nx + nbrs[i][0];
			int vy = ny + nbrs[i][1];


			if((da(vx,vy).visited == false && // only go to new places
				da(vx,vy).col != BACKGROUND_COL)){ // foreground
					// point 

					//! BEGIN: Matthijs 14-04-2006
					if(bBrPntAsNbr2 && 
						( (abs(nPosX2-vx) + abs(nPosY2-vy)) < 
						(abs(nx-vx) + abs(ny-vy)) ) &&
						da(vx,vy).col == SK_POINT_COL)
					{
						continue;
					}
					//! END: Matthijs 14-04-2006 

					da(vx,vy).visited = true; // so that a nearby branch
					// does not get confused 
					nextpts[num_nbrs][0] = vx;
					nextpts[num_nbrs][1] = vy;
					num_nbrs++;		
			}
		}

		// then, recursively build the branches supplying as next
		// points the neighbours found above
		for(i=0; i<num_nbrs; i++){
			create_branch(nx, ny, 
				nextpts[i][0], // next x
				nextpts[i][1], // next y
				branches, da); 
		}

		return; // do no more after the branching is over
	}

	std::cerr << "\ngot to a false branching point\n";
	found_nbr = false;

	break;

case SK_POINT_COL: 

	if(da(nx,ny).visited == false){
		dpv->push_back(da(nx,ny)); // insert in branch
		da(nx,ny).visited = true;  // update visited status
		xprev = x; yprev = y;
		x = nx; y = ny;            // go to the next point
		found_nbr = true;          // continue
	}
	break;

default:
	found_nbr = false;
	break;
			} // end of switch
		}
	} // end of while loop

	if(dpv->size() <= 1) // we did not create a real branch only
		delete dpv;        // the first point was put in and there
	//  __cannot__ be 1-point branches 

}

//////// destroy_preliminary_branch_list():
//
// Goes through the list of branches and deletes them. They are
// pointers, that's why need this...
// 
////////
void destroy_preliminary_branch_list(std::vector<DivPtVector*> &branches)
{

	//std::cerr << "\ndestroying ... [ ";
	//int i=0;
	while(branches.size() > 0){
		//std::cerr << ++i <<" ";
		DivPtVector *dpv;
		dpv = branches.back();
		delete dpv;
		branches.pop_back();
		//std::cerr << "("<<dpv->size() << ") ";
	}
	//  std::cerr << " ]\n";
}



///////// assign_contour_segments(skeleton):
//
//
//
/////////
int assign_contour_segments(DiscreteDivergenceSkeleton *skeleton)
{

	Curve *curve = skeleton->getShape()->getCurves()->front();


	std::vector<NodeOrder> end_pt_list;

	std::vector<DiscreteDivergenceSkeletonNode*> &nodes = skeleton->getNodes();
	std::vector<DiscreteDivergenceSkeletonNode*>::iterator I;

	for(I=nodes.begin(); I!=nodes.end(); I++){
		DiscreteDivergenceSkeletonNode *n = *I;

		if(n->getCol() == END_POINT_COL){

			NodeOrder no(n, curve->computeDistance(n->getPoint()).t);
			end_pt_list.push_back(no);

			//std::cerr << "no.t: " << no.t << "\n";
		}
	}


	//std::cerr << "#end-pts: " << end_pt_list.size() << "\n";

	//std::cerr << "curveLendgth: " << curve->getLength() << "\n";
	//DBG_MSG1("Number of elements in the NodeOrder list: ",end_pt_list.size());
	std::sort(end_pt_list.begin(), end_pt_list.end());

	std::vector<NodeOrder>::iterator II;

	// do all gaps except between the last and first element
	for(II=end_pt_list.begin(); II!=(end_pt_list.end() - 1); II++){
		NodeOrder no1 = *II;
		NodeOrder no2 = *(II+1);

		assign_contour_seg_to_branches(skeleton, no1, no2, 0, 0); 
	}

	// take care of the last segment
	NodeOrder no1 = end_pt_list.back();
	NodeOrder no2 = end_pt_list.front();
	assign_contour_seg_to_branches(skeleton, no1, no2, 0, 0);


	/// test
	//    DDSEdgeVect::iterator edgeI;
	//    DDSEdgeVect &edges = skeleton->getEdges();

	//    for(edgeI = edges.begin(); edgeI != edges.end(); edgeI++){
	//      DDSEdge *e = *edgeI;

	//      Point ep1 = e->getN1()->getPoint();
	//      Point ep2 = e->getN2()->getPoint();

	//      std::cerr << ep1 << " to " << ep2 << "\n";;

	//      Curve *c = skeleton->getShape()->getCurves()->front();
	//      e->print_left_right_seg(c);

	//      std::cerr << "\n";

	//    }


	return 0;
}

///////// assign_contour_seg_to_branches():
//
// This only works for skeletons which are trees ! The algorithm makes
// use of the fact that there is a unique path between any two
// end-points. It is a recursive algorithm that, given the two nodes
// (the end-points), finds the path (sequence of edges) and assigns
// the contour segment to all edges along it.
//
// Start the algorithm by calling with edge=0 and n_from=0.
//
/////////
int assign_contour_seg_to_branches(DiscreteDivergenceSkeleton *sk,
								   NodeOrder no1, NodeOrder no2,
								   DDSEdge *edge,
								   DDSNode *n_from)
{

	DDSEdgeVect::iterator edgeI;

	// start the algorithm if needed
	if(n_from == 0){
		DDSEdgeVect &edges = no1.n->getEdges();


		for(edgeI = edges.begin(); edgeI != edges.end(); edgeI++){
			DDSEdge *e = *edgeI;
			if(assign_contour_seg_to_branches(sk, no1, no2, e, no1.n)
				== 1)

				if(e->hasRight())
					e->setLeftSegment(no1.t, no2.t);
				else
					e->setRightSegment(no1.t, no2.t);

			return 1;
		}

		return 0;
	}

	// the algorithm starts here
	DDSNode *n_to;
	if(edge->getN1() == n_from)
		n_to = edge->getN2();
	else
		n_to = edge->getN1();


	if(n_to == no1.n)
		return 0; // since we want to reach no2.n, not come back!

	if (n_to->getCol() == END_POINT_COL)
		if (n_to != no2.n)
			return 0;
		else{ // n_to == no2.n
			return 1; // good, found it
		}

		// otherwise n_to is a junction-point
		DDSEdgeVect &edges = n_to->getEdges();
		//DBG_MSG1("---------- Number of elements in the edges list: ",edges.size());
		for(edgeI = edges.begin(); edgeI != edges.end(); edgeI++){
			DDSEdge *e = *edgeI;

			if(e != edge)
				if(assign_contour_seg_to_branches(sk, no1, no2, e, n_to)==1){

					// assign the segment to the edge if the edge is on the right path
					if(e->hasRight()) 
						e->setLeftSegment(no1.t, no2.t);
					else
						e->setRightSegment(no1.t, no2.t);

					return 1;
				}
		}

		return 0;


}


///////// create_skeleton():
//
// Makes the skeleton.
//
/////////
DiscreteDivergenceSkeleton* sg::create_skeleton(DivArr &da, DivergenceMap* dm, 
												DiscreteDivergenceSkeleton* skeleton)
{

	int i=0, xs, ys, x=0, y=0;
	xs = da.getXSize();
	ys = da.getYSize();

	// find the initial branch-end-point
	while(i < xs*ys &&
		!(da(x,y).col == END_POINT_COL)){
			x = i%xs;
			y = i/xs;
			i++;
	}

	// now, create a preliminary branch list
	std::vector<DivPtVector*> branches; // <- here

	da(x,y).visited = true;
	//std::cerr << "got " << branches.size() << " branches\n";


	// find the next point
	int vx, vy;
	for(i=0; i<8; i++){
		vx = x + nbrs[i][0];
		vy = y + nbrs[i][1];

		if(da(vx,vy).col != BACKGROUND_COL)
			break;
	}

	// make the branches
	create_branch(x, y, vx, vy,branches, da);

	// now, make the DiscreteDivergenceSkeleton
	//std::cerr << "making DiscreteDivergenceSkeleton\n";
	//  DBG_MSG1("Number of branches in the graph: ",branches.size());

	std::vector<DiscreteDivergenceSkeletonNode *> nodes;
	// begin by creating the nodes
	std::vector<DivPtVector*>::iterator I;
	for(I=branches.begin(); I!=branches.end(); I++){
		DivPtVector* dpv = *I;

		// try the first branch-end-point
		DivPt dp = dpv->front();
		bool dp_is_new = true;

		std::vector<DiscreteDivergenceSkeletonNode *>::iterator NodeI;
		if(nodes.size() > 0)
			for(NodeI = nodes.begin(); NodeI!=nodes.end(); NodeI++){
				DiscreteDivergenceSkeletonNode *n = *NodeI;

				if(n != 0)
					if(dp.p == n->getPoint()){
						dp_is_new = false;   

						if(dpv->n1 == 0)
							dpv->n1 = n;
						else if(dpv->n2 == 0)
							dpv->n2 = n;
						else
							std::cerr << "\n ERROR: More than two nodes per branch !!!\n";

						break;
					}
			}

			if(dp_is_new){
				FluxPoint fp(dp.p, dp.val, dp.dist, dp.col);
				DiscreteDivergenceSkeletonNode *n = new DiscreteDivergenceSkeletonNode(fp);
				nodes.push_back(n);

				if(dpv->n1 == 0)
					dpv->n1 = n;
				else if(dpv->n2 == 0)
					dpv->n2 = n;
				else
					std::cerr << "\n ERROR: More than two nodes per branch !!!\n";
			}

			// and the other branch-end-point
			dp = dpv->back();
			dp_is_new = true;

			if(nodes.size() > 0)

				for(NodeI = nodes.begin(); NodeI!=nodes.end(); NodeI++){
					DiscreteDivergenceSkeletonNode *n = *NodeI;

					if(n != 0){
						if(dp.p == n->getPoint()){
							dp_is_new = false;     

							if(dpv->n1 == 0)
								dpv->n1 = n;
							else if(dpv->n2 == 0)
								dpv->n2 = n;
							else
								std::cerr << "\n ERROR: More than two nodes per branch !!!\n";

							break;
						}
					}
				}

				if(dp_is_new){
					FluxPoint fp(dp.p, dp.val, dp.dist, dp.col);
					DiscreteDivergenceSkeletonNode *n = new DiscreteDivergenceSkeletonNode(fp);
					nodes.push_back(n);

					if(dpv->n1 == 0)
						dpv->n1 = n;
					else if(dpv->n2 == 0)
						dpv->n2 = n;
					else
						std::cerr << "\n ERROR: More than two nodes per branch !!!\n";
				}

	}

	// insert the nodes in the skeleton
	if (skeleton == NULL)
		skeleton = new DiscreteDivergenceSkeleton(dm);


	std::vector<DiscreteDivergenceSkeletonNode *>::iterator NodeI;
	for(NodeI = nodes.begin(); NodeI!=nodes.end(); NodeI++){
		DiscreteDivergenceSkeletonNode *n = *NodeI;

		if(n!=0)
			skeleton->addNode(n);
	}

	// now make and insert the edges
	for(I=branches.begin(); I!=branches.end(); I++){
		DivPtVector* dpv = *I;
		if(dpv->n1 == 0 || dpv->n2 == 0)
			std::cerr << "ERROR: Branch does not have two branch-end-points.\n";

		std::vector<DivPt>::iterator DI;
		FluxPointList fpl;
		for(DI = dpv->begin(); DI != dpv->end(); DI++){
			DivPt dp = *DI;
			fpl.push_back(FluxPoint(dp.p, dp.val, dp.dist, dp.col));

		}

		DiscreteDivergenceSkeletonEdge *e;
		e = new DiscreteDivergenceSkeletonEdge(skeleton, fpl, dpv->n1,
			dpv->n2);
		skeleton->addEdge(e);

	}

	// to complete the skeleton creation, assign left and right contour
	// portions to each branch/edge.
	// assign_contour_segments(skeleton);

	//std::cerr << nodes.size() << "\n";
	// finally, destroy the preliminary list
	destroy_preliminary_branch_list(branches);

	return skeleton;
}


///////// DivergenceSkeletonMaker::make_skeleton():
//
// Collects the algorithm...
//
/////////
void DivergenceSkeletonMaker::make_skeleton(DivergenceMap* dm, 
											double step, double thresh)
{
	DivArr* da =  create_shape_DivArr(*dm, step);

	thin_div_arr(*da, thresh, step);

	colour_skeleton_array(*da);

	create_skeleton(*da, dm);

	delete da;
}

///////// DivergenceSkeletonMaker::getDiscreteDivergenceSkeleton():
//
//
//
/////////
DiscreteDivergenceSkeleton *
DivergenceSkeletonMaker::getDiscreteDivergenceSkeleton(
	DivergenceMap* dm, const double& step, const double& thresh)
{
	DivArr* da = create_shape_DivArr(*dm, step);

	thin_div_arr(*da, thresh, step);

	colour_skeleton_array(*da);

	DiscreteDivergenceSkeleton * skeleton = create_skeleton(*da, dm);

	delete da;

	return skeleton;
}



///////// DivergenceSkeletonMaker::getThinDivArr(double step, double thresh):
//
//
//
/////////
/*DivArr& DivergenceSkeletonMaker::getThinDivArr(double step, double thresh)
{
	DivArr* da = create_shape_DivArr(*dm, step);
	thin_div_arr(*da, thresh, step);
	colour_skeleton_array(*da);
	DiscreteDivergenceSkeleton * skeleton = create_skeleton(*da, *dm);

	skeleton = 0;
	
	return *da; // should delete da somewhere
}*/

///////////////////////////////////////////////////////////////////////////////
// New functions added by Diego Macrini

/*! 
@brief It eliminates the empty branch formed by two connected nodes src and tgt.
It copies the non-empty branches from src to tgt and deletes src.
*/
void DiscreteDivergenceSkeleton::eraseEmptyEdge(DDSNode* src, DDSNode* tgt)
{
	DDSEdgeVect::iterator edgeIt;
	DDSEdge *pEdge, *pEmptyEdge = NULL;

	// Look for the actual edge with given endpoints
	for (edgeIt = src->edges.begin(); edgeIt != src->edges.end(); edgeIt++)
	{
		pEdge = *edgeIt;

		if ((pEdge->n1 == src && pEdge->n2 == tgt) ||
			(pEdge->n2 == src && pEdge->n1 == tgt))
		{
			pEmptyEdge = pEdge;
			break;
		}
	}

	if (!pEmptyEdge || pEmptyEdge->size() > 2)
	{
		std::cerr << "Error in eraseEmptyEdge(): no empty edge" << std::endl;
		return;
	}

	// Update the info of the tgt node and the flux endpoins of each branch in src
	for (edgeIt = src->edges.begin(); edgeIt != src->edges.end(); edgeIt++)
	{
		pEdge = *edgeIt;

		if (pEdge != pEmptyEdge)
		{
			// Update the endpoins of the branch
			if (pEdge->n1 == src)
				pEdge->n1 = tgt;

			if (pEdge->n2 == src)
				pEdge->n2 = tgt;

			// Update the flux points of the branch
			FluxPointList&    fpl = pEdge->getFluxPoints();
			BoundaryInfoList& bil = pEdge->getBoundaryInfoList();

			if (fpl.front().p == src->fp.p)
			{
				if (bil.size() == fpl.size())
					bil.insert(bil.begin(), bil.front()); // just a duplicate

				fpl.insert(fpl.begin(), tgt->fp);
			}

			if (fpl.back().p == src->fp.p)
			{
				if (bil.size() == fpl.size())
					bil.push_back(bil.back()); // just a duplicate

				fpl.push_back(tgt->fp);
			}

			tgt->edges.push_back(pEdge);
		}
	}

	// Remove empty edge from the edge list of tgt
	for (edgeIt = tgt->edges.begin(); edgeIt != tgt->edges.end(); edgeIt++)
	{
		if (*edgeIt == pEmptyEdge)
		{
			tgt->edges.erase(edgeIt);
			break;
		}
	}

	// Remove empty edge from the edge list of the graph
	for (edgeIt = m_edges.begin(); edgeIt != m_edges.end(); edgeIt++)
	{
		if (*edgeIt == pEmptyEdge)
		{
			m_edges.erase(edgeIt);
			break;
		}
	}

	// Remove the src node from the node list of the graph
	DDSNodeVect::iterator nodeIt;

	for (nodeIt = m_nodes.begin(); nodeIt != m_nodes.end(); nodeIt++)
	{
		if (*nodeIt == src)
		{
			m_nodes.erase(nodeIt);
			break;
		}
	}

	// Delete the empty edge
	delete pEmptyEdge;

	// Finally, release the memory of the src node
	delete src;
}

/*!
@brief Adds the boundary gap points to the list of connected boundary points
within a gap. It does not add 'pt0' and only adds pt1 if 'bIncludeLast' is true. 

Use addGapPoint(pt) to add the first point before calling addGapPoints(...).

If the given points are not connected, the points in between them are interpolated 
by a line. The result is always the addition of a list of connected gap points.
*/
void BoundarySegment::addGapPoints(const Point& pt0, const Point& pt1, bool bIncludeLast)
{
	ASSERT_VALID_POINT(pt0);
	ASSERT_VALID_POINT(pt1);

	// If the points are already connected, only add last point if requested
	if (fabs(pt1.x - pt0.x) <= 1 && fabs(pt1.y - pt0.y) <= 1)
	{
		if (bIncludeLast)
			gapPts.push_back(pt1);

		return;
	}

	// Interpolate the given points with a rasterized line, so that the result 
	// is a connected set of points

	// Rasterize a line using Bresenham's line algorithm
	int x0 = round(pt0.x);
	int y0 = round(pt0.y);
	const int x1 = round(pt1.x);
	const int y1 = round(pt1.y);

	int dy = y1 - y0;
	int dx = x1 - x0;
	int stepx, stepy;

	if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
	if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
	dy <<= 1;                                                  // dy is now 2*dy
	dx <<= 1;                                                  // dx is now 2*dx

	// do not add the first pt, because it's already there. 
	// ie, no gapPts.push_back(...) here

	if (dx > dy) {
		int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
		while (x0 != x1) {
			if (fraction >= 0) {
				y0 += stepy;
				fraction -= dx;                                // same as fraction -= 2*dx
			}
			x0 += stepx;
			fraction += dy;                                    // same as fraction -= 2*dy

			if (bIncludeLast || (x0 != x1 || y0 != y1))        // add last pt only if requested
				gapPts.push_back(Point(x0, y0));
		}
	} else {
		int fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) {
				x0 += stepx;
				fraction -= dy;
			}
			y0 += stepy;
			fraction += dx;

			if (bIncludeLast || (x0 != x1 || y0 != y1))
				gapPts.push_back(Point(x0, y0));
		}
	}

	if (bIncludeLast && (gapPts.back().x != x1 || gapPts.back().y != y1))
		gapPts.push_back(Point(x1, y1));
}

/*!
	Normal values for params and recommended values.
	
	resolution >= 4 [16]; 0 < step <= 1 [1]; fluxTau <= -1 [-2.5];
*/
DiscreteDivergenceSkeleton* DivergenceSkeletonMaker::getDiscreteDivergenceSkeleton(
	Shape* pShape, int resolution, const double& step,  const double& thresh)
{
	if (resolution < 4 || step > 1 || thresh > -1)
		return NULL;

	// Create a DistanceTransform object...
	DistanceTransform* pDT = new DistanceTransform(pShape);
	
	// ...and a DivergenceMap...
	DivergenceMap* pDivMap = new DivergenceMap(pDT, resolution);

	return getDiscreteDivergenceSkeleton(pDivMap, step, thresh);
}

