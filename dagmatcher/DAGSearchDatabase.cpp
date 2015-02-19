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
 * Author: Maxim Trokhimtchouk (trokhim@cs.utoronto.ca)
 *
 * $Log: DAGSearchDatabase.cpp,v $
 * Revision 1.2  2002/05/16 22:49:29  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#include "DAGSearchDatabase.h"
#include "BasicUtils.h"

using namespace dml;

DAGSearchDatabase::DAGSearchDatabase()
{
	nnd.name = "";
	nnd.data = HnSRTreeFile::null;
	nnd.dimension = 0;
}

DAGSearchDatabase::~DAGSearchDatabase()
{
  if(nnd.data!=HnSRTreeFile::null)
    {
      nnd.data.close();
    }
}

void DAGSearchDatabase::Add(ID id, const DAG& dag)
{
	leda_node v;
	DAGSearchRecord record;
	TSV h;
	HnPoint pn;

	//Check if database has been activated
	if(nnd.data != HnSRTreeFile::null)
	{
		record.id = id;
		record.offset = dag.GetFileOffset();
		record.totalVote = dag.GetTotalTSVSum();

		//for each node in the dag class record its TSV to the NND
		forall_nodes(v, dag)
	    {
			if (dag.GetNodeTSVNorm(v) > 0)
			{
				record.node = dag.GetNodeIndex(v);
				record.nodeLbl = dag.NodeType(v);
				h = dag.GetNodeTSV(v, nnd.dimension);
				pn = TSVtoHnPoint(h);
				nnd.data.store(pn, new_HnDataItem((void*)(&record),sizeof(DAGSearchRecord)));
	    	}
		}
	}
}

void  DAGSearchDatabase::Remove(ID id, DAG& dag)
{
	leda_node v;
	TSV h;
	HnPoint pn;

	DAGSearchRecord record;
	record.id = id;

	//Check if database has been activated
	if(nnd.data!=HnSRTreeFile::null)
	{
	  forall_nodes(v, dag)
	  {
		  record.node = dag.GetNodeIndex(v);
		  h = dag.GetNodeTSV(v, nnd.dimension);
		  pn = TSVtoHnPoint(h);

		  // I am not sure if I should allocate mdemory for each search record
		  nnd.data.remove(pn, new_HnDataItem((void*)(&record),sizeof(DAGSearchRecord)));
	  }
	}
}

/*!
	@brief Executes nearest-neighbour search.

	The `numNeighbors (k)' is not necessarily the maximum number of points to be obtained.
	If the **farthest point** of a result set has multiple points at the same rank,
	they are also included in the result set.
*/
SmartArray<DAGSearchRecordEx> DAGSearchDatabase::GetKClosest(const DAG& dag, const leda_node& querynode, int k)
{
	SmartArray<DAGSearchRecordEx> closestDAGs;

	//Check if database has been activated
	if(nnd.data != HnSRTreeFile::null && k >= 1)
	{
		int i, size;
		HnPoint pp;
		TSV tsv;
		DAGSearchRecordEx sr;

		HnPointVector points;
		HnDataItemVector dataItems;

		tsv = dag.GetNodeTSV(querynode, nnd.dimension);

		pp = TSVtoHnPoint(tsv);

		nnd.data.getNeighbors(pp, k, &points, &dataItems);

		closestDAGs.Resize(size = dataItems.size());

		for(i = 0; i < size; i++)
		{
			sr = *(DAGSearchRecord*)(dataItems.elementAt(i).toCharArray());
			sr.nodeTSV = HnPointToTSV(points.elementAt(i));
			closestDAGs.Add(sr);
		}
	}

	return closestDAGs;
}

SmartArray<DAGSearchRecordEx> DAGSearchDatabase::GetClosest(const DAG& dag, const leda_node& querynode,
															const double& range)
{
	SmartArray<DAGSearchRecordEx> closestDAGs;

	ASSERT_UNIT_INTERVAL(range);

	//Check if database has been activated
	if(nnd.data != HnSRTreeFile::null)
	{
		int i, size;
		double delta, val;
		HnRect rect;
		TSV tsv;
		DAGSearchRecordEx sr;

		HnPointVector points;
		HnDataItemVector dataItems;

		tsv = dag.GetNodeTSV(querynode, nnd.dimension);
		rect = new_HnRect(nnd.dimension);

		for(i = 0; i < tsv.GetSize(); i++)
		{
			val = tsv[i];
			delta = val * range;
			rect.setRangeAt(val - delta, val + delta, i);
		}

		nnd.data.getInRect(rect, &points, &dataItems);

		closestDAGs.Resize(size = dataItems.size());

		for(i = 0; i < size; i++)
		{
			sr = *(DAGSearchRecord*)(dataItems.elementAt(i).toCharArray());
			sr.nodeTSV = HnPointToTSV(points.elementAt(i));
			closestDAGs.Add(sr);
		}
	}

	return closestDAGs;
}

bool DAGSearchDatabase::Create(std::string name, int dim)
{
    //check if the dimensions are larger or equal to the minimal dimensions allowed
    if(dim < Min_NND_Dimension) return false;

    //Check if database has been activated
	if(nnd.data==HnSRTreeFile::null)
	{
	  //create the near neighbor database
	  nnd.data = new_HnSRTreeFile(name.c_str(), dim, sizeof(DAGSearchRecord), HnProperties::null);

	  //check if the database was created successful
	  if(nnd.data == HnSRTreeFile::null)
	    {
	      //return false if an error occured
	      return false;
	    }
	  else
	    {
	      nnd.name = name;
	      nnd.dimension = dim;
	    }
	}
	else
	{
	  return false;
	}
	return true;
}

bool DAGSearchDatabase::Open(std::string name)
{
  //close the indexing database
  if(nnd.data!=HnSRTreeFile::null)
  {
    nnd.data.close();
  }

  nnd.data = new_HnSRTreeFile(name.c_str(), "rw");
  if(nnd.data == HnSRTreeFile::null) return false;

  nnd.name = name;
  nnd.dimension = nnd.data.getDimension();
  return true;
}

void DAGSearchDatabase::Close()
{
	nnd.data.close();
	nnd.data = HnSRTreeFile::null;
}

std::istream&  DAGSearchDatabase::Read(std::istream& is)
{
  char* namestr;
  size_t t;
  std::string name;
  int dim;

  //close the indexing database
  if(nnd.data!=HnSRTreeFile::null)
  {
    nnd.data.close();
  }

  is.read((char*)&t,sizeof(t));
  namestr = new char[t];

  is.read(namestr, t);
  name = std::string(namestr);
  delete [] namestr;

  is.read((char*)&dim,sizeof(dim));

  //load the near neighbor database
  nnd.data = new_HnSRTreeFile(name.c_str(), "rw");

  //check if the database was created successfuly
  if(nnd.data == HnSRTreeFile::null)
    {
      //if error occured set stream state to fail and return it
      is.setstate(std::ios::failbit);
      return is;
    }

     nnd.name = name;
     nnd.dimension = dim;

     return is;
}

std::ostream&  DAGSearchDatabase::Write(std::ostream& os)
{
  size_t t;

  t = nnd.name.length() + 1;

  os.write((char*)&t,sizeof(t));
  os.write(nnd.name.c_str(), t);
  os.write((char*)&nnd.dimension,sizeof(nnd.dimension));

  return os;
}

HnPoint DAGSearchDatabase::TSVtoHnPoint(const TSV& tsv)
{
	int d = tsv.GetSize();
	HnPoint hp = new_HnPoint(d);

	for(int i = 0; i < d; i++)
		hp.setCoordAt(tsv[i], i);

	return hp;
}

TSV DAGSearchDatabase::HnPointToTSV(const HnPoint& pt)
{
	int d = pt.getDimension();
	TSV tsv(d);

	for(int i = 0; i < d; i++)
		tsv[i] = pt.getCoordAt(i);

	return tsv;
}