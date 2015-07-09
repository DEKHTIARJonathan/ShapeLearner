#include <stdlib.h>

#include "shapeLearnerDll.h"

#include <iostream>
#include <string>

int main( int argc, char* argv[] )
{
   //openDataBase("postgres", "postgres", "postgres", "54.77.188.25", 11111, "structure.sql") ;
   //exit(EXIT_SUCCESS) ;

   char* _dbInit = "structure.sql";
   openDataBase("postgres", "postgres", "postgres", "54.77.188.25", 11111, _dbInit);
   std::cout << "Image signing Go!" << std::endl;
   signBinaryImage("img.ppm", "production", 2);
   waitBeforeClosing ();
   return 0;
}