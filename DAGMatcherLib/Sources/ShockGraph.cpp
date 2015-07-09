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
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#include "base64.h"

#define NSAMPLES	 5
#define VDIM		 4
#define DIMxSAMPLES  (VDIM * NSAMPLES)

#define SA_GROWING_FACTOR 50

using namespace std;
using namespace dml;
using namespace leda;

struct ER // Pair of edge and radius values
{
   leda_edge e;
   double r; // avg radius of target(e)

   ER() { e = nil; r = 0.0; }
   ER(leda_edge edge, const double& radius) { e = edge; r = radius; }

   /*!
   @brief Helper function to compare the edges that will be reverted.
   We want to consider the nodes with greatest radius first.

     The compare function ussuallry performs elem1 - elem2, and elemnts get
     sorted from smallest to largest. i.e., if elem1 < elem 2, we should return
     -1 to get an ascending ordering.
     We want descending sort, so elem1 < elem2 should return 1.
   */
   static int CompareEdgesDescending(const void *elem1, const void *elem2 )
   {
      ER* p1 = (ER*) elem1;
      ER* p2 = (ER*) elem2;

      return (p1->r < p2->r) ? 1:(p1->r > p2->r) ? -1:0;
   }
};

/////////////////////////////////////////////////////////////////////////////////
// Init static variables
DAGMatcher* ShockGraph::s_pDAGMatcher = NULL;

/////////////////////////////////////////////////////////////////////////////////
// class member functions

const char* ShockGraph::GetNextIdx()
{
   static char szNum[100];

   sprintf(szNum, "%d", ++m_nLastIndexUsed);
   return szNum;
}

ShockGraph::ShockGraph()
{
   m_nLastIndexUsed = 0;
   m_pSkeleton = NULL;

   // Init the matching algorithm for shock graphs
   if (s_pDAGMatcher == NULL)
      SetMatchingAlgorithm();
}

//! Sets the matching algorithm from the values in the match params
void ShockGraph::SetMatchingAlgorithm()
{
   int matchAlgo = DAGMatcher::GetMatchParams().nMatchingAlgorithm;
   int nodeSimFunc = DAGMatcher::GetMatchParams().nNodeSimilarityFunction;
   SimilarityMeasurer* pNDM;

   if (nodeSimFunc == 0) // default is zero
   {
      pNDM = new SGRadialSimilarityMeasurer;
   }
   else
   {
      ShowError("Unknown node distance function ID");
      throw;
   }

   // Delete any existing algorithm before creating a new one
   delete s_pDAGMatcher;

   if (matchAlgo == 0)
      s_pDAGMatcher = new DAGMatcherGreedy(pNDM);
   else if (matchAlgo == 1)
      s_pDAGMatcher = new DAGMatcherOptimal(pNDM);
   else if (matchAlgo == 2)
      s_pDAGMatcher = new DAGMatcherTopological(pNDM);
   else if (matchAlgo == 3)
      s_pDAGMatcher = new DAGMatcherAdaptive(pNDM);
   else
   {
      ShowError("Unknown matching algorithm ID");
      throw;
   }
}

ShockGraph::~ShockGraph()
{
   delete m_pSkeleton;
}

void ShockGraph::Clear()
{
   DAG::Clear();

   m_nLastIndexUsed = 0;

   delete m_pSkeleton;
   m_pSkeleton = NULL;
}

//! Extends the behaviour of the same function in the base class.
istream& ShockGraph::Read(istream& is, bool bOnlyDataForMatching /*= false*/)
{
   DAG::Read(is, bOnlyDataForMatching);

   if (!bOnlyDataForMatching)
   {
      // pSkeleton works as a boolean variable here
      is.read((char*) &m_pSkeleton, sizeof(m_pSkeleton));
      if (m_pSkeleton)
      {
         m_pSkeleton = new SkeletalGraph();
         m_pSkeleton->Read(is);
      }
   }

   return is;
}

//! Extends the behaviour of the same function in the base class.
ostream& ShockGraph::Write(ostream& os) const
{
   DAG::Write(os);

   // pSkeleton works as a boolean variable here
   os.write((char*) &m_pSkeleton, sizeof(m_pSkeleton));
   if (m_pSkeleton)
      m_pSkeleton->Write(os);

   return os;
}

//! Extends the behaviour of the same function in the base class.
void ShockGraph::Print(ostream& os, bool bXMLFormat) const
{
   if (bXMLFormat)
   {
      XML_OPEN_ATTS(os, "DAG");
      XML_PRINT_ATT2(os, "class", ClassName());
      XML_PRINT_ATT2(os, "nodeCount", GetNodeCount());
      XML_END_ATTS(os);

      DAG::Print(os, bXMLFormat);

      //{
      //   ofstream fileTmp("c:\\users\\monville\\desktop\\graph.bin", std::ofstream::binary) ;
      //   write(fileTmp) ;
      //}

      //{
      //   ostringstream ba ;
      //   write(ba) ;
      //   std::string bas= ba.str() ;
      //   
      //   ofstream fileTmp("c:\\users\\monville\\desktop\\graph.str.bin", std::ofstream::binary) ;
      //   int s= bas.size();
      //   fileTmp.write(bas.c_str(), bas.size()) ;
      //}


      {
         ostringstream ba ;
         write(ba) ;
         std::string bas= ba.str() ;
         
         std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(bas.c_str()), bas.size()) ;

         //ofstream fileTmp("c:\\users\\monville\\desktop\\graph.str.base64.txt") ;
         //fileTmp << encoded ;

         static const char* DAG_BASE64= "DAG_BASE64" ;
         XML_OPEN_ATTS(os, DAG_BASE64) ;
         XML_PRINT_ATT2(os, "content", encoded) ;
         XML_CLOSE(os, DAG_BASE64) ;
      }



      XML_CLOSE(os, "DAG");
   }
   else
   {
      DAG::Print(os, bXMLFormat);

      os << "\nShockGraph specific:\n";

      PRINT_OPEN(os, m_dims.xmin);
      PRINT(os, m_dims.xmax);
      PRINT(os, m_dims.ymin);
      PRINT_CLOSE(os, m_dims.ymax);
   }
}


bool ShockGraph::ReadFromXML( const char* xmlContentCString, bool bOnlyDataForMatching )
{
   const std::string xmlContent(xmlContentCString) ;

   static const char* base64attribute= "DAG_BASE64=\"" ;
   std::string::size_type indexStart= xmlContent.find_last_of(base64attribute) + strlen(base64attribute) ;
   std::string::size_type indexEnd= xmlContent.find_first_of("\">", indexStart) ;

   std::string base64Str(xmlContent.substr(indexStart, indexEnd-indexStart)) ;

   std::string bytes(base64_decode(base64Str)) ;
   std::stringstream ss ;
   ss.write(bytes.c_str(), bytes.size()) ;
   
   Read(ss, bOnlyDataForMatching) ;
   
   return true ;
}







/*!
   We MUST have this operator, otherwise C++ would create one for us
   with this exact parameters (and highest precedence over every other
   operator=), and that's no good since m_pSkeleton is a pointer!!!!
*/
ShockGraph& ShockGraph::operator=(const ShockGraph& rhs)
{
   DAG::operator=(rhs);

   // @TODO: get a copy of the skel ptr by
   // m_pSkeleton = new SkeletalGraph(*m_pSkeleton->GetSkeleton());
   // right know, m_pSkeleton is set to NULL. Ie, isn't copied.

   return *this;
}

/*!
   Extends the behaviour of the same function in the base class. This function may
   never be called, but we leave it to make sure that if it is ever called, it does the
   right thing.
*/
DAG& ShockGraph::operator=(const DAG& rhs)
{
   // Make sure we have a ShockGraph on the rhs
   const ShockGraph* pRhs = dynamic_cast<const ShockGraph*>(&rhs);

   if (!pRhs)
      THROW_EXCEPTION("Invalid pointer type.");

   // Now, can can proceed as usual with the right operator=
   return operator=(*pRhs);
}

/*!
Relation function for two Shock Graph nodes.
For now, all nodes of the same type are related.
*/
bool ShockGraph::AreNodesRelated(leda_node u, const DAG& from, leda_node v) const
{
   return true;
}

//! Creates a new shock graph object.
DAG* ShockGraph::CreateObject() const
{
   return new ShockGraph();
}

//! Creates a new shock graph node object.
DAGNodePtr ShockGraph::CreateNodeObject(NODE_LABEL lbl) const
{
   return new SGNode(lbl, TEST_NODE);
}

/*!
Creates a new SGNode and fill its contents with the current
information in the stream.
*/
DAGNodePtr ShockGraph::ReadNode(istream& is) const
{
   DAGNodePtr node(new SGNode);

   node->Read(is);

   return node;
}

//! Returns the classs name
String ShockGraph::ClassName() const
{
   return "ShockGraph";
}

/*!
   @brief Determines whether the end shock points in the given branch are joints or terminal points.

   In general, joint points will be outliers wrt the points in the branch.
*/
void ShockGraph::LabelEndPoints(SGNode* pNode, sg::FluxPointList& fpl, sg::DDSNode* n1, sg::DDSNode* n2)
{
   sg::DDSEdgeVect *e0, *eN;

   if (n1->getPoint() == fpl[0].p)
   {
      ASSERT(n2->getPoint() == fpl[fpl.size() - 1].p);

      e0 = &n1->getEdges();
      eN = &n2->getEdges();
   }
   else
   {
      ASSERT(n1->getPoint() == fpl[fpl.size() - 1].p && n2->getPoint() == fpl[0].p);

      e0 = &n2->getEdges();
      eN = &n1->getEdges();
   }

   pNode->m_nEndPt0 = (e0->size() == 1) ? TERMINAL_POINT:JOINT_POINT;
   pNode->m_nEndPtN = (eN->size() == 1) ? TERMINAL_POINT:JOINT_POINT;
}

SGNode* ShockGraph::CopyNodeInfo(sg::DDSEdge* ddsEdge, leda_node u, SGNode* pNode)
{
   sg::FluxPointList& fpl = ddsEdge->getFluxPoints();
   int num_pts = fpl.size();
   ASSERT(num_pts > 0);
   double dx, dy;

   pNode->m_shocks.Resize(num_pts);

   for (int j = 0; j < num_pts; j++)
   {
      const sg::FluxPoint& pt = fpl[j];
      ShockInfo& si = pNode->m_shocks[j];

      si.xcoord = pt.p.x;
      si.ycoord = pt.p.y;
      si.radius = fabs(pt.dist);
      si.color = 0; //pt.col; // not used anymore. see type instead

      // Some NaN have been found in a value for radius
      if (isnan(si.radius) && !finite(si.radius))
      {
         cerr << "\nShock point has a NaN as radius." << endl;
         // Try to recover from this
         si.radius = (j > 0) ? pNode->m_shocks[j - 1].radius:0.5;
      }

      if (j > 0)
      {
         ShockInfo& prevSi = pNode->m_shocks[j - 1];

         dx = si.xcoord - prevSi.xcoord;
         dy = si.ycoord - prevSi.ycoord;

         si.dr = si.radius - prevSi.radius;
         si.speed = sqrt(dx * dx + dy * dy);
         si.dr_ds = si.dr / si.speed;

         // dir is either -1, +1 or 0
         si.dir = (fabs(si.dr_ds) <= m_compParams.dMinSlope) ? 0:int(si.dr / fabs(si.dr));
      }
      else
      {
         si.dr = 0;
         si.speed = 0;
         si.dr_ds = 0;
         si.dir = 0;
      }
   }

   LabelEndPoints(pNode, fpl, ddsEdge->getN1(), ddsEdge->getN2());

   return GroupShockPoints(u, pNode);
}

/*!
   @brief Groups the points according to their type. It also computes the
   derived values right now, since the points will not be subdivided further.
*/
SGNode* ShockGraph::GroupShockPoints(leda_node u, SGNode* pNode)
{
   ShockBranch& pts = pNode->m_shocks;
   SGNode* pNewNode;
   SGNode* pReturnNode = NULL;

   int i, from;

   int d0, dN;	// if d0 or dN == 1, means their end points were chopped off
   POINTS data = pNode->GetVelocityRadiusArray(d0, dN);

   double x0 = data[0].x;

   // Time to fit the data with a few lines
   PolyLineApprox poly(data.GetSize() / m_compParams.dMinError,
      m_compParams.dMinSlope, 10, m_compParams.dMaxYDiff);

   if (DAG::IsDbgMode())
      Logger::Log("disp('Fitting node: " + (std::string)pNode->GetNodeLbl() + " in " + (std::string)GetDAGLbl() + "');\n", constants::LogCore);

   poly.Fit(data);

   int nSize = poly.m_knots.GetSize();
   ASSERT(nSize >= 1);
   int dir, ptIdx;
   SmartArray<LineSegment> approx_segs;

   for (from = 0, i = 0; i < nSize; i++)
   {
      approx_segs.AddTail(poly.m_knots[i].seg);

      dir = poly.m_knots[i].dir;
      ptIdx = poly.m_knots[i].nIndex + d0;

      if (i < nSize - 1)
      {
         // If the segment is too small, skip it.
         if (ptIdx - from < 2)
            continue;

         // If there is a significant change in acceleration,
         // we create a new node, even though the direction is the same.
         if (dir == poly.m_knots[i + 1].dir)
         {
            if (dir == 0)
               continue;

            //poly.CompAcuteAngle(i, i + 1) < m_compParams.dMaxAccelChg)
            double m0 = poly.m_knots[i].seg.m;
            double m1 = poly.m_knots[i + 1].seg.m;
            double mr = fabs(m1 - m0) / fabs((fabs(m0) >= fabs(m1)) ? m0 : m1);

            if (DAG::IsDbgMode())
                  Logger::Log("disp('Acc change: " + to_string((long double) mr ) + ", Max:" +  to_string((long double) m_compParams.dMaxAccelChg ) + "');", constants::LogCore);

            if (mr < m_compParams.dMaxAccelChg)
               continue;
         }

         pNewNode = SplitNode(u, pNode, ptIdx - from,
            approx_segs[approx_segs.GetSize() - 1], poly.m_knots[i + 1].seg);
      }
      else
         pNewNode = pNode;

      // Record the first node of the chain
      if (pReturnNode == NULL)
         pReturnNode = pNewNode;

      pNewNode->SetNodeType(dir == 0 ? SHOCK_3:SHOCK_1);
      pNewNode->m_shocks.SetDir(dir);

      // Update the fitting segments. Note that if <from> != 0, the x coord of points in the
      // branch will be shifted. SetSegments will correct this given the displacement.
      pNewNode->m_shocks.SetSegments(approx_segs, from == 0 ? 0.0:approx_segs[0].p0.x - x0);

      pNewNode->ComputeDerivedValues();

      from = ptIdx;
      approx_segs.Clear();
   }

   return pReturnNode;
}

/*
   @brief Splits in two the shock points in a node. It returns the new node created that
   contains the first points. It also links the two nodes.
*/
SGNode* ShockGraph::SplitNode(leda_node v, SGNode* pNode, int nEnd,
                       LineSegment& leftSeg, LineSegment& rightSeg)
{
   SGNode* pNewNode = new SGNode(GetNextIdx(), 0);
   ShockBranch& newPts = pNewNode->m_shocks;
   ShockBranch& pts = pNode->m_shocks;

   // We must repeat the endpoint where the nodes connect. nEnd is inclusive
   pNewNode->m_shocks = pts.Left(nEnd);

   // Now remove the left part. nEnd is inclusive
   pts = pts.Right(nEnd);

   // Create new node
   leda_node u = NewNode(pNewNode);

   // Update the m_nEndPt0 and m_nEndPtN member variables of both nodes
   pNewNode->m_nEndPt0 = pNode->m_nEndPt0;
   pNewNode->m_nEndPtN = SPLIT_POINT;
   pNode->m_nEndPt0 = SPLIT_POINT;

   // Update the Y value (radius) at the joint pt, to avoid outliers in this crucial
   // function. i.e. type 2 and 4 will depend on accurate values here
   double jointRadius = (leftSeg.Y1() + rightSeg.Y0()) / 2.0;
   pNewNode->m_shocks[pNewNode->m_shocks.GetSize() - 1].radius = jointRadius;
   pts[0].radius = jointRadius;
   leftSeg.p1.y = jointRadius;
   rightSeg.p0.y = jointRadius;

   // If 'v' has a parent node, move it to be a parent of 'u' instead
   leda_edge e = first_in_edge(v);

   if (e != nil)
    {
      NewEdge(source(e), u);
      ASSERT(in_succ(e) == nil); // There should be only one edge
      DelEdge(e);
    }

   // Link u to v
   NewEdge(u, v);

   return pNewNode;
}

/*!
   @brief Traverses the skeleton and creates a "pre" Shock Graph
*/
void ShockGraph::ConnectNodes(leda_node u, SGNode* pNode, sg::DDSEdge* e, sg::DDSNode* n)
{
   using namespace sg;

   // Then, keep creating and connecting nodes
   DDSEdgeVect& edges = n->getEdges();
   DDSEdgeVect::iterator I;
   DDSEdge* ee;
   SGNode* pNewNode;
   leda_node v;

   for(I = edges.begin(); I != edges.end(); I++)
   {
      ee = *I;

      if (e != ee)
      {
         pNewNode = new SGNode(GetNextIdx(), SHOCK_base);
         v = NewNode(pNewNode);
         NewEdge(u, v);
         CopyNodeInfo(ee, v, pNewNode);
         ConnectNodes(v, pNewNode, ee, ee->getN1() != n ? ee->getN1():ee->getN2());
      }
   }
}

/*!
   @brief Reverses the direction of an edge and moves all the edges that meet
   at a junction.

   All the children of a node are linked with each other at a juntion, so
   when we move a node up, we need to redistribute the relations in the juntion as well.

   we start with parent->x and check for two cases: parent->x or parent->x->children
*/
void ShockGraph::RevertEdge(leda_edge pxEdge)
{
   leda_node parent = source(pxEdge);
   leda_node x = target(pxEdge);
   leda_edge e;

   rev_edge(pxEdge);

   // Move all children of x's parent (that are also connected to x)
   // to the left side of x
   const ShockInfo& inPt = GetJointPoint(parent, x);

      forall_adj_edges(e, parent)
         if (GetJointPoint(parent, target(e)) == inPt)
            move_edge(e, x, target(e));
}

/*!
   @brief "Tries" to relabel type 3 nodes acting as a 2s or 4s.
   i.e., a type 3 branch that is a sort of neck between two type 1s
   of opposite directions; or a type 3 that is an implicit root.
*/
void ShockGraph::TryRelabel3As2or4(leda_node v)
{
   ASSERT(NodeType(v) == SHOCK_3);

   int parentType = NodeType(GetFirstParent(v));

   if (parentType == ROOT)
      UnsafeGetSGNode(v)->SetNodeType(SHOCK_4);
   else if(outdeg(v) == 0 && indeg(v) == 2)
      UnsafeGetSGNode(v)->SetNodeType(SHOCK_2);

   // If we are interested in spliting 3's into left and
   // right nodes, we can do it here.
   if (m_compParams.nSlipt3s != 0 && parentType == ROOT)
   {
      const SGNode& x = *GetSGNode(v);

      const ShockInfo& leftPt = x[0];
      const ShockInfo& rightPt = x[x.GetSize() - 1];

      leda_node leftChild = NewNode(new SGNode(x));
      leda_node rightChild = NewNode(new SGNode(x));
      leda_edge e;

      forall_adj_edges(e, v)
      {
         if (GetJointPoint(v, target(e)) == leftPt)
            move_edge(e, leftChild, target(e));
         else
         {
            ASSERT(GetJointPoint(v, target(e)) == rightPt);
            move_edge(e, rightChild, target(e));
         }
      }

      // Link x to its left and right sides
      NewEdge(v, rightChild);
      NewEdge(v, leftChild);
   }
}

/*!
  @brief Inserts type 4 and type 2 nodes, which are local maxima or minima
  SINGLE shock points.

  If there are a bunch of type 1 nodes connected at a point and they have
  the same direction with respect to the join point, then the joint is either a
  type 2 or a type 4.
*/
void ShockGraph::Insert4sAnd2s()
{
   leda_node u, v;
   leda_edge e;
   bool bAllType1SameDir;
   int dir;

   forall_nodes(u, *this)
   {
      if (NodeType(u) == SHOCK_1 && outdeg(u) > 0)
      {
         bAllType1SameDir = true;
         dir = GetBranchDir(u, GetFirstChild(u));

         // Check if all children of <u> are type 1
         forall_adj_nodes(v, u)
         {
            if (NodeType(v) != SHOCK_1 || GetBranchDir(v, u) != dir)
            {
               bAllType1SameDir = false;
               break;
            }
         }

         if (bAllType1SameDir)
         {
            const ShockBranch& pts = GetSGNode(u)->m_shocks;
            int type = dir == 1 ? SHOCK_2:SHOCK_4;

            if (type == SHOCK_2 && outdeg(u) != 1)
               return; // we only consider necks between two branches

            SGNode* pNewNode = new SGNode(GetNextIdx(), type);
            pNewNode->m_shocks.AddTail(GetJointPoint(u, GetFirstChild(u)));
            pNewNode->ComputeDerivedValues();

            bool localMinOrMax = false;

            //Before inserting the node, in case of a type 4,
            // we must make sure it's really a local maximum pt
            if (type == SHOCK_4)
            {
               if (pNewNode->m_shocks > GetSGNode(u)->m_shocks)
               {
                  localMinOrMax = true;

                  forall_adj_nodes(v, u)
                  {
                     if (!(pNewNode->m_shocks > GetSGNode(v)->m_shocks))
                     {
                        localMinOrMax = false;
                        break;
                     }
                  }
               }
            }
            else if (type == SHOCK_2)
            {
               if (GetSGNode(u)->m_shocks > pNewNode->m_shocks &&
                     GetSGNode(GetFirstChild(u))->m_shocks > pNewNode->m_shocks)
                  localMinOrMax = true;
            }

            if (localMinOrMax) // We have either a type 2 or a type 4
               {
               v = NewNode(pNewNode);

               forall_adj_edges(e, u)
                  move_edge(e, v, target(e));

               NewEdge(u, v); // keep the old order u -> new (v) -> u's children
            }
            else
               delete pNewNode;
         }
      }
   }
}

void ShockGraph::ComputeNodeRole(leda_node v)
{
   NODE_ROLE role;
   int nType = NodeType(v);

   if (nType == ROOT)
      role = BIRTH;
   else if (nType == SHOCK_2 || (nType == SHOCK_3 && outdeg(v) == 0))
      role = UNION;
   else if (outdeg(v) == 0)
      role = DEATH;
   else
      role = PROTRUSION;

   UnsafeGetSGNode(v)->SetNodeRole(role);
}

/*!
   @brief Collect the edges that need to be reverted according to time of formation.
   Edges must be directed from last-to-form to earliest-to-form.
*/
void ShockGraph::DirectEdgesAcordingToTime()
{
   bool bContinue;
   leda_edge e;
   int nNoInfLoop = 0;

   do
   {
      ASSERT(nNoInfLoop++ < 1000);
      bContinue = false;

      forall_edges(e, *this)
      {
         const ShockBranch& tgt = GetSGNode(target(e))->m_shocks;
         const ShockBranch& src = GetSGNode(source(e))->m_shocks;

         if (tgt > src)
         {
            RevertEdge(e);
            bContinue = true;
         }
      }
   } while (bContinue);
}

/*!
   @brief Add common root node, generalize type 2 and 4, compute node roles.

   common root node: add a dummy root node so that there is a single root.
   generalize 2 and 4: Type 3 that are children of root are type 4 and type 3s
      that are terminal nodes with two type 1s as parents are type 2.
   node roles: assigns roles according to the sg grammar.
*/
void ShockGraph::FinalizeSGConstruction()
{
   // Nodes that do not have a parent should be children of the root node.
   leda_node root = NewNode(new SGNode("#", ROOT));
   leda_node v, p1;
   int nodeType;

   // BTW, type 3 nodes with 2 type 1 parent/children should
   // be relabeled as 2/4 respectivelly.
   forall_nodes(v, *this)
   {
      if (v != root && indeg(v) == 0)
         NewEdge(root, v);

      /* OPTIONAL STEP */
      if (NodeType(v) == SHOCK_3) // MAY need to be relabeled.
         TryRelabel3As2or4(v);

      nodeType = NodeType(v);

      // At this point some basic grammatical rules MUST be satisfied
      ASSERT (nodeType == ROOT ||
         nodeType == SHOCK_1 ||
         nodeType == SHOCK_3 ||
         (nodeType == SHOCK_2 && indeg(v) == 2 && outdeg(v) == 0) ||
         (nodeType == SHOCK_4 && indeg(v) == 1 && NodeType(GetFirstParent(v)) == ROOT));

      // Relabel node using grammatical knowledge whenever there are errors...
      // Rule 4 errors: A 1 with many 3s as children => all 3's are 1's
      if (nodeType == SHOCK_3)
      {
         p1 = GetFirstParent(v);

         if (NodeType(p1) == SHOCK_1 && indeg(v) == 1 && outdeg(p1) > 1)
            UnsafeGetSGNode(v)->SetNodeType(SHOCK_1);
         else if (NodeType(p1) == SHOCK_3 && indeg(v) == 1 && outdeg(p1) > 1)
            UnsafeGetSGNode(v)->SetNodeType(SHOCK_1);
      }

      ComputeNodeRole(v);
   }
}

/*!
@brief Maps the edges in the DDSkeleton to nodes in a shock graph and copies the shocks' info.

  The DDSkeleton has the shock point info stored in the edges of the graph. The endpoint shocks are the nodes
  in this graph and the edges conect these endpoints.
*/
bool ShockGraph::ComputeSGFromDDSGraph(sg::DDSGraph* sk)
{
   using namespace sg;

//	cerr << "labeling skeleton... " << flush;

   SGNode* pNode = new SGNode(GetNextIdx(), SHOCK_base);

   leda_node u = NewNode(pNode);

   DDSEdge* se = *sk->getEdges().begin();

   SGNode* pFirstNode = CopyNodeInfo(se, u, pNode);

   ConnectNodes(u, pNode, se, se->getN1());

   ConnectNodes(u, pFirstNode, se, se->getN2());

   //ShowGraph(*this);
   Insert4sAnd2s();

   DirectEdgesAcordingToTime();

   FinalizeSGConstruction();

   ComputeDerivedValues();

   Logger::Log ( "Computation for (" + std::string(GetObjName().c_str()) + " ,  " + to_string((_Longlong) GetViewNumber()) + ") is over!", constants::LogCore);

   return true;
}

/*!
   @brief Creates a shock graph from an image file or an image in memory.
   If imgInfo.pField == NULL, then an image file is read from imgInfo.strFileName.
*/
bool ShockGraph::Create(const ImageInfo& imgInfo, const ShockGraphParams& sgparams,
                        const SkeletalGraphParams& skelparams)
{
   Clear(); //make sure the current shock graph is empty

   m_compParams = sgparams;

   SetDAGLbl(imgInfo.strFileName);

   m_pSkeleton = new SkeletalGraph();

   m_pSkeleton->Create(imgInfo, skelparams, &m_dims);
   //m_pSkeleton->AssignBoundaryInfo(0);

   return ComputeSGFromDDSGraph(m_pSkeleton->GetDDSGraph());
}

/*!
   @brief Creates a shock graph from a given skeletal graph.

   The dimensions and graph label must be given, since they
   depend on the source image of the skeleton.
*/
bool ShockGraph::Create(SkeletalGraph* pSkeleton, const ShapeDims& dims,
                   String strLbl, const ShockGraphParams& sgparams)
{
   DAG::Clear();

   m_nLastIndexUsed = 0;

   m_compParams = sgparams;
   m_dims = dims;
   SetDAGLbl(strLbl);

   m_pSkeleton = pSkeleton;
   //m_pSkeleton->AssignBoundaryInfo(0);

   return ComputeSGFromDDSGraph(m_pSkeleton->GetDDSGraph());
}

/*!
   @brief The direction of branch in u with respect to the joint
   point with v.
*/
int ShockGraph::GetBranchDir(leda_node u, leda_node wrtV) const
{
   const ShockInfo& pt = GetJointPoint(u, wrtV);

   return GetSGNode(u)->GetBranchDir(pt == GetSGNode(u)->m_shocks[0] ?
      ShockBranch::FORWARD:ShockBranch::BACKWARD);
}

const ShockInfo& ShockGraph::GetJointPoint(leda_node u, leda_node v) const
{
   const SGNode* pNodeU = GetSGNode(u);
   const SGNode* pNodeV = GetSGNode(v);

   const ShockInfo& uStart = pNodeU->m_shocks[0];
   const ShockInfo& uEnd   = pNodeU->m_shocks.GetTail();

   const ShockInfo& vStart = pNodeV->m_shocks[0];
   const ShockInfo& vEnd   = pNodeV->m_shocks.GetTail();

   if (uStart == vStart || uStart == vEnd)
      return uStart;
   else if (uEnd == vStart || uEnd == vEnd)
      return uEnd;

   pNodeU->m_shocks.Print(cerr);
   pNodeV->m_shocks.Print(cerr);
   ASSERT(false);
   return vStart;
}

/*!
   @brief Checks that there is a grammatical rule for each node and its children.

   It traverses the tree (rooted DAG) in order. It stops atthe first rule violation.

   Note: subgraphs rooted at a node with multiple parents or with an ancestor
   with multiple parents whould be checked multiple times. While this is easy to fix,
   it turns out that in the SG grammar, those cases are trivial (subgraphs with a single node),
   and so it cheaper to checke them multiple times (at most two) than to keep track of visits.

   @param root is the root node of the shock graph
   @return leda_node nil if all rules are valid. o.w., first node (in depth-first order) with invalid rule.
*/
leda_node ShockGraph::CheckGrammar(leda_node root /* = nil */) const
{
   if (root == nil)
      root = GetFirstRootNode();

   if (GetRewriteRule(root) == INV_RULE)
      return root;

   leda_node child, invNode;

   forall_adj_nodes(child, root)
   {
      invNode = CheckGrammar(child);

      if (invNode != nil)
         return invNode;
   }

   return nil;
}

/*!
   @brief Checks that all adjacent nodes are of valid types and have exactly 1 in-edge

   @param r root node
   @param validType arrat of valid node tipes, ie, SHOCK_1, SHOCK_2, ...
   @param pInvJointPt (optinal) pointer to invalid joint point between r and it children.
   @return true if valid. false otherwise.
*/
bool ShockGraph::CheckChildNodeTypes(leda_node r, SmartArray<int> validTypes,
   const ShockInfo* pInvJointPt /* = NULL */) const
{
   int i, n = validTypes.GetTailIdx() + 1;
   leda_node c;
   bool righLbl;

   forall_adj_nodes(c, r)
   {
      if (indeg(c) > 1)
         return false;

      if (pInvJointPt != NULL && (*pInvJointPt) == GetJointPoint(r, c))
         return false;

      righLbl = false;

      for (i = 0; !righLbl && i < n; i++)
         if (NodeType(c) == validTypes[i])
            righLbl = true;

      if (!righLbl)
         return false;
   }

   return true;
}

/*!
   @brief Gets the generating rule for a node and its children, ie, the rewrite rule.

   Note: subgraphs rooted at a node with multiple parents or with an ancestor
   with multiple parents whould be checked multiple times. While this is easy to fix,
   it turns out that in the SG grammar, those cases are trivial (subgraphs with a single node),
   and so it cheaper to checke them multiple times (at most two) than to keep track of visits.
*/
GRAMMAR_RULE ShockGraph::GetRewriteRule(leda_node r) const
{
   SmartArray<int> validTypes(2); // don't need more than 2 elements

   if (outdeg(r) == 0)
   {
      switch (NodeType(r))
      {
         case SHOCK_4: return RULE_7;
         case SHOCK_3: return RULE_8;
         case SHOCK_2: return RULE_9;
         case SHOCK_1: return RULE_10;
      }
   }
   else
   {
      switch (NodeType(r))
      {
         case ROOT:
            validTypes.AddTail(SHOCK_3);
            validTypes.AddTail(SHOCK_4);
            return CheckChildNodeTypes(r, validTypes) ? RULE_1:INV_RULE;
         case SHOCK_4:
            validTypes.AddTail(SHOCK_1);
            return CheckChildNodeTypes(r, validTypes) ? RULE_2:INV_RULE;
         case SHOCK_3:
            {
               leda_node p1 = GetFirstParent(r);
               int p1Type = NodeType(p1);

               if (indeg(r) == 1 && (p1Type == SHOCK_1 || p1Type == ROOT))
               {
                  const ShockInfo* pPt = NULL;

                  if (p1Type == SHOCK_1)
                     pPt = &GetJointPoint(r, p1);

                  validTypes.AddTail(SHOCK_1);
                  return CheckChildNodeTypes(r, validTypes, pPt) ? RULE_3:INV_RULE;
               }
            }
            return INV_RULE;
         case SHOCK_1:
            if (outdeg(r) > 1)
            {
               validTypes.AddTail(SHOCK_1);
               if (CheckChildNodeTypes(r, validTypes))
                  return RULE_4;
            }
            else /* outdeg(r) == 1 */
            {
               leda_node c = GetFirstChild(r);
               leda_node p2 = GetFirstParent(c); // there is at least 1 parent
               int childInDeg = indeg(c);
               int childType = NodeType(c);

               if (childInDeg >= 2 && p2 == r)
                  p2 = GetSecondParent(c); // ensures p2 is second parent

               if (childType == SHOCK_1 && childInDeg == 1)
                  return RULE_4; // NEW RULE 4, allow chain of type 1s
               else if (childType == SHOCK_2 && childInDeg == 2 && NodeType(p2) == SHOCK_1)
                  return RULE_5;
               else if (childType == SHOCK_3 && childInDeg == 2 && NodeType(p2) == SHOCK_1 &&
                   GetJointPoint(r, c) != GetJointPoint(p2, c))
                  return RULE_6;
               else if (childType == SHOCK_3 && childInDeg == 1)
                  return RULE_6;
            }
            return INV_RULE;
      }
   }
   return INV_RULE;
}