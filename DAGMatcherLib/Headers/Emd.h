#ifndef _EMD_H_
#define _EMD_H_

#include "stdafx.h"

/*****************************************************************************/
#define MAX_SIG_SIZE   1000
#define MAX_ITERATIONS 500
//#define INFINITY       1e20
//#define EPSILON        1e-20
#define EPSILON        1e-10
/*****************************************************************************/

#define Matrix NEWMAT::Matrix
#define ColumnVector NEWMAT::ColumnVector
#define DiagonalMatrix NEWMAT::DiagonalMatrix

typedef ColumnVector * feature_t;

/*****************************************************************************/

typedef struct
{
  int n;                /* Number of features in the signature */
  feature_t *Features;  /* Pointer to the features vector */
  double *Weights;       /* Pointer to the weights of the features */
} signature_t;

typedef struct
{
  int from;             /* Feature number in signature 1 */
  int to;               /* Feature number in signature 2 */
  double amount;         /* Amount of flow from "from" to "to" */
} flow_t;

double emd(signature_t *Signature1, signature_t *Signature2,
	  double (*func)(feature_t *, feature_t *),
	  flow_t *Flow, int *FlowSize);

#endif
