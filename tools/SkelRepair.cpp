#include "SkelRepair.h"

const SkelRepair g_skelRepair;

SkelRepair::SkelRepair() : m_nMaxNumRows(0), m_nMaxNumCols(0)
{
	const int pat1[] = {	0, 0, 1, 
				0, 1, 1, 
				0, 0, 1  };
			    
	const int sol1[] = {	0, 0, 1, 
				0, 0, 1, 
				0, 0, 1  };
	
	sr.AddFix(pat1, sol1, 3, 3);
}
