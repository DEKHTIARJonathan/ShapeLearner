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
 * $Log: SkeletalGraph.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __SKELETAL_GRAPH_H__
#define __SKELETAL_GRAPH_H__

#include "FluxSkeleton\include\DDSGraphProject.h"

#include <fstream>

#include "PolyLineTLSApprox.h"
#include "PolyBezierApprox.h"
#include "DMLString.h"
#include "DDSGraphUtils.h"

template<class T> class FIELD; // for AFMM skeleton in ImageInfo

namespace dml {
class BoundaryPointFinder;

struct ImageInfo
{
	String strFileName;
	FIELD<float>* pField;
	double noiseLevel;

	ImageInfo() { noiseLevel = 0; pField = NULL; }
	ImageInfo(const char* fn) : strFileName(fn) { noiseLevel = 0; pField = NULL; }
};

//! SkeletalGraph parameters for construction
struct SkeletalGraphParams
{
	enum SKEL_ALGO {VoronoiAlgorithm = 1, FluxAlgorithm = 2, AFMMAlgorithm = 3};

	int nSkelAlgorithm; // 2 = FluxSkeleton, 3 = AFMMSkeleton

	// Flux Skeleton Params
	double fluxStep, fluxTau, fluxResolution;

	//AFMM Skeleton Params
	double afmmTau1, afmmTau2;

	// Skeleton Simplification Params
	int nBndryRecErrorWeight, nStrucRecErrorWeight;
	int nSimplifyExternal, nSimplifyComplete, nSimplifyInternal;

	// Skeleton Approximation Params
	double dMinLinearError, dMinBezierError;
	int dMaxLineSegments, dMaxBezierSegments;

	SkeletalGraphParams() { memset(this, 0, sizeof(SkeletalGraphParams)); }
};

/*!
	The SkeletalGraph class wrapps and extends the DiscreteDivergenceSkeleton (aka FluxSkeleton)
	in the FluxSkeleton project. Currently, the AFMMSkeleton class inherits
	from the DiscreteDivergenceSkeleton and so the wrapper may contain a FluxSkeleton
	or an AFMMSkeleton.
*/
class SkeletalGraph
{
protected:
	sg::DDSGraph* m_pDDSGraph;              //!< It may point to a DDSGraph or a AFMMSkeleton

	BoundaryPointFinder* m_pBndryPtFinder;  //!< Finds closest point on shape boundary

	SmartArray<EuclideanLineSegmentArray> m_lineSegments;
	SmartArray<BezierSegmentArray> m_bezierSegments;

public:
	SkeletalGraph(sg::DDSGraph* p = NULL) { m_pDDSGraph = p; m_pBndryPtFinder = NULL; }

	~SkeletalGraph();

	bool Create(const ImageInfo& imgInfo, const SkeletalGraphParams& params,
		ShapeDims* pDims = NULL);

	bool CreateFluxSkeleton(const char* szPPMFileName, const SkeletalGraphParams& params,
		ShapeDims* pDims = NULL);

	bool CreateAFMMSkeleton(const ImageInfo& imgInfo, const SkeletalGraphParams& params,
		ShapeDims* pDims = NULL);

	bool Regularize();

	const sg::DDSGraph* GetDDSGraph() const { return m_pDDSGraph; }
	sg::DDSGraph* GetDDSGraph() { return m_pDDSGraph; }
	//void SetSkeleton(sg::DDSGraph* p) { m_pDDSGraph = p; }

	sg::DDSGraph* ReadDDSGraph(std::istream& is);
	void WriteDDSGraph(std::ostream& os, sg::DDSGraph& dds);

	void ReadNodesAndEdges(std::istream& is, sg::DDSGraph& dds);
	void WriteNodesAndEdges(std::ostream& os, sg::DDSGraph& dds);

	void Read(std::istream& is) { m_pDDSGraph = ReadDDSGraph(is); }
	void Write(std::ostream& os) { WriteDDSGraph(os, *m_pDDSGraph); }

	void ApproxSkeletonWithLines(double dMinError, int nMaxSegments);
	void ApproxSkeletonWithCubicBeziers(double dMinError, int nMaxSegments);

	// Inline functions for display purposes
	SmartArray<EuclideanLineSegmentArray> GetLinearApproximation() const
	{
		return m_lineSegments;
	}

	void SetLinearApproximation(SmartArray<EuclideanLineSegmentArray> lineSegs)
	{
		m_lineSegments = lineSegs;
	}

	SmartArray<BezierSegmentArray> GetBezierApproximation() const
	{
		return m_bezierSegments;
	}

	void SetBezierApproximation(SmartArray<BezierSegmentArray> bezierSegs)
	{
		m_bezierSegments = bezierSegs;
	}

	//!	Returns a wrapper class (just a pointer) to the shape boundary
	ShapeBoundary GetShapeBoundary()
	{
		return ShapeBoundary(m_pDDSGraph);
	}

	//!	Returns the number of points along the shape boundary
	unsigned int GetBoundarySize()
	{
		return ShapeBoundary(m_pDDSGraph).Size();
	}

#ifdef USE_EXTRA_BOUNDARY_INFO
	void SetBoundaryPointsUsingGivenPoints();
#endif

	void SetBoundaryPointsUsingNNSearch(int nSmoothIter = 0);

	//! Assign bndry info to each skeletal point
	void AssignBoundaryInfo(int nSmoothIter = 0)
	{
		SetBoundaryPointsUsingNNSearch(nSmoothIter);
	}

	void AssignBoundaryPoints(const sg::FluxPointList& fpl, sg::BoundaryInfoList& bil,
		const sg::BoundaryInfo& bi0, const sg::BoundaryInfo& biN);

	void AssignRadiusValues(const sg::BoundaryInfoList& bil, sg::FluxPointList& fpl);

	BoundaryPointFinder* GetBndryPtFinder() { return m_pBndryPtFinder; }

	const SkelNodes& getNodes() const { return m_pDDSGraph->getNodes(); }
	SkelNodes& getNodes()             { return m_pDDSGraph->getNodes(); }

	const SkelEdges& getEdges() const { return m_pDDSGraph->getEdges(); }
	SkelEdges& getEdges()             { return m_pDDSGraph->getEdges(); }
};
} // namespace dml

#endif //__SKELETAL_GRAPH_H__
