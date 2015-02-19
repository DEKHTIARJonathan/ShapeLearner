//$$ newmat9.cpp         Input and output

// Copyright (C) 1991,2,3,4: R B Davies


#define WANT_STREAM

#include "include.h"

#include "newmat.h"
#include "newmatio.h"
#include "newmatrc.h"

#ifdef use_namespace
namespace NEWMAT {
#endif


#ifdef DO_REPORT
#define REPORT { static ExeCounter ExeCount(__LINE__,9); ++ExeCount; }
#else
#define REPORT {}
#endif

// for G++ 3.01
#ifndef ios_format_flags
#define ios_format_flags long
#endif

std::ostream& operator<<(std::ostream& s, const BaseMatrix& X)
{
   GeneralMatrix* gm = ((BaseMatrix&)X).Evaluate(); operator<<(s, *gm);
   gm->tDelete(); return s;
}


std::ostream& operator<<(std::ostream& s, const GeneralMatrix& X)
{
   MatrixRow mr((GeneralMatrix*)&X, LoadOnEntry);
   int w = s.width();
   int nr = X.Nrows();

#ifdef WIN32
   ios_format_flags f = s.flags();
#else
   ios_format_flags f = s.flags();
#endif
   
   s.setf(std::ios::fixed, std::ios::floatfield);
   for (int i=1; i<=nr; i++)
   {
      int skip = mr.skip;  int storage = mr.storage;
      Real* store = mr.data;  skip *= w+1;
      while (skip--) s << " ";
      while (storage--) { s.width(w); s << *store++ << " "; }
//      while (storage--) s << setw(w) << *store++ << " ";
      mr.Next();  s << "\n";
   }
   s << std::flush;  s.flags(f); return s;
}

// include this stuff if you are using an old version of G++
// with an incomplete io library

/*

std::ostream& operator<<(std::ostream& os, Omanip_precision i)
   { os.precision(i.x); return os; }

Omanip_precision setprecision(int i) { return Omanip_precision(i); }

std::ostream& operator<<(std::ostream& os, Omanip_width i)
   { os.width(i.x); return os; }

Omanip_width setw(int i) { return Omanip_width(i); }

*/

#ifdef use_namespace
}
#endif


