/**************************************************************************

   File:                ShapeMaker.h

   Author(s):           Pavel Dimitrov

   Created:             22 Jun 2002

   Last Revision:       $Date: 2002/06/27 14:14:40 $

   Description: Abstract class. Shape making objects create Shape's
                from user sources. This is the "input" to the
		library. The idea is that the user would implement the
		ShapeMaker (and possible Shape) they need for the
		specific application.

   $Revision: 1.2 $

   $Log: ShapeMaker.h,v $
   Revision 1.2  2002/06/27 14:14:40  pdimit
   Just updated the descriptions in the header files.

   Revision 1.1.1.1  2002/06/23 05:42:08  pdimit
   Initial import


	
   Copyright (c) 2002 by Pavel Dimitrov, Centre for Intelligent Machines,
   McGill University, Montreal, QC.  Please see the copyright notice
   included in this distribution for full details.

 **************************************************************************/

#ifndef SHAPEMAKER_H
#define SHAPEMAKER_H

#include "defines.h"
#include "Shape.h"

namespace sg{

  // abstract class
  class ShapeMaker {

  public:

    virtual ~ShapeMaker() {}

    virtual Shape *getShape(bool foreground=false) = 0;
  };

}

#endif  /* SHAPEMAKER_H */
