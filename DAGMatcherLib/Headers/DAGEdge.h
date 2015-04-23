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
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 *-----------------------------------------------------------------------*/

#ifndef __DAGEDGE_H__
#define __DAGEDGE_H__

#include "stdafx.h"

#define DEFAULT_DAG_EDGE_WEIGHT 1.0

namespace dml {
typedef dml::String EDGE_LABEL;

/*!
	\brief This class represents a edge of an object of class DAG.

	DAGEdge is an abstract class intended to provide the basic functionality of
	a DAG node and to specify which functions must be overloaded (the pure virtual ones).

	/see DAG and DAGEdgePtr.
*/
class DAGEdge
{
protected:
	double weight;

public:

	DAGEdge(const double& w = DEFAULT_DAG_EDGE_WEIGHT)
	{
		weight = w;
	}

	virtual ~DAGEdge()
	{
	}

	const double& GetWeight() const
	{
		return weight;
	}

	void SetWeight(const double& w)
	{
		weight = w;
	}

	virtual void Clear()
	{
		weight = DEFAULT_DAG_EDGE_WEIGHT;
	}

	//! Computes derived values wrt the source and target nodes of e
	virtual void ComputeDerivedValues(leda::edge e)
	{
		// there is nothing to do
	}

	virtual void Print(std::ostream& os = std::cout, bool bXMLFormat = false) const
	{
		if (bXMLFormat)
		{
			XML_PRINT(os, "weight", weight);
		}
		else
		{
			PRINT(os, weight);
		}
	}

	//! Returns true iff the edge has label to display
	virtual bool HasLblForGraph() const
	{
		return false;
	}

	//! Returns the label that will be displayed in the graph
	virtual EDGE_LABEL GetLblForGraph(bool bExtraInfo = true) const
	{
		return int(weight);
	}

	//! Returns the color that is use to visualize the edge
	virtual leda::color GetColorForGraph() const
	{
		return leda::black;
	}

	/*!
		@brief Default clone function. Do not call from the derived class!
		This function is required by the SmartPtr class.
	*/
	virtual DAGEdge* CreateObject() const
	{
		return new DAGEdge();
	}

	virtual std::istream& Read(std::istream& is)
	{
		is.read((char*) &weight, sizeof(weight));
		return is;
	}

	virtual std::ostream& Write(std::ostream& os) const
	{
		os.write((char*) &weight, sizeof(weight));
		return os;
	}

	friend std::ostream& operator<<(std::ostream &os, const DAGEdge& edge)
	{
		return edge.Write(os);
	}

	friend std::istream& operator>>(std::istream &is, DAGEdge& edge)
	{
		return edge.Read(is);
	}
};
} //namespace dml

#endif //__DAGEDGE_H__
