#define WANT_STREAM
#define WANT_TIME

#include "include.h"

#include "newmat.h"

#include "tmt.h"

#ifdef use_namespace
//using namespace NEWMAT;
namespace NEWMAT {
#endif


/**************************** test program ******************************/


class PrintCounter
{
   int count;
   const char* s;
public:
   ~PrintCounter();
   PrintCounter(const char * sx) : count(0), s(sx) {}
   void operator++() { count++; }
};

PrintCounter PCZ("Number of non-zero matrices (should be 1) = ");
PrintCounter PCN("Number of matrices tested                 = ");

PrintCounter::~PrintCounter()
{ std::cout << s << count << "\n"; }


void Print(const Matrix& X)
{
   ++PCN;
   std::cout << "\nMatrix type: " << X.Type().Value() << " (";
   std::cout << X.Nrows() << ", ";
   std::cout << X.Ncols() << ")\n\n";
   if (X.IsZero()) { std::cout << "All elements are zero\n" << std::flush; return; }
   int nr=X.Nrows(); int nc=X.Ncols();
   for (int i=1; i<=nr; i++)
   {
      for (int j=1; j<=nc; j++)  std::cout << X(i,j) << "\t";
      std::cout << "\n";
   }
   std::cout << std::flush; ++PCZ;
}

void Print(const UpperTriangularMatrix& X)
{
   ++PCN;
   std::cout << "\nMatrix type: " << X.Type().Value() << " (";
   std::cout << X.Nrows() << ", ";
   std::cout << X.Ncols() << ")\n\n";
   if (X.IsZero()) { std::cout << "All elements are zero\n" << std::flush; return; }
   int nr=X.Nrows(); int nc=X.Ncols();
   for (int i=1; i<=nr; i++)
   {
      int j;
      for (j=1; j<i; j++) std::cout << "\t";
      for (j=i; j<=nc; j++)  std::cout << X(i,j) << "\t";
      std::cout << "\n";
   }
   std::cout << std::flush; ++PCZ;
}

void Print(const DiagonalMatrix& X)
{
   ++PCN;
   std::cout << "\nMatrix type: " << X.Type().Value() << " (";
   std::cout << X.Nrows() << ", ";
   std::cout << X.Ncols() << ")\n\n";
   if (X.IsZero()) { std::cout << "All elements are zero\n" << std::flush; return; }
   int nr=X.Nrows(); int nc=X.Ncols();
   for (int i=1; i<=nr; i++)
   {
      for (int j=1; j<i; j++) std::cout << "\t";
      if (i<=nc) std::cout << X(i,i) << "\t";
      std::cout << "\n";
   }
   std::cout << std::flush; ++PCZ;
}

void Print(const SymmetricMatrix& X)
{
   ++PCN;
   std::cout << "\nMatrix type: " << X.Type().Value() << " (";
   std::cout << X.Nrows() << ", ";
   std::cout << X.Ncols() << ")\n\n";
   if (X.IsZero()) { std::cout << "All elements are zero\n" << std::flush; return; }
   int nr=X.Nrows(); int nc=X.Ncols();
   for (int i=1; i<=nr; i++)
   {
      int j;
      for (j=1; j<i; j++) std::cout << X(j,i) << "\t";
      for (j=i; j<=nc; j++)  std::cout << X(i,j) << "\t";
      std::cout << "\n";
   }
   std::cout << std::flush; ++PCZ;
}

void Print(const LowerTriangularMatrix& X)
{
   ++PCN;
   std::cout << "\nMatrix type: " << X.Type().Value() << " (";
   std::cout << X.Nrows() << ", ";
   std::cout << X.Ncols() << ")\n\n";
   if (X.IsZero()) { std::cout << "All elements are zero\n" << std::flush; return; }
   int nr=X.Nrows();
   for (int i=1; i<=nr; i++)
   {
      for (int j=1; j<=i; j++) std::cout << X(i,j) << "\t";
      std::cout << "\n";
   }
   std::cout << std::flush; ++PCZ;
}


void Clean(Matrix& A, Real c)
{
   int nr = A.Nrows(); int nc = A.Ncols();
   for (int i=1; i<=nr; i++)
   {
      for (int j=1; j<=nc; j++)
      { Real a = A(i,j); if ((a < c) && (a > -c)) A(i,j) = 0.0; }
   }
}

void Clean(DiagonalMatrix& A, Real c)
{
   int nr = A.Nrows();
   for (int i=1; i<=nr; i++)
   { Real a = A(i,i); if ((a < c) && (a > -c)) A(i,i) = 0.0; }
}

void PentiumCheck(Real N, Real D)
{
   Real R = N / D;
   R = R * D - N;
   if ( R > 1 || R < -1)
      std::cout << "Pentium error detected: % error = " << 100 * R / N << "\n";
}

#ifdef use_namespace
}
using namespace NEWMAT;
#endif


//*************************** main program **********************************

void TestTypeAdd();                            // test +
void TestTypeMult();                           // test *
void TestTypeConcat();                         // test |
void TestTypeSP();                             // test SP
void TestTypeKP();                             // test KP
void TestTypeOrder();                          // test >=


int main()
{
   time_lapse tl;      // measure program run time
   Real* s1; Real* s2; Real* s3; Real* s4;
   std::cout << "\nBegin test\n";   // Forces std::cout to allocate memory at beginning
   std::cout << "Now print a real number: " << 3.14159265 << std::endl;
   // Throw exception to set up exception buffer
#ifndef DisableExceptions
   Try { Throw(Exception("Just a dummy\n")); }
   CatchAll {}
#else
   std::cout << "Not doing exceptions\n";
#endif
   { Matrix A1(40,200); s1 = A1.Store(); }
   { Matrix A1(1,1); s3 = A1.Store(); }
   {
      Tracer et("Matrix test program");

      Matrix A(25,150);
      {
         int i;
         RowVector A(8);
         for (i=1;i<=7;i++) A(i)=0.0; A(8)=1.0;
         Print(A);
      }
      std::cout << "\n";

      TestTypeAdd(); TestTypeMult(); TestTypeConcat();
      TestTypeSP(); TestTypeKP(); TestTypeOrder();

      Try { 
         trymat1();
         trymat2();
         trymat3();
         trymat4();
         trymat5();
         trymat6();
         trymat7();
         trymat8();
         trymat9();
         trymata();
         trymatb();
         trymatc();
         trymatd();
         trymate();
         trymatf();
         trymatg();
         trymath();
         trymati();
         trymatj();
         trymatk();
         trymatl();
         trymatm();

         std::cout << "\nEnd of tests\n";
      }
      CatchAll
      {
         std::cout << "\nTest program fails - exception generated\n\n";
         std::cout << Exception::what();
      }


   }

   { Matrix A1(40,200); s2 = A1.Store(); }
   std::cout << "\n(The following memory checks are probably not valid with all\n";
   std::cout << "compilers - see documentation)\n";
   std::cout << "\nChecking for lost memory: "
      << (unsigned long)s1 << " " << (unsigned long)s2 << " ";
   if (s1 != s2) std::cout << " - error\n\n"; else std::cout << " - ok\n\n";
   { Matrix A1(1,1); s4 = A1.Store(); }
   std::cout << "\nChecking for lost memory: "
      << (unsigned long)s3 << " " << (unsigned long)s4 << " ";
   if (s3 != s4) std::cout << " - error\n\n"; else std::cout << " - ok\n\n";

   // check for Pentium bug
   PentiumCheck(4195835L,3145727L);
   PentiumCheck(5244795L,3932159L);

#ifdef DO_FREE_CHECK
   FreeCheck::Status();
#endif
   return 0;
}




//************************ test type manipulation **************************/


// These functions may cause problems for Glockenspiel 2.0c; they are used
// only for testing so you can delete them


void TestTypeAdd()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << "+     ";
   int i;
   for (i=0; i<MatrixType::nTypes(); i++) std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
	{
		std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
         std::cout << (list[j]+list[i]).Value() << " ";
      std::cout << "\n";
   }
   std::cout << "\n";
}

void TestTypeMult()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << "*     ";
   int i;
   for (i=0; i<MatrixType::nTypes(); i++)
      std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
   {
		std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
         std::cout << (list[j]*list[i]).Value() << " ";
      std::cout << "\n";
   }
   std::cout << "\n";
}

void TestTypeConcat()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << "|     ";
   int i;
   for (i=0; i<MatrixType::nTypes(); i++)
      std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
   {
		std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
         std::cout << (list[j] | list[i]).Value() << " ";
      std::cout << "\n";
   }
   std::cout << "\n";
}

void TestTypeSP()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << "SP    ";
   int i;
   for (i=0; i<MatrixType::nTypes(); i++)
		std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
   {
		std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
         std::cout << (list[j].SP(list[i])).Value() << " ";
      std::cout << "\n";
   }
   std::cout << "\n";
}

void TestTypeKP()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << "KP    ";
   int i;
   for (i=0; i<MatrixType::nTypes(); i++)
		std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
   {
		std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
         std::cout << (list[j].KP(list[i])).Value() << " ";
      std::cout << "\n";
   }
   std::cout << "\n";
}

void TestTypeOrder()
{
   MatrixType list[12];
   list[0] = MatrixType::UT;
   list[1] = MatrixType::LT;
   list[2] = MatrixType::Rt;
   list[3] = MatrixType::Sq;
   list[4] = MatrixType::Sm;
   list[5] = MatrixType::Dg;
   list[6] = MatrixType::BM;
   list[7] = MatrixType::UB;
   list[8] = MatrixType::LB;
   list[9] = MatrixType::SB;
   list[10] = MatrixType::Id;
   list[11] = MatrixType::Sk;

   std::cout << ">=    ";
   int i;
   for (i = 0; i<MatrixType::nTypes(); i++)
      std::cout << list[i].Value() << " ";
   std::cout << "\n";
   for (i=0; i<MatrixType::nTypes(); i++)
   {
      std::cout << list[i].Value() << " ";
      for (int j=0; j<MatrixType::nTypes(); j++)
	 std::cout << ((list[j]>=list[i]) ? "Yes   " : "No    ");
      std::cout << "\n";
   }
   std::cout << "\n";
}


//************** elapsed time class ****************

time_lapse::time_lapse()
{
   start_time = ((double)clock())/(double)CLOCKS_PER_SEC;
}

time_lapse::~time_lapse()
{
   double time = ((double)clock())/(double)CLOCKS_PER_SEC - start_time;
   std::cout << "Elapsed (processor) time = " << std::setprecision(2) << time << " seconds" << std::endl;
   std::cout << std::endl;
}


