/**************************************************************************

   File:                spline_smooth.h

   Author(s):           Pavel Dimitrov

   Created:             11 Jun 2002

   Last Revision:       $Date: 2002/06/26 04:32:11 $

   Description: 

   $Revision: 1.1 $

   $Log: spline_smooth.h,v $
   Revision 1.1  2002/06/26 04:32:11  pdimit
   After the failed attempt at getting the spline smoothing to work


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef SPLINE_SMOOTH_H
#  define SPLINE_SMOOTH_H

/**********************
     Include Files
 **********************/

/**********************
     Public Defines
 **********************/

/**********************
      Public Types
 **********************/

/********************
   Public Variables 
 ********************/

#include <stdlib.h>
#include <math.h>

#define SQR(x) ((x)*(x))

//define the structure to contain x,y and z components of the velocity/position of filament nodes for the Biot-Savart integeral 
typedef struct coordinates
{
  float xcord;
  float ycord;
  float zcord;
}Coordinates;


/***************************************************************************************  
Points contains the array, n is the length of the array, and
wight is the smoothing -- the closer to infinity, the tighter 
to the original the result 
***************************************************************************************/
void Periodic_Smoothing_Spline(Coordinates *Points,
			       int n, // the number of nodes
			       float weight);


#endif  /* SPLINE_SMOOTH_H */
