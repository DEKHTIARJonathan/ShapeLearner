/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT) and Drexel University.
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
 * Author: M. Fatih Demirci and Diego Macrini
 *
 * $Log: CompareHistogram.cpp,v $
 *-----------------------------------------------------------------------*/

#include "CompareHistogram.h"
#include "Emd.h"
#include "BasicUtils.h"

/*!
 * @brief ...
 *
 * Big text
 *
 * @param hist_values1 ....
 * @param weights ...
 * @return normalized distance measure between histograms
 */
double CompareHistograms(const SmartArray<double> hist_values1, const SmartArray<double> weights1,
		                const SmartArray<double> hist_values2, const SmartArray<double> weights2){
 // variables to fill later.
 leda_graph G[2];
 leda_node_array <double> node_weight[2];
 //leda_edge_array <double> edge_weight[2];
 double emd_return;
 Matrix embedded[2];
 int ncols[2];
 int cdim[2];
 leda_node_array <int> column_map[2]; // a mapping of nodes into matrix columns.
 ColumnVector **gvec[2]; // column vectors corresponding to matrix columns.
 double *n_weight[2]; // normalized leda_node weights (to sum up to 1).
 //ColumnVector mu[2];
 int i, j, k;

 /*
 hist_values1.Print(std::cerr);
 weights1.Print(std::cerr);
 hist_values2.Print(std::cerr);
 weights2.Print(std::cerr);
 */

 // read in two histograms.
 for( i = 0; i < 2; i++ ) {
	 if (i == 0)
		 embedded[i] = GetGraph(G[i], node_weight[i], column_map[i], hist_values1, weights1);
	 else
		 embedded[i] = GetGraph(G[i], node_weight[i], column_map[i], hist_values2, weights2);

	 ncols[i] = embedded[i].Ncols();
	 cdim[i] = embedded[i].Nrows();

	 // copy weights into a column-weight array.
	 SmartArray<double> orig_weight(ncols[i]);
	 leda_node v;
	 forall_nodes( v, G[i] ) {
		 column_map[i][v] -= 1;
		 orig_weight[column_map[i][v]] = node_weight[i][v];
	 }

	 // normalize column-weights.
	 n_weight[i] = new double[ncols[i]];
	 normalize_weights( (double*)(const double*)orig_weight, ncols[i], n_weight[i] );

	 // copy the matrix into an array of column vectors -- for "emd.c".
	 gvec[i] = new ColumnVector*[ncols[i]];

	 for( j=0; j<ncols[i]; j++ )
		 gvec[i][j] = new ColumnVector( embedded[i].Column(j+1) );
 }

 	//int min_dim = 3;

	// set up structures for emd.
	signature_t s[2] = { {ncols[0], gvec[0], n_weight[0]},
		{ncols[1], gvec[1], n_weight[1]}};

	SmartArray<flow_t> flow(ncols[0]+ncols[1]-1);
	int flow_size;

	// set up structures for least squares.
	leda_d_array <int, ColumnVector *> X;
	leda_d_array <int, ColumnVector *> Y;
	leda_d_array <int, double> pair_weight;

	// set up emd/lset iteration.
	double old_emd_value = 999999.9;
	double curr_emd_value = old_emd_value - 1;

	while( curr_emd_value < old_emd_value ) {
		// call emd.
		old_emd_value = curr_emd_value;
		curr_emd_value = emd( s, s+1, vector_dist, (flow_t*)(const flow_t*)flow, &flow_size );
		emd_return = curr_emd_value;

		ASSERT(flow_size > 0);

		// set up LSET.
		X.clear();
		Y.clear();
		pair_weight.clear();

		for( k = 0; k < flow_size; k++ ) {
			X[k] = gvec[0][flow[k].from];
			Y[k] = gvec[1][flow[k].to];
			pair_weight[k] = flow[k].amount;
		}

		// call least squares.
		//double error = least_squares_estimation( X, Y, pair_weight )/flow_size;
	}

	// clean-up memory
	for( i = 0; i < 2; i++ ) {
		for( j = 0; j < ncols[i]; j++ ) {
			delete gvec[i][j];
		}
		delete[] gvec[i];
		delete[] n_weight[i];
	}

 return emd_return;
}

/**************************************************************************************/

Matrix GetGraph(leda_graph &G,leda_node_array <double> &node_weight, leda_node_array <int> &column_map,
                const SmartArray<double> hist_values, const SmartArray<double> weights){
 int number, i/*, mass_no=0*/;
 leda_node v;

 number = hist_values.GetSize();

 for (i=0; i<number; i++)      //Generating a leda_graph with n nodes and 0 edges.
       G.new_node();

  double* Node_Mass = new double[number];

  if (Node_Mass == NULL){
	  std::cerr << "Not enough memory !" << std::endl;
      exit(-1);
  }

  Matrix Vector_l2(3, G.number_of_nodes()); //There are only x,y,z coordinates

  for (i=0; i<number; i++){
          Node_Mass[i] = weights[i];
          Vector_l2(1,i+1) = hist_values[i];
          Vector_l2(2,i+1) = 0;
          Vector_l2(3,i+1) = 0;
  }

  leda_node_array<double> node_mass;
  leda_node_array<int> node_label;
  node_label.init(G);
  node_mass.init(G);
  node_weight.init(G);
  column_map.init(G);
  int in = 0;

  forall_nodes(v,G) {
    node_label[v] = in;
    node_mass[v] = Node_Mass[in];
    node_weight[v] = Node_Mass[in];
    column_map[v] = in + 1;
    in++;
  }

  delete[] Node_Mass;

  return Vector_l2;
}

// distance defined on pairs of column vectors.
double vector_dist( ColumnVector **v1, ColumnVector **v2 ) {
        return (double) sqrt( (*(*v1) - *(*v2)).SumSquare() );
}

// normalizing weights to sum up to 1.
void normalize_weights( double weight[], int nv, double normed_weight[] ) {
        // compute sum of weights and normalize them.
        double weight_sum = 0;
		int i;

        for( i=0; i<nv; i++ )
                weight_sum += weight[i];

        ASSERT(weight_sum > 0);

        for( i=0; i<nv; i++ )
                normed_weight[i] = weight[i]/weight_sum;
}

//  "normalizing" a set of column vectors by finding their weighted average,
//  shifting column-vectors to make average 0, and dividing by the deviation.
/** @exception precondition
                normed_weight contains normalized weights.
*/
void normalize_column_vectors( ColumnVector *mcol[], int nc,
                double normed_weight[], ColumnVector &mu, double &sigma ) {
/*        int nr = (*mcol[0]).Nrows();

        // compute mu.
        mu = ColumnVector(nr);
        mu = 0.0;
        for( int i=0; i<nc; i++ )
                mu += normed_weight[i] * (*mcol[i]);

        // subtract mu from everything.
        for( int i=0; i<nc; i++ )
                *mcol[i] -= mu;

        // compute sigma.
        sigma = 0;
        for( int i=0; i<nc; i++ )
                sigma += normed_weight[i] * ((*mcol[i]).SumSquare());
        sigma = sqrt(sigma);

        ASSERT(sigma > 0);

        // normalize by sigma.
        for( int i=0; i<nc; i++ )
                (*mcol[i]) /= sigma;

#ifdef _DEBUG_
        std::cerr << "computed sigma: " << sigma << std::endl;
        std::cerr << "computed mu: " << mu.t();
#endif*/
	WARNING(true, "Code is commented out");
	ASSERT(false);
}

//  Projecting X's onto Y's using linear least squares estimate.
/** @exception precondition
                X and Y are assumed to be normalized (0 average, deviation 1).
                normed_weight's are assumed to be normalized (sum 1).
*/
double least_squares_estimation(
                leda_d_array <int, ColumnVector *> &X, leda_d_array <int, ColumnVector *> &Y,
                leda_d_array <int, double> &normed_weight ) {
        int ncols = X.size();
        int nrows = X[0]->Nrows();

#ifdef _DEBUG_
        // compute initial error (debug).
        double init_error = 0;
        for( int i=0; i<ncols; i++ ) {
                init_error += (double) normed_weight[i]*sqrt( ((*X[i])-(*Y[i])).SumSquare() );
        }
        std::cerr << "initial error in LSET: " << init_error << std::endl;
#endif

        // compute Sigma_XY.
        Matrix Sigma_XY( nrows, nrows );
        Sigma_XY = 0.0;

        /*for( int i=0; i<ncols; i++ ) {
                Sigma_XY += (normed_weight[i]*(*Y[i]))*(*X[i]).t();
        }*/
	WARNING(true, "Code is commented out");
	ASSERT(false);

        //std::cerr << "Sigma" << std::endl;
        //std::cerr << Sigma_XY << std::endl;
        //bool noNaNs = true;
		int i, j;

        for (i = 1; i <= Sigma_XY.Nrows(); i++)
					for (j = 1; j <= Sigma_XY.Ncols(); j++)
						ASSERT(!isnan(Sigma_XY(i, j)));

        // compute SVD.
        Matrix U(nrows, nrows), V(nrows, nrows);
        DiagonalMatrix D(nrows);
		NEWMAT::SVD( Sigma_XY, D, U, V );

        // compute R.
        Matrix R = U * V.t();

        // update X's. Since they are represented by pointers,
        // need to update each pointer's value once - use a leda_d_array to check it.
        leda_d_array <long, ColumnVector *> ptr_map;
        for( i=0; i<ncols; i++ ) {
                if( !ptr_map.defined( (long) X[i] ) ) {
                        *X[i] = R * (*X[i]);
                        ptr_map[(long) X[i]] = X[i];
                }
        }

        // return the error of the estimate.
        double error = 0;
        for( i=0; i<ncols; i++ ) {
                error += (double) normed_weight[i]*sqrt( ((*X[i])-(*Y[i])).SumSquare() );
        }

#ifdef _DEBUG_
        std::cerr << "final error in LSET: " << error << std::endl;
#endif

        return error;
}