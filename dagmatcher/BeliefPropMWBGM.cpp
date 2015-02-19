/*
C version of belief propagation b-matching.

Bert Huang (2006)
Send comments and questions to bert at cs.columbia.edu

Please contact Bert if you use this code for anything other than 
evaluation. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "BipartiteNodeGraph.h"

#define NEG_INF -1
#define MAX_ITER 10000
#define CONVERGENCE_INSURANCE 20

using namespace dml;

struct node_t;

typedef std::vector<node_t*> NodeArray;

struct node_t 
{
	
	int id;
	int neighborhood;              //number of neighbors (not counting itself)
	NodeArray neighbors;           //vector of pointers to neighbors
	std::vector<double> message;   //vector of messages from this node to all other nodes
	std::vector<double> phi;       //potential
	std::vector<int> beliefs;      //current top b node ids
};

node_t* initNode(int n, int id, int b, int total)
{
	int i;
	node_t* node = new node_t;

	node->id = id;
	node->neighborhood = n;

	node->message.resize(total);
	node->phi.resize(n);
	node->neighbors.resize(n);
	node->beliefs.resize(b);

	for (i = 0; i < total; i++)
		node->message[i] = 1;		

	for (i = 0; i < n; i++)
		node->phi[i] = 1;

	for (i = 0; i < b; i++)
		node->beliefs[i] = i;

	return node;
}

/*
	updates the messages stored in the given node
	by using messages from neighbors in opposite
*/
void updateMessages(node_t *node, int b)
{
	int i, j, minind, bth, counter;
	int n = node->neighborhood;

	std::vector<double> workvec(n);
	std::vector<int> topvals(b + 1);

	//compute work vector
	for (i = 0; i < n; i++) {
		workvec[i] = node->phi[i]*node->neighbors[i]->message[node->id];
	}

	//initialize topvals to be the first b+1 indexes
	minind = 0; bth = 0;

	for (i = 0; i < b + 1; i++) {
		topvals[i] = i;
		if (workvec[topvals[i]]<workvec[topvals[minind]])
			minind=i;
	}

	//find true topvals and true minind
	for (i = b + 1; i < n; i++) {
		if (workvec[i]>workvec[topvals[minind]]) {
			topvals[minind] = i;
			for (j=0; j<b+1; j++) {
				if (workvec[topvals[j]]<workvec[topvals[minind]])
					minind=j;
			}
		}
	}

	//find bth entry
	if (minind==0)
		bth=1;
	for (i = 0; i < b + 1; i++) {
		if (i!=minind && workvec[topvals[i]]<workvec[topvals[bth]])
			bth = i;
	}

	//update messages
	for (i = 0; i < n; i++) {
		if (workvec[i]>=workvec[topvals[bth]])
			node->message[node->neighbors[i]->id]=node->phi[i]/workvec[topvals[minind]];
		else
			node->message[node->neighbors[i]->id]=node->phi[i]/workvec[topvals[bth]];
	}

	//update beliefs
	counter=0;
	for (i = 0; i < b + 1; i++) {
		if (i!=minind) {
			node->beliefs[counter++] = node->neighbors[topvals[i]]->id;
		}
	}
}

/*
	checks if there is a valid bmatching in the beliefs
*/
int permCheck(NodeArray& nodes, SmartMatrix<double>& P, int n, int b) 
{
	int ret = 1, i, j;

	std::vector<int> counters(n);

	for (i = 0; i < n; i++)
		counters[i]=0;

	for (i = 0; i < n; i++) {
		for (j = 0; j < b; j++) {
			P[i][j] = nodes[i]->beliefs[j];
			counters[nodes[i]->beliefs[j]]++;
		}
	}

	for (i = 0; i < n; i++) 
		if (counters[i] != b)
			ret = 0;

	return ret;
}

/*BipartiteNodeGraph::NodeAssigmentList BipartiteNodeGraph::ComputeBeliefPropMWBM()
{
	SmartMatrix<double> P;
	NodeArray alpha, beta;

	ASSERT(A.size() == B.size());

	const int n = A.size();
	const int b = 1;

	P.Resize(n, b);

	alpha.resize(n);
	beta.resize(n);

	// Allocate node and initialize them
	leda::node_map<node_t*> alphaMap(*this), betaMap(*this);
	NodeSetIterator it;
	leda::edge e;
	leda::node v;
	node_t* pNode;
	int i;

	//initialize alpha nodes
	i = 0;
	forall_items(it, A)
	{ 
		v = A[it];
		alpha[i] = initNode(outdeg(v), i, b, n);
		alphaMap[v] = alpha[i];
		i++;
	}

	i = 0;
	forall_items(it, B)
	{ 
		v = B[it];
		beta[i] = initNode(indeg(v), i, b, n);
		betaMap[v] = beta[i];
		i++;
	}

	// Connect nodes
	forall_items(it, A)
	{
		v = A[it];
		pNode = alphaMap[v];
		i = 0;

		forall_out_edges(e, v)
		{
			pNode->neighbors[i] = betaMap[target(e)];
			pNode->phi[i]       = exp(edgeValues[e]);
			i++;
		}
	}

	forall_items(it, B)
	{
		v = B[it];
		pNode = betaMap[v];
		i = 0;

		forall_out_edges(e, v)
		{
			pNode->neighbors[i] = alphaMap[source(e)];
			pNode->phi[i]       = exp(edgeValues[e]);
			i++;
		}
	}

	// Start belief propagation
	int converged = CONVERGENCE_INSURANCE, iters = 0;

	while(converged > 0) {

		for (i = 0; i < n; i++) {
			updateMessages(beta[i], b);
		}
		for (i=0; i < n; i++) {
			updateMessages(alpha[i], b);
		}

		//printf("message update successful\n");
		//check for convergence
		converged -= permCheck(alpha, P, n, b);

		//printf("permcheck successful\n");

		if (++iters > MAX_ITER) {
			ShowMsg("Reached maximum iterations without converging");
			converged = 0;
		}			
	}

	NodeAssigmentList edgeList;

	// Copy the mapping to a list of edges
	leda::node u;
	i = 0;
	forall_items(it, A)
	{
		u = A[it];
		v = B[B.get_item(P[i][0])];

		forall_out_edges(e, u)
		{
			if (target(e) == v)
			{
				edgeList.push_back(e);
				break;
			}
		}

		i++;
	}

	// Delete nodes
	for (i = 0; i < n; i++)
		delete alpha[i];

	for (i = 0; i < n; i++)
		delete beta[i];

	return edgeList;
}*/
