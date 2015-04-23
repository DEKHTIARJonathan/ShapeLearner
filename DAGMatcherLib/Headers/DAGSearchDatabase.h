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
 * $Log: DAGSearchDatabase.h,v $
 * Revision 1.2  2002/05/16 22:49:29  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef _DAG_SEARCH_DATABASE_H_
#define _DAG_SEARCH_DATABASE_H_

#include "stdafx.h"

namespace dml {
///Type used as an identification type through out the system
typedef int ID;

///Defines a constant that identifies nonexistant element
const ID NULLID = -1;

///This constant defines the minimal dimention for the near neigbour search database.
const int  Min_NND_Dimension = 1;

///DAGSearchRecord structure
/*
*This structure is used to contain data in the near neighbour search database
*/
struct DAGSearchRecord
{
	ID id; 		///<Identification number of the model to which the node belongs to.
	ID offset;  ///<Offset of the model in the database
	ID node;    ///<ID of the node, TSV of which is contained in NND

	double totalVote;
	int nodeLbl;
};

struct DAGSearchRecordEx : public DAGSearchRecord
{
	TSV nodeTSV;

	void operator=(const DAGSearchRecord& rhs)
	{
		id = rhs.id;
		offset = rhs.offset;
		node = rhs.node;
		totalVote = rhs.totalVote;
		nodeLbl = rhs.nodeLbl;
	}
};

///Near neighbour search database used to index the DAG prototypes
/*
*This an encupsulating class for a near neigbour search database.
*It defines the interface for functions required by the indexer and
*implements them using an adopted near neighbour search database.
*/
class DAGSearchDatabase
{
	///Near neigbour database data
	/**
	*This structure contains the instance of the NND class,
	*as well as the name of a file in which the database is stored.
	*This structure also stores the number of dimensions in NND.
	*/
	class NND
	{
	public:
		HnSRTreeFile data; ///<Instance of the actual NND class.
		std::string name; ///<Name of the database file.
		int dimension; ///<Number of dimensions in the database
	} nnd;

public:
	///Default constructor.
	DAGSearchDatabase();

	///Destructor.
	virtual ~DAGSearchDatabase();

	///Adds prototype.
	 /**
	 *Adds the prototype to the database.
	 *@pre No error checking is performed.
	 *@param prototype contains identification number of the
	 *@param prototype prototype view described which is to be added to NND.
	 *@param dag refers to the instance of the DAG class
	 *@param dag that describes the prototype view to be addded to the database.
	 *@return No return values.
	 */
	 void Add(ID id, const DAG& dag);

	///Removes prototype.
	/**
	*Removes the prototype from the search database.
	 *@pre No error checking is performed.
	 *@param prototype contains identification number
	 *of the prototype view which is to be removed from NND.
	 *@param dag refers to the instance of the DAG class that
	 *describes the prototype view to be removed from the database.
	 *@return No return values.
	*/
	void Remove(ID id, DAG& dag);

	///Finds k closest nodes.
	/**
	*Finds k closest nodes to the query node.
	*@param querynode refers to the node for which n closest
	* nodes will be found in the database.
	*@param k contains number of neighbours to be retrieved during the search.
	*/
	SmartArray<DAGSearchRecordEx> GetKClosest(const DAG& dag, const leda_node& querynode, int k);

	///Finds closest nodes in a given range.
	/**
	*Finds the closest nodes to the quety node in the given range.
	*@param querynode refers to the node for which n closest
	* nodes will be found in the database. e.g. 0.25
	*@param range tolarance of deviation at every dimension.
	*/
	SmartArray<DAGSearchRecordEx> GetClosest(const DAG& dag, const leda_node& querynode, const double& range);

	///Creates the database.
	/**
	*Creates the database with the file name 'name' and dimensions 'dim'.
	*@pre name must be a valid file name.
	*@param name contains file name of the database to be created.
	*@param dim is the number of dimensions of the new database.
	*@return Returns true if database was created successfully, false otherwise.
	*/
	bool Create(std::string name, int dim);

	///Opens the database.
	/**
	*Opens the database with the file name 'name'.
	*@pre name must be a valid file name.
	*@param name contains file name of the database to be created.
	*@return Returns true if database was opened successfully, false otherwise.
	*/
	bool Open(std::string name);

	///Read the database from the stream.
	/**
	*Reads database parameters from a stream and loads the database.
	*@param is refers to a stream which is used to load the database.
	*@return Returns reference to a stream used to load the database.
	*@return If the error occured during an attempt to load the database,
	*@return state of the stream will be set to fail.
	*/
	std::istream& Read(std::istream& is);

	///Writes the database from a stream
	/**
	*Writes database parameters to a stream. This parameters can latter
	*be used to read the database
	*@param os refers to a stream which is used to write the database parameters.
	*@return Returns reference to a stream used to write the database parameters.
	*/
	std::ostream& Write(std::ostream& os);

	void Close();

	int GetDim() const { return nnd.dimension; }

private:
  static HnPoint TSVtoHnPoint(const TSV& tsv);
  static TSV HnPointToTSV(const HnPoint& pt);
};
} //namespace dml

#endif //_DAG_SEARCH_DATABASE_H_
