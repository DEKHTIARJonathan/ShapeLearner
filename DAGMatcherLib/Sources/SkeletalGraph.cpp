/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini
 *
 * $Log: SkeletalGraph.cpp,v $
 *
 *-----------------------------------------------------------------------*/

#include <LEDA/core/d_array.h>
#include "SkeletalGraph.h"
#include "BoundaryPointFinder.h"
#include "skeleton.h"  // AFMM skeleton headers
#include "BasicUtils.h"

// VC++6 -> Include CImg before DAG.h
#include "../CImg/CImg.h"
#include "ImageProcessing.h"

using namespace std;
using namespace sg;
using namespace dml;

void SmoothBranchPoints(sg::FluxPointList& pts);

/*!
	@brief Removes the DDSGraph and the BoundaryPointFinder objects
	associated with the skeletal graph.
*/
SkeletalGraph::~SkeletalGraph()
{
	delete m_pDDSGraph;
	delete m_pBndryPtFinder;
}

/*!
	@brief Creates a skeleton graph using either a FluxSkeleton algorithm or
	a Augmmented Fast Marching Method algorithm.
*/
bool SkeletalGraph::Create(const ImageInfo& imgInfo, const SkeletalGraphParams& params,
                           ShapeDims* pDims /*= NULL*/)
{
	delete m_pDDSGraph; // delete any existing graph (if there is one)

	if (params.nSkelAlgorithm == SkeletalGraphParams::FluxAlgorithm)
	{
		if (imgInfo.pField != NULL)
			ShowError("FluxSkeletons can only be computed from an image file");

		return CreateFluxSkeleton(imgInfo.strFileName, params, pDims);
	}
	else if (params.nSkelAlgorithm == SkeletalGraphParams::AFMMAlgorithm)
	{
		return CreateAFMMSkeleton(imgInfo, params, pDims);
	}
	else
	{
		m_pDDSGraph = NULL;
		ShowError("Unknown skeleton algorithm");
		return false;
	}
}

/*!
	@brief Computes the discrete divergence skeleton of a closed curve in a ppm file.
*/
bool SkeletalGraph::CreateFluxSkeleton(const char* szPPMFileName, const SkeletalGraphParams& params,
                                       ShapeDims* pDims /*= NULL*/)
{
	using namespace sg;

	cout << endl << "Reading " << szPPMFileName << "... " << flush;

	cimg_library::CImg<unsigned char> image(szPPMFileName);

	if (image.dimx() == 0 && image.dimy() == 0)
	{
		cerr << "Can't open " << szPPMFileName << endl;
		return NULL;
	}

	sg::SimpleShapeMaker ssm(image.dimx(), image.dimy());

	int x, y;

	// update the SimpleShape
	for (y=0; y < image.dimy(); y++)
		for (x=0; x < image.dimx(); x++)
			ssm(x,y) = (image(x, y) != 0);

	cout << "Computing flux skeleton..." << flush;

	// The shape contour is being made here
	Shape* pShape = ssm.getShape(); // it's deleted by DivergenceSkeletonMaker

	if (pDims)
		pShape->getBounds(&pDims->xmin, &pDims->xmax, &pDims->ymin, &pDims->ymax);

	// Use the shape contour to create a skeleton
	DivergenceSkeletonMaker dsm;

	// Normal values for params: resolution >= 10; fluxStep = 1; fluxTau = 2.5
	m_pDDSGraph = dsm.getDiscreteDivergenceSkeleton(pShape,
		params.fluxResolution, 1.0 / params.fluxStep, -params.fluxTau);

	return m_pDDSGraph != NULL;
}

/*!
	@brief Computes a AFMM Star skeleton from an image in memory
*/
bool SkeletalGraph::CreateAFMMSkeleton(const ImageInfo& imgInfo, const SkeletalGraphParams& params,
                                       ShapeDims* pDims /*= NULL*/)
{
	SkelCompParams afmmParams; // all params are initialized to valid values

	afmmParams.fThresSimp1 = params.afmmTau1;
	afmmParams.fThresSimp2 = params.afmmTau2;
	afmmParams.nRecErrorWeightBnd = params.nBndryRecErrorWeight;
	afmmParams.nRecErrorWeightStr = params.nStrucRecErrorWeight;

	// Instead of passing a NULL pField and setting afmmParams.pInputfile = imgInfo.strFileName,
	// we read the image here so that we can preprocess it and also deal with more image formats.
	if (imgInfo.pField == NULL)
	{
		cimg_library::CImg<float> img(imgInfo.strFileName);

		// If color image, we only keep the "red" chanel
		if (img.dimv() > 1)
			img = img.channel(0);

		// Removes "white holes" and "black satellites" from the image
		//RemoveNonMaxComponents(img);

		afmmParams.pInputField = CopyImageToField(img);
	}
	else
		afmmParams.pInputField = imgInfo.pField;

	AFMMSkeleton* pAFMMSGraph = AFMMSkeleton::MakeSkeleton(afmmParams);

	if(pAFMMSGraph)
	{
		if(params.nSimplifyComplete == 1)
			pAFMMSGraph->Simplify();
		else if(params.nSimplifyExternal == 1)
			pAFMMSGraph->SimplifyExternal();
		else if(params.nSimplifyInternal == 1)
			pAFMMSGraph->SimplifyInternal();

		if (pDims)
			pAFMMSGraph->GetDimensions(&pDims->xmin, &pDims->xmax, &pDims->ymin, &pDims->ymax);
	}

	m_pDDSGraph = pAFMMSGraph;

	return m_pDDSGraph != NULL;
}

/*!
	@brief Ensures that all invariant properties of a skeletal graph are
	satisfied
*/
bool SkeletalGraph::Regularize()
{
	SkelJoint* pJoint;
	SkelBranch* pBranch;

	if (!m_pDDSGraph)
		return false;

	std::vector< std::pair<SkelJoint*, SkelJoint*> > nodePairs;

	forall_joints(pJoint, m_pDDSGraph->getNodes())
	{
		// Search for special joints that are redundant and delete them
		if (pJoint->degree() < 3)
		{
			WARNING1(true, "Joint has less than 3 branches", pJoint->fp);

			SkelBranch* pBranch;
			SkelJoint* pTgtJoint = NULL;
			double minDist = 3;

			forall_branches(pBranch, pJoint->edges)
			{
				if (pBranch->n1 != pJoint &&
					pJoint->fp.p.dist(pBranch->n1->fp.p) < minDist)
				{
					pTgtJoint = pBranch->n1;
					minDist = pJoint->fp.p.dist(pBranch->n1->fp.p);
				}

				if (pBranch->n2 != pJoint &&
					pJoint->fp.p.dist(pBranch->n2->fp.p) < minDist)
				{
					pTgtJoint = pBranch->n2;
					minDist = pJoint->fp.p.dist(pBranch->n2->fp.p);
				}
			}

			if (minDist > 2)
			{
				ShowError("Invalid input. Skeleton is disconnected.");
				return false;
			}

			nodePairs.push_back(make_pair(pJoint, pTgtJoint));
		}
	}

	// Find the most common case of adjacent joints, which shoud in fact be a single joint
	forall_branches(pBranch, m_pDDSGraph->getEdges())
	{
		if (pBranch->size() == 2 && pBranch->n1->degree() == 3 && pBranch->n2->degree() == 3)
		{
			if (pBranch->n1->fp.radius() > pBranch->n2->fp.radius())
			{
				nodePairs.push_back(make_pair(pBranch->n2, pBranch->n1));
			}
			else
			{
				nodePairs.push_back(make_pair(pBranch->n1, pBranch->n2));
			}
		}
	}

	// Remove all empty DDS edges (ie branches) found
	for (unsigned int i = 0; i < nodePairs.size(); i++)
		m_pDDSGraph->eraseEmptyEdge(nodePairs[i].first, nodePairs[i].second);

	// Smooth skeletal branch points
	forall_branches(pBranch, m_pDDSGraph->getEdges())
	{
		sg::FluxPointList& fpl = pBranch->getFluxPoints();
		int zeroRadiusCount = 0;

		// Some terminal points may have zero radius due to subpixel accuracy
		for (unsigned int i = 0; i < fpl.size(); i++)
		{
			if (fpl[i].radius() == 0)
			{
				fpl[i].dist = 0.5; // half a pixel
				zeroRadiusCount++;
			}
		}

		WARNING1(zeroRadiusCount > 1,
			"There are too many points with zero radius", zeroRadiusCount);

		SmoothBranchPoints(pBranch->getFluxPoints());
	}

	return true;
}

/*!
	@brief Reads the nodes and edges of a skeleton graph from a binary file.
*/
void SkeletalGraph::ReadNodesAndEdges(std::istream& is, DDSGraph& dds)
{
	DDSNode* pNode;
	FluxPoint fp;
	BoundaryInfo bi;
	leda_d_array<long, DDSNode*> map;
	int i, j, size, fp_count, bi_count;
	//double lt1, lt2, rt1, rt2;
	void *key, *keyN1, *keyN2;

	// Read the size of the graph (ie, the number of nodes)
	is.read((char*) &size, sizeof(size));

	// Read the ID and FluxPoint attribute of each node
	for (i = 0; i < size; i++)
	{
		is.read((char*) &key, sizeof(key)); // get the node ID
		is.read((char*) &fp, sizeof(fp));   // get the FluxPoint attribute

		pNode = new DDSNode(fp);  // create a new node

		map[(long)key] = pNode;   // map the ID the node pointer
		dds.addNode(pNode);       // add the node pointer to the graph attributes
	}

	// Read the number of edges in the graph
	is.read((char*) &size, sizeof(size));

	for (i = 0; i < size; i++)
	{
		is.read((char*) &keyN1, sizeof(keyN1));
		is.read((char*) &keyN2, sizeof(keyN2));

		/*is.read((char*) &lt1, sizeof(lt1));
		is.read((char*) &lt2, sizeof(lt2));
		is.read((char*) &rt1, sizeof(rt1));
		is.read((char*) &rt2, sizeof(rt2));*/

		// Read the size of the flux point list
		is.read((char*) &fp_count, sizeof(fp_count));
		FluxPointList fpl;
		fpl.reserve(fp_count);

		// Read each flux point
		for (j = 0; j < fp_count; j++)
		{
			is.read((char*) &fp, sizeof(fp));
			fpl.push_back(fp);
		}

		// Read the size of boundary info list
		is.read((char*) &bi_count, sizeof(bi_count));
		BoundaryInfoList bil;
		bil.reserve(bi_count);

		// Read the boundary info associated with every flux point
		for (j = 0; j < bi_count; j++)
		{
			is.read((char*) &bi, sizeof(bi));
			bil.push_back(bi);
		}

		// Create a new edge object with pointers to its pair of terminal nodes
		DDSEdge* e = new DDSEdge(&dds, fpl, map[(long)keyN1], map[(long)keyN2]);

		if (bi_count > 0)
			e->setBoundaryInfoList(bil);

		//if (lt1 != -1 || lt2 != -1) e->setLeftSegment(lt1, lt2);
		//if (rt1 != -1 || rt2 != -1) e->setRightSegment(rt1, rt2);

		dds.addEdge(e);
	}
}

/*!
	@brief Writes the nodes and edges of a skeleton graph into a binary file.
*/
void SkeletalGraph::WriteNodesAndEdges(std::ostream& os, DDSGraph& dds)
{
	DDSNodeVect::iterator I;
	DDSEdgeVect::iterator II;
	FluxPointList::iterator III;
	BoundaryInfoList::iterator IV;

	DDSNodeVect& nodes = dds.getNodes();
	DDSEdgeVect& edges = dds.getEdges();

	DDSNode *pNode, *n1, *n2;
	DDSEdge *e;
	FluxPoint fp;
	BoundaryInfo bi;

	// Write the size of the graph (ie, the number of nodes)
	int size = nodes.size();
	os.write((char*) &size, sizeof(size));

	// Assign an ID to each node and write it along with
	// the FluxPoint attribute of the node. The ID of the
	// node is defined by the pointer to it stored in the node array.
	for(I = nodes.begin(); I != nodes.end(); I++)
	{
        pNode = *I; // get the pointer to the node and use it as its ID
		os.write((char*) &pNode, sizeof(pNode));
		os.write((char*) &pNode->fp, sizeof(pNode->fp));
	}

	// Write the number of edges in the graph
	size = edges.size();
	os.write((char*) &size, sizeof(size));

	for(II = edges.begin(); II != edges.end(); II++)
	{
		e = *II;
		n1 = e->getN1();
		n2 = e->getN2();
		os.write((char*) &n1, sizeof(n1));
		os.write((char*) &n2, sizeof(n2));
		/*os.write((char*) &e->left_t1, sizeof(e->left_t1));
		os.write((char*) &e->left_t2, sizeof(e->left_t2));
		os.write((char*) &e->right_t1, sizeof(e->right_t1));
		os.write((char*) &e->right_t2, sizeof(e->right_t2));*/

		FluxPointList& fpl = e->getFluxPoints();

		size = fpl.size();
		os.write((char*) &size, sizeof(size));

		for(III = fpl.begin(); III != fpl.end(); III++)
		{
			fp = *III;
			os.write((char*) &fp, sizeof(fp));
		}

		// NEW: save the boundary info associated with every flux point
		BoundaryInfoList& bil = e->getBoundaryInfoList();

		size = bil.size();
		os.write((char*) &size, sizeof(size));

		for(IV = bil.begin(); IV != bil.end(); IV++)
		{
			bi = *IV;
			os.write((char*) &bi, sizeof(bi));
		}
	}
}

/*!
	Reads the member variables of a DDSGraph and then calls
	ReadNodesAndEdges() in order to read the attributes of
	all nodes and edges of the graph.

	It also reads the m_lineSegments and m_bezierSegments
	members of the SkeletalGraph.
*/
DDSGraph* SkeletalGraph::ReadDDSGraph(std::istream& is)
{
	LineSeg* pLineSeg;
	bool closed;
	double xmin, xmax, ymin, ymax;
	int size;
	double startLength;
	Point startPt, endPt;

	//Read Contour
	is.read((char*) &xmin, sizeof(xmin));
	is.read((char*) &xmax, sizeof(xmax));
	is.read((char*) &ymin, sizeof(ymin));
	is.read((char*) &ymax, sizeof(ymax));

	std::vector<CurveSeg*> segments;
	is.read((char*) &size, sizeof(size));

    for(int i = 0; i < size; i++)
	{
		is.read((char*) &startLength, sizeof(startLength));
		is.read((char*) &startPt, sizeof(startPt));
		is.read((char*) &endPt, sizeof(endPt));

		pLineSeg = new LineSeg(startPt, endPt, startLength);
		segments.push_back(pLineSeg);
	}

	is.read((char*) &closed, sizeof(closed));

	SimpleShape* pSimpleShape = new SimpleShape(segments, closed);

	pSimpleShape->setBounds(xmin, xmax, ymin, ymax);

	DDSGraph* dds = new DDSGraph(pSimpleShape);

	// Read all nodes and edges
	ReadNodesAndEdges(is, *dds);

	// Read piecewise skeletion approximation
	m_lineSegments.ReadRecursive(is);
	m_bezierSegments.ReadRecursive(is);

	return dds;
}

/*!
	Writes the member variables of a DDSGraph and then calls
	WriteNodesAndEdges() in order to write the attributes of
	all nodes and edges of the graph.

	It also writes the m_lineSegments and m_bezierSegments
	members of the SkeletalGraph.
*/
void SkeletalGraph::WriteDDSGraph(std::ostream& os, DDSGraph& dds)
{
	SimpleShape* ss = (SimpleShape*)dds.getShape();
	Curve* p = *(ss->getCurves()->begin());
	DiscreteSegCurve* c = dynamic_cast<DiscreteSegCurve*>(p);
	LineSeg* pLineSeg;
	double xmin, xmax, ymin, ymax;
	bool closed;

	ASSERT(c);

	//Read Contour
	ss->getBounds(&xmin, &xmax, &ymin, &ymax);
	os.write((char*) &xmin, sizeof(xmin));
	os.write((char*) &xmax, sizeof(xmax));
	os.write((char*) &ymin, sizeof(ymin));
	os.write((char*) &ymax, sizeof(ymax));

	int size = c->segments.size();
	os.write((char*) &size, sizeof(size));

	std::vector<CurveSeg*>::iterator I;
	for(I = c->segments.begin(); I != c->segments.end(); I++)
	{
		pLineSeg = dynamic_cast<LineSeg*>(*I);
		ASSERT(pLineSeg);

		os.write((char*) &pLineSeg->startLength, sizeof(pLineSeg->startLength));
		os.write((char*) &pLineSeg->startPt, sizeof(pLineSeg->startPt));
		os.write((char*) &pLineSeg->endPt, sizeof(pLineSeg->endPt));
	}

	closed = c->isClosed();
	os.write((char*) &closed, sizeof(closed));

	// Write all nodes and edges
	WriteNodesAndEdges(os, dds);

	// Write piecewise skeletion approximation
	m_lineSegments.WriteRecursive(os);
	m_bezierSegments.WriteRecursive(os);
}

/*!
	Approximates each skeletal branch with a piecewise linear function.
*/
void SkeletalGraph::ApproxSkeletonWithLines(double dMinError, int nMaxSegments)
{
	DDSEdgeVect::iterator edgeIt;
	int i, n;

	ASSERT(m_pDDSGraph);

	DDSEdgeVect& edges = m_pDDSGraph->getEdges();

	m_lineSegments.Resize(edges.size());

	for(edgeIt = edges.begin(), n = 0; edgeIt != edges.end(); edgeIt++, n++)
	{
		FluxPointList& flux_points = (*edgeIt)->getFluxPoints();

		POINTS data(flux_points.size());

		for (i = 0; i < data.GetSize(); i++)
			data[i].Set(flux_points[i].p.x, flux_points[i].p.y);

		PolyLineTLSApprox poly(data.GetSize() / dMinError, nMaxSegments);

		poly.Fit(data);

		// Copy all the line segments
		m_lineSegments[n].Resize(poly.m_knots.GetSize());

		for (int i = 0; i < poly.m_knots.GetSize(); i++)
			m_lineSegments[n][i] = poly.m_knots[i].seg;
	}
}

/*!
	@brief Approximates each skeleton branch with a piecewise cubic Bezier curve.
	It also sets the tangent at each skeleton point.
*/
void SkeletalGraph::ApproxSkeletonWithCubicBeziers(double dMinError, int nMaxSegments)
{
	DDSEdgeVect::iterator edgeIt;
	int i, n, nSize;
	POINT pt;

	ASSERT(m_pDDSGraph);

	DDSEdgeVect& edges = m_pDDSGraph->getEdges();

	m_bezierSegments.Resize(edges.size());

	for(edgeIt = edges.begin(), n = 0; edgeIt != edges.end(); edgeIt++, n++)
	{
		FluxPointList& fpl = (*edgeIt)->getFluxPoints();
		BoundaryInfoList& bil = (*edgeIt)->getBoundaryInfoList();
		nSize = fpl.size();

		POINTS data(nSize);

		for (i = 0; i < nSize; i++)
			data[i].Set(fpl[i].p.x, fpl[i].p.y);

		PolyBezierApprox poly(nSize / dMinError, nMaxSegments);

		poly.Fit(data);

#ifdef USE_EXTRA_BOUNDARY_INFO
		// Set tangent at each skeleton point
		if (bil.size() != nSize)
			bil.resize(nSize);

		for (i = 0; i < nSize; i++)
		{
			pt = poly.GetTangent(i);
			bil[i].tangent = Vector(pt.x, pt.y);
		}
#endif

		// Copy the skeleton approximation so that we can display it later
		m_bezierSegments[n].Resize(poly.m_knots.GetSize());

		for (i = 0; i < poly.m_knots.GetSize(); i++)
			m_bezierSegments[n][i] = poly.m_knots[i].seg;
	}
}

#ifdef USE_EXTRA_BOUNDARY_INFO
/*!
	@brief Set boundary points assuming that one boundary point per
	skeleton points is given (true for the AFMM skeletons).

	Given one boundary point and the tangent at a skeleton point, the
	second boundary point can be found by mirroring the given vector
	from skeleton point to boundary point about the tangent direction.
*/
void SkeletalGraph::SetBoundaryPointsUsingGivenPoints()
{
	DDSEdgeVect::iterator edgeIt;
	unsigned int i;

	ASSERT(m_pDDSGraph);

	DDSEdgeVect& edges = m_pDDSGraph->getEdges();

	for(edgeIt = edges.begin(); edgeIt != edges.end(); edgeIt++)
	{
		FluxPointList& fpl = (*edgeIt)->getFluxPoints();
		BoundaryInfoList& bil = (*edgeIt)->getBoundaryInfoList();

		for (i = 0; i < bil.size(); i++)
		{
			BoundaryInfo& bi = bil[i];
			FluxPoint& fp = fpl[i];

			Vector v(bi.first.pt.x - fp.p.x, bi.first.pt.y - fp.p.y);
			Vector t = bi.tangent;

			bi.alpha = acos(t.dot(v) / (t.norm() * v.norm()));

			t.rotate(-bi.alpha);
			t.normalize();

			bi.second.pt.x = fp.p.x + t.x * fabs(fp.dist);
			bi.second.pt.y = fp.p.y + t.y * fabs(fp.dist);
			bi.second.index = bi.first.index;
			bi.second.dtValue = 0;
		}
	}
}
#endif

/*!
	@brief Assigns the boundary point information of each point in the skeleton graph data structure.
	It follows a nearest neighbour search approach. This is the only function that modifies the input
	skeleton graph.

	Essentially, this function fills the information in the BoundaryInfoList of each
	edge in the skeleton graph. In fact, the BoundaryInfoList member variable was added
	to the DDSGraph class so that we could assing the needed info in this function.
*/
void SkeletalGraph::SetBoundaryPointsUsingNNSearch(int nSmoothIter /*= 0*/)
{
	ASSERT(m_pBndryPtFinder == NULL); // or simply delete it (may have to if recomputing)

	ShapeBoundary bndry(m_pDDSGraph);

	unsigned int numPts = bndry.Size();
	std::vector<sg::Point> boundaryPts(numPts);

	m_pBndryPtFinder = new BoundaryPointFinder(numPts);

	// Copy all points to a simple array
	for (unsigned int i = 0; i < numPts; i++)
		boundaryPts[i] = bndry[i];

	// Init the bndry point finder
	m_pBndryPtFinder->AddBoundaryPoints(boundaryPts, nSmoothIter);

	m_pBndryPtFinder->AssignBoundaryPoints(m_pDDSGraph);

	/*SkelBranch* pBranch;

	// Assing a pair of bndry points to each point in each branch
	forall_branches(pBranch, m_pDDSGraph->getEdges())
	{
		m_pBndryPtFinder->AssignBoundaryPoints(
			pBranch->getFluxPoints(),
			pBranch->getBoundaryInfoList());
	}*/

		// Make sure that the spokes do not cross
	//CorrectCrossingSpokes(fpl[0].p, bil[0].first, fpl[N].p, bil[N].first);
	//CorrectCrossingSpokes(fpl[0].p, bil[0].second, fpl[N].p, bil[N].second);
}

//! Here we average the points. Diego: Bug fixed. it was "eating" one point.
void SmoothBranchPoints(sg::FluxPointList& pts)
{
	if (pts.size() <= 2)
		return;

	// We have at least 3 points
	std::vector<sg::Point> spts(pts.size());

	unsigned int sz = pts.size() - 1;
	unsigned int i;

	for (i = 1; i < sz; i++)
	{
		spts[i].x = (pts[i - 1].p.x + pts[i].p.x + pts[i + 1].p.x) / 3.0;
		spts[i].y = (pts[i - 1].p.y + pts[i].p.y + pts[i + 1].p.y) / 3.0;
	}

	for (i = 1; i < sz; i++)
	{
		pts[i].p = spts[i];
	}
}

/*
	@brief Uses the info computed by AssignBoundaryInfo() to the set
	bndry pts of a NEW branch.
*/
void SkeletalGraph::AssignBoundaryPoints(const sg::FluxPointList& fpl,
										 sg::BoundaryInfoList& bil,
										 const sg::BoundaryInfo& bi0,
										 const sg::BoundaryInfo& biN)
{
	ASSERT(m_pBndryPtFinder);
	m_pBndryPtFinder->AssignBoundaryPoints(fpl, bil, bi0, biN);
}

/*!
	@brief Uses the info computed by AssignBoundaryInfo() to set the
	radius values of a NEW branch
*/
void SkeletalGraph::AssignRadiusValues(const sg::BoundaryInfoList& bil,
									   sg::FluxPointList& fpl)
{
	ASSERT(m_pBndryPtFinder);
	m_pBndryPtFinder->AssignRadiusValues(bil, fpl);
}