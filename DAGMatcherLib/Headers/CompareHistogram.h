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
 * $Log: CompareHistogram.h,v $
 *-----------------------------------------------------------------------*/

#include "stdafx.h"

#define Matrix NEWMAT::Matrix
#define ColumnVector NEWMAT::ColumnVector
#define DiagonalMatrix NEWMAT::DiagonalMatrix

/*!
 * @brief ...
 *
 * Big text
 *
 * @param hist_values1 ....
 * @param weights ...
 * @return normalized distance measure between histograms
 */
double CompareHistograms(const SmartArray<double>, const SmartArray<double>,
                                const SmartArray<double>, const SmartArray<double>);

Matrix GetGraph(leda_graph&, leda_node_array<double>&, leda_node_array<int>&,
                const SmartArray<double>, const SmartArray<double>);

/** Distance defined on pairs of column vectors.
        @param v1 -- first vector.
        @param v2 -- second vector.
        @return L_2 distance between the vectors. */
double vector_dist( ColumnVector **v1, ColumnVector **v2 );

/** Normalizing weights to sum up to 1.
        @param weight -- the array of weights.
        @param nv -- number of weights in the array.
        @param normed_weight -- the array of normalized weights. */
void normalize_weights( double weight[], int nv, double normed_weight[] );

/** "normalizing" matrix by finding weighted average of its column-vectors,
        shifting column-vectors to make average 0, and dividing by the deviation.
        @param mcol -- the array of vectors to be replaced with normalized version.
        @param nc -- the number of pointers to vectors in the array.
        @param normed_weight -- the normalized weights attached to column-vectors.
        @param mu -- the average computed.
        @param sigma - the deviation computed. */
void normalize_column_vectors( ColumnVector *mcol[], int nc,
                double normed_weight[], ColumnVector &mu, double &sigma );

/** Projecting X's onto Y's using linear least squares estimate.
        @param X -- one set of vectors.  Changed into the LSET.
        @param Y -- another set of vectors.  Both of the same dimension.
        @param weight -- weights of pairings, assumed to be normalized.
        @return the error of the approximation. */
double least_squares_estimation(
                leda_d_array <int, ColumnVector *> &X, leda_d_array <int, ColumnVector *> &Y,
                leda_d_array <int, double> &normed_weight );
