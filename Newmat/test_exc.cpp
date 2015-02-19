#define WANT_STREAM

#include "newmatap.h"
#include "newmatio.h"              // to help namespace with VC++ 5

#ifdef use_namespace
using namespace RBD_LIBRARIES;
#endif

//#include <except.h>             // if you want to use set_terminate

/**************************** test exceptions ******************************/


int main()
{
   // activate the next expression if you want to use compiler supported
   // exceptions and you want Terminate to catch uncaught exceptions
   // set_terminate(Terminate);
   Real* s1; Real* s2; Real* s3; Real* s4;
   // Forces std::cout to allocate memory at beginning
   std::cout << "\nThis tests the exception system, so you will get\n" <<
      "a long list of error messages\n\n";
   std::cout << "\nPrint a real number (may help lost memory test): " << 3.14159265 << "\n";
   // Throw exception to set up exception buffer
   Try { Throw(Exception("Just a dummy\n")); }
   CatchAll {};
   { Matrix A1(40,200); s1 = A1.Store(); }
   { Matrix A1(1,1); s3 = A1.Store(); }
   {
      Tracer et("Test");

      Try
      {
         Tracer et("Try block");



         std::cout << "-----------------------------------------\n\n";
         Matrix A(2,3), B(4,5); A = 1; B = 2;
         std::cout << "Incompatible dimensions\n";
         et.ReName("Block A");
         Try { Matrix C = A + B; }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Bad index\n";
         et.ReName("Block B");
         Try { Real f = A(3,3); std::cout << f << std::endl; }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Illegal conversion\n";
         et.ReName("Block C");
         Try { UpperTriangularMatrix U = A; }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Invert non-square matrix - 1\n";
         et.ReName("Block D");
         Try { CroutMatrix X = A; }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Invert non-square matrix - 2\n";
         et.ReName("Block E");
         Try { Matrix X = A.i(); }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Non 1x1 matrix to scalar\n";
         et.ReName("Block F");
         Try { Real f = A.AsScalar(); std::cout << f << std::endl; }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Matrix to vector\n";
         et.ReName("Block G");
         Try { ColumnVector CV = A;}
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Invert singular matrix\n";
         et.ReName("Block H");
         Try { Matrix X(2,2); X<<1<<2<<2<<4; X = X.i(); }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "SubMatrix error\n";
         et.ReName("Block I");
         Try { Matrix X = A.Row(3); }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "SubMatrix error\n";
         et.ReName("Block J");
         Try { Matrix X = A.Row(0); }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Cholesky error\n";
         et.ReName("Block K");
         Try
         {
            SymmetricMatrix SM(50); SM = 10;
            LowerTriangularMatrix L = Cholesky(SM);
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Inequality error\n";
         et.ReName("Block L");
         Try
         {
            Matrix A(10,10), B(10,10); A = 10; B = 20;
            if ( A < B) A = B;
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Maximum of empty matrix\n";
         et.ReName("Block M");
         Try
         {
            Matrix A(10,20); A = 5; Matrix B=A.Rows(6,5);
            MaximumAbsoluteValue(B);
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Incorrectly ReSizing band matrix\n";
         et.ReName("Block N");
         Try
         {
            BandMatrix A(20,5,3); A = 5; UpperBandMatrix B;
            B.ReSize(A);
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "Incorrectly ReSizing symmetric band matrix\n";
         et.ReName("Block M");
         Try
         {
            BandMatrix A(20,5,3); A = 5; SymmetricBandMatrix B;
            B.ReSize(A);
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

         std::cout << "ReSize CroutMatrix\n";
         et.ReName("Block O");
         Try
         {
            Matrix A(3,3); A = 0; A(1,1) = A(2,2) = A(3,3) = 1;
            CroutMatrix B = A;;
            B.ReSize(A);
         }
         CatchAll { std::cout << Exception::what() << std::endl; }
         std::cout << "-----------------------------------------\n\n";

      }
      CatchAll { std::cout << "\nException generated in test program\n\n"; }
   }

   std::cout << "\nEnd test\n";
   { Matrix A1(40,200); s2 = A1.Store(); }
   std::cout << "\n(The following memory checks are probably not valid with all\n";
   std::cout << "compilers - see documentation)\n";
   std::cout << "\nChecking for lost memory: "
      << (unsigned long)s1 << " " << (unsigned long)s2 << " ";
   if (s1 != s2) std::cout << " - error\n"; else std::cout << " - ok\n";
   { Matrix A1(1,1); s4 = A1.Store(); }
   std::cout << "\nChecking for lost memory: "
      << (unsigned long)s3 << " " << (unsigned long)s4 << " ";
   if (s3 != s4) std::cout << " - error\n\n"; else std::cout << " - ok\n\n";


#ifdef DO_FREE_CHECK
   FreeCheck::Status();
#endif

//   Throw(Runtime_error("Exception outside try block"));

   return 0;
}
