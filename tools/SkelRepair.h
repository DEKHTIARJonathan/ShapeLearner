#ifndef __SKEL_REPAIR_H__
#define __SKEL_REPAIR_H__

#include "../AFMMSkeleton/include/field.h"
#include "MatrixLite.h"

typedef MatrixLite<char> PATMASK;
typedef std::pair<PATMASK, PATMASK> PATFIX;

struct FIXSET : public std::vector<PATFIX>
{
	int nNonZeros;
	int nPositives;
	
	//class iterator : public vector<PATFIX> 
	//{
	//};
	
	bool exists(const PATFIX& pf) const
	{
		for(FIXSET::iterator it = begin(); it != end(); it++)
			if (it->first == pf.first)
				return true;
		return false;
	}
	
	void print(std::ostream& os) const
	{
		for(FIXSET::iterator it = begin(); it != end(); it++)
		{
			const PATMASK& pm[2] = {it->first, it->second};

			for(int r = 0; r < pat.rows(); r++) 
			{
				for (int i = 0; i < 2; i++)
				{
					for(int c = 0; c < pat.cols(); c++) 
					{	
						if (pm[i][r][c] > 0)
							os << "| X ";
						else if (pm[i][r][c] < 0)
							os << "| ? ";
						else
							os << "|   ";
					}
					os << "\t";
				}
				os << "|\n ";
				for (int k = 0; k < pat.cols(); k++) os << "--- ";
				os << "\n";
			}
		}
	}
};

class SkelRepair
{
	std::vector<FIXSET> fixsetVec;
	int m_nMaxNumRows;
	int m_nMaxNumCols;
	
public:
	SkelRepair();
	
	int CountNonZeros(FIELD<int>* s, int dr, int dc, rows, int cols) const
	{
		int n = 0;
		
		for(int r = 0; r < rows; r++)
			for(int c = 0; c < cols; c++)
				if (s->value(c + dc, r + dr) != 0)
					n++;
		return n;
	}
	
	int SetNonZerosAndPositives(const PATFIX& pf0, int& nNonZeros, int& nPositives) const
	{
		const PATMASK& m = pf0.first;
		nNonZeros = nPositives = 0;
		
		for(int r = 0; r < m.rows(); r++)
			for(int c = 0; c < m.cols(); c++)
			{
				if (m[r][c] != 0) nNonZeros++;
				if (m[r][c] > 0) nPositives++;
			}
		return n;
	}
	
	void AddFix(const int* pat, const int* sol, int rows, int cols)
	{
		PATFIX pf;
		pf.first.CopyFromArray(pat, rows, cols);
		pf.second.CopyFromArray(sol, rows, cols);
		AddFixSet(pf);
		
		// Keep track of the max num of rows and cols
		if (rows > m_nMaxNumRows)
			m_nMaxNumRows = rows;
			
		if (cols > m_nMaxNumCols)
			m_nMaxNumCols = cols;
	}
	
	void AddFixSet(PATFIX& pf0)
	{
		FIXSET fs;
		PATFIX pf1;
		
		fs.nNonZeros = CountNonZeros(pf0);
		AddWithMirrors(pf0, fs);
		
		pf0.first.Rotate90(pf1.first);
		pf0.second.Rotate90(pf1.second);
		AddWithMirrors(pf1, fs);
		
		pf0.first.Rotate180(pf1.first);
		pf0.second.Rotate180(pf1.second);
		AddWithMirrors(pf1, fs);
		
		pf0.first.Rotate270(pf1.first);
		pf0.second.Rotate270(pf1.second);
		AddWithMirrors(pf1, fs);
		
		fixsetVec.push_back(fs);
	}
	
	void AddWithMirrors(PATFIX& pf0, FIXSET& fs)
	{
		PATFIX pf1;
		
		if (!fs.exists(pf0))
			fs.push_back(pf0);
			
		pf0.MirrorUp(pf1.first);
		pf0.MirrorUp(pf1.second);
		if (!fs.exists(pf1))
			fs.push_back(pf1);
			
		pf0.MirrorRight(pf1.first);
		pf0.MirrorRight(pf1.second);
		if (!fs.exists(pf1))
			fs.push_back(pf1);
	}
	
	bool SkelRepair::SingleCheckFix(FIELD<int>* s, int dr, int dc, const PATFIX& pf) const
	{
		const PATMASK& pat = pf.first;
		const PATMASK& sol = pf.second;
		int r, c, x, y;
		
		// Check the pattern
		for (r = 0; r < szRows; r++)
		{
			for (c = 0; c < szCols; c++)
			{
				if (pat[r][c] == -1)
					continue;
				if (s->value(c + dc, r + dr) == 0 && pat[r][c] != 0)
					return false;
				if (s->value(c + dc, r + dr) != 0 && pat[r][c] == 0)
					return false;
			}
		}
		
		// Apply the fix
		for (r = 0; r < szRows; r++)
		{
			for (c = 0; c < szCols; c++)
			{
				if (sol[r][c] == -1)
					continue;
				else if (s->value(c + dc, r + dr) != 0 && sol[r][c] == 0)
					s->value(c + dc, r + dr) = 0;
				else if (s->value(c + dc, r + dr) == 0 && sol[r][c] != 0)
				{
						// Find the closest valid value to insert
						for (x = c + dc - 1, y = r + dr; x <= c + dc + 1; x += 2)
							if (s->value(x,y) != 0)
							{
								s->value(c + dc, r + dr) = s->value(x,y);
								continue;
							}
								
						for (x = c + dc, y = r + dr - 1; y <= r + dr + 1; y += 2)
							if (s->value(x,y) > 0)
							{
								s->value(c + dc, r + dr) = s->value(x,y);
								continue;
							}
						
						for (x = c + dc - 1; x <= c + dc + 1; x += 2)
							for (y = r + dr - 1; y <= r + dr + 1; y += 2)
								if (s->value(x,y) > 0)
								{
									s->value(c + dc, r + dr) = s->value(x,y);
									continue;
								}
				}
			}
		}
			
		return true;
	}
	
	bool SkelRepair::FullCheckFix(FIELD<int>* s, int dc, int dr, const FIXSET& fs) const
	{	
		for(FIXSET::iterator it = fs.begin(); it != fs.end(); it++)
			if (SingleCheckFix(s, dc, dr, *it))
				return true;
		
		return false;
	}

	void FixField(FIELD<int>* s) const
	{	
		std::vector<FIXSET>::iterator it;
		MatrixLite<int> nz(m_nMaxNumRows, m_nMaxNumCols);
		int r, c, rows, cols;
		
		/* Another "invariant" property to check other than num nz is the
		number of neigbours for the center pixel in an even size mask
		*/
		
		for(c = 0; c < s->dimX(); c++)
		{
			for(r = 0; r < s->dimY(); r++)
			{
				nz.Init(-1);
				
				for(it = fixsetVec.begin(); it != fixsetVec.end(); it++)
				{
					rows = it->rows();
					cols = it->cols();
					
					if (c + cols >= s->dimX() || r + rows >= s->dimY())
						break; // the mask doesn't fit
					
					if (nz(rows, cols) == -1)
						nz(rows, cols) = CountNonZeros(s, r, c, rows, cols);
					
					if (nz(rows, cols) >= it->nPositives && nz(rows, cols) <= it->nNonZeros)
						FullCheckFix(s, r, c, *it);
				}
			}
		}
	}
	
	void Print(std::ostream& os) const
	{
		vector<FIXSET>::iterator it;
		int i = 0;
		for(it = fixsetVec.begin(); it != fixsetVec.end(); it++)
		{
			os << "\nPattern number " << i++;
			it->print(os);
		}
	}
};

#endif //__SKEL_REPAIR_H__


