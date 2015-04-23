#ifndef _BASIC_TYPEDEFS_H_
#define _BASIC_TYPEDEFS_H_

#include "stdafx.h"

namespace dml {
// Define a smart pointer type for DAGs
class DAG;
typedef SmartPtr<DAG> DAGPtr; //!< smart pointer for DAGs

// Define a smart pointer type for DAG nodes
class DAGNode;
typedef SmartPtr<DAGNode> DAGNodePtr; //!< smart pointer DAG nodes

// Define a smart pointer type for DAG edges
class DAGEdge;
typedef SmartPtr<DAGEdge> DAGEdgePtr; //!< smart pointer type for DAG edges

// Define a shared pointer type for NodeMatchInfo objects
//class NodeMatchInfo;
//typedef SharedPtr<NodeMatchInfo> NodeMatchInfoPtr; //!< shared pointer for node assignments

typedef leda::list<leda::edge> EdgeList;
typedef leda::d_array<leda::node, leda::node> DynamicNodeMap;
typedef leda::d_array<leda::node, int> NodeIndexMap;
typedef leda::d_array<DAGNodePtr, DAGNodePtr> DAGNodePtrMap;
typedef leda::d_array<DAGNodePtr, double> DAGNodeSimilarityMap;

typedef leda::node_array<leda::node> NodeToNodeArray;

// Types that are not in used anymore

//typedef leda::list<leda::node> NodeList;
//typedef leda::d_array<leda::edge, leda::node> EdgeNodeMap;
//typedef leda::d_array<leda::node, leda::list_item> NodeListItemMap;
//typedef leda::list<DAGNodePtr> DAGNodeList;
} // namespace dml

#endif //_BASIC_TYPEDEFS_H_
