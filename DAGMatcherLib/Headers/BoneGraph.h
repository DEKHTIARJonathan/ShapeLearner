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
 * $Log: BoneGraph.h,v $
 *
 *-----------------------------------------------------------------------*/

#ifndef __BONE_GRAPH_H__
#define __BONE_GRAPH_H__

#include "stdafx.h"

namespace dml {
/*!
   \brief Represents a gesture graph consisting of <BGNode *>'s.

   \see DAG, DAGNode, BGNode.
*/
class BoneGraph : public DAG
{
protected:
   SkeletalGraph* m_pSkeleton;

   static DAGMatcher* s_pDAGMatcher; //!< Matching algorithm for bone graphs

public:
   // Member variables for displaying and debugging the representation
   BezierSegmentArray m_skeletalGaps;
   SmartArray<POINTS> m_boundaryGaps;

public:
   BoneGraph();
   ~BoneGraph();

   bool Create(const ImageInfo& imgInfo, const BoneGraphParams& bgparams,
      const SkeletalGraphParams& skelparams);

   bool Create(SkeletalGraph* pSkeleton, const ShapeDims& dims, String strLbl,
      const BoneGraphParams& bgparams);

   int GetSlopeDir(leda::node u, leda::node wrtV) const;

   // DAG virtual functions
   virtual DAG& operator=(const DAG& rhs);
   virtual BoneGraph& operator=(const BoneGraph& rhs);

   virtual void Clear();

   virtual void ComputeDerivedValues();

   virtual DAGEdgePtr ReadEdge(std::istream& is);

   virtual double ComputeTSVs(leda::node root);

   // DAG pure virtual functions
   virtual DAGMatcher* GetMatchingAlgorithm() const { return s_pDAGMatcher; }

   virtual void SetMatchingAlgorithm();
   virtual bool AreNodesRelated(leda_node g1Node, const DAG& g2, leda_node g2Node) const;
   virtual DAG* CreateObject() const;
   virtual DAGNodePtr CreateNodeObject(NODE_LABEL lbl) const;
   virtual DAGNodePtr ReadNode(std::istream& is) const;
   virtual String ClassName() const;
   virtual int NodeType(leda_node v) const { return GetBGNode(v)->Type(); }

   //! Returns the stroed skeletal graph or NULL if it does not exist
   virtual const SkeletalGraph* GetSkeleton() const { return m_pSkeleton; }

   //! Releases the ownership of the stored skeletal graph (if it exists)
   virtual SkeletalGraph* ReleaseSkeleton()
   {
      SkeletalGraph* pSkel = m_pSkeleton;
      m_pSkeleton = NULL;
      return pSkel;
   }

   virtual std::istream& Read(std::istream& is, bool bOnlyDataForMatching = false);
   virtual std::ostream& Write(std::ostream& os) const;
   virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const;


   const BGNode* GetBGNode(leda::node v) const { return (const BGNode*)(const DAGNode*)GetNode(v); }
   BGNode* UnsafeGetBGNode(leda::node v) { return (BGNode*)GetBGNode(v); }

   const BGEdge* GetBGEdge(leda::edge e) const { return (const BGEdge*)(const DAGEdge*)GetEdge(e); }
   BGEdge* UnsafeGetBGEdge(leda::edge e) { return (BGEdge*)GetBGEdge(e); }

   double GetInwardEdgeSaliency(leda::node v) const
   {
      leda::edge e;
      double sal = 0;

      forall_in_edges(e, v)
         sal += GetBGEdge(e)->Saliency();

      return sal;
   }

   double GetOutwardEdgeSaliency(leda::node v) const
   {
      leda::edge e;
      double sal = 0;

      forall_out_edges(e, v)
         sal += GetBGEdge(e)->Saliency();

      return sal;
   }
};
} //namespace dml

#endif //__BONE_GRAPH_H__
