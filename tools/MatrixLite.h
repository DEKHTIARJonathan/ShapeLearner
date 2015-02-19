#include <memory.h>

template<class T> class MatrixLite
{
	int m_row;
	int m_col;
	T* m_data; 
	
private:	
	MatrixLite& operator=(const MatrixLite&);
	
public:
	MatrixLite(int row, int col) : m_row(row), m_col(col), 
		m_data((row != 0 && col != 0) ? new T[row * col] : NULL)
	{
	}
	
	MatrixLite(const MatrixLite& src) : m_row(src.m_row), m_col(src.m_col),
		m_data((src.m_row != 0 && src.m_col != 0) ? new T[src.m_row * src.m_col] : NULL)
	{
		for(int r = 0;r < m_row; r++)
			for(int c = 0; c < m_col; c++)
				(*this)[r][c] = src[r][c];
	}
	
	~MatrixLite()
	{
		if(m_data)
			delete[] m_data;
	}
	
	inline T* operator[](int i) { return (m_data + (m_col * i)); }
	inline T const*const operator[](int i) const {return (m_data + (m_col * i)); }
	
	int rows() const { return m_row; }
	int cols() const { return m_col; }
	
	void Init(int val)
	{
		memset(m_data, val, m_row * m_col * sizeof(T));
	}
	
	void Resize(int row, int col)
	{
		if (m_row != row || m_col != col)
		{
			if(m_data) delete[] m_data;
			m_data = new T[row * col];
			m_row = row;
			m_col = col;
		}
	}
	
	bool operator==(const MatrixLite& rhs) const
	{
		if (m_row != rhs.m_row || m_col != rhs.m_col)
			return false;
			
		for (int r = 0; r < m_row; r++)
			for (int c = 0; c < m_col; c++)
				if ((*this)[r][c] != rhs[r][c])
					return false;
		return true;
	}
	
	bool operator!=(const MatrixLite& rhs) const
	{
		return !operator==(rhs);
	}
	
	void CopyFromArray(const T* rhs, int row, int col)
	{
		Resize(row, col);
		memcpy(m_data, rhs, m_row * m_col * sizeof(T));
	}
	
	void Rotate90(MatrixLite& rotMat) const
	{
		rotMat.Resize(m_row, m_col);
		for (int r = 0, R = m_row; r < m_row; r++, R--)
			for (int c = 0; c < m_col; c++)
				rotMat[c][r] = (*this)[R][c];
	}
	
	void Rotate180(MatrixLite& rotMat) const
	{
		rotMat.Resize(m_row, m_col);
		for (int r = 0, R = m_row; r < m_row; r++, R--)
			for (int c = 0, C = m_col; c < m_col; c++, C--)
				rotMat[r][c] = (*this)[R][C];
	}
	
	void Rotate270(MatrixLite& rotMat) const
	{
		rotMat.Resize(m_row, m_col);
		for (int r = 0; r < m_row; r++)
			for (int c = 0, C = m_col; c < m_col; c++, C--)
				rotMat[c][r] = (*this)[r][C];
	}
	
	void MirrowUp(MatrixLite& rotMat) const
	{
		rotMat.Resize(m_row, m_col);
		for (int r = 0, R = m_row; r < m_row; r++, R--)
			for (int c = 0; c < m_col; c++)
				rotMat[r][c] = (*this)[R][c];
	}
	
	void MirrowRight(MatrixLite& rotMat) const
	{
		rotMat.Resize(m_row, m_col);
		for (int r = 0; r < m_row; r++)
			for (int c = 0, C = m_col; c < m_col; c++, C--)
				rotMat[r][c] = (*this)[r][C];
	}
};
