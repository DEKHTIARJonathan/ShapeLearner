@q % -*- mode: web; mode: cweb; -*-     @>
@q %    WEB SYSTEM  :  cweb             @>
@q %       PROGRAM  :  Curve.w          @>
@q %        AUTHOR  :  Pavel Dimitrov   @>
@q  %        [pdimit@@cantor.McRCIM.McGill.EDU] @>
@q % CREATION DATE  :  Mon May 27 16:26:20 2002 @>
@q %% @>


@*1 Curve (Interface).
This class is the basis for all curve representations. It is an {\it
abstract} class; that is, it is only an interface (ain't C++
great!). 

@(Curve.hpp@>=

#ifndef Curve_HPP
#define Curve_HPP
@<Curve includes@>@;

namespace sg {
@<Curve class definition@>
}
#endif

@ @<Curve includes@>=
#include "Point.hpp"
#include "Vector.hpp"

@ @<Curve class definition@>=
class Curve {
      protected:@;

        bool closed;
	double length;
	
      public:@;

	bool isClosed() {return closed;}

	double length() {return length;}

	// Virtual methods
	virtual double distToPt(Point pt);
	virtual Point atT(double t);
        virtual Vector normal(double t);
        virtual Curve subCurve(double t0, double t1);
        virtual Point pointClosestToPt(Point p);
};


@q@>