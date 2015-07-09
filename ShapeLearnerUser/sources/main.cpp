
#include <stdlib.h>

#include "shapeLearnerDll.h"


int main( int argc, char* argv[] )
{
   openDataBase("postgres", "postgres", "postgres", "54.77.188.25", 11111, "structure.sql") ;
   exit(EXIT_SUCCESS) ;
}



