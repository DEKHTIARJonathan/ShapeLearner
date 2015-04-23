/**------------------------------------------------------------------------
 * @file
 *
 * Copyright 2001 by the University of Toronto (UT).
 *
 *                       All Rights Reserved
 *
 * Permission to copy and modify this software and its documentation
 * only for internal use in your organization is hereby granted,
 * provided that this notice is retained thereon and on all copies.
 * UT makes no representations as to the suitability and operability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * UT DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
 * SHALL UT BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * No other rights, including for example, the right to redistribute
 * this software and its documentation or the right to prepare
 * derivative works, are granted unless specifically provided in a
 * separate license agreement.
 *
 * Author: Diego Macrini
 *
 * $Log: String.h,v $
 * Revision 1.2  2002/05/16 22:51:05  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
#ifndef _DML_STRING_H_
#define _DML_STRING_H_

#include <stdio.h>
#include <string.h>
#include "SmartArray.h"

typedef const char* CSTR;

namespace dml {
/*!
	\brief Simple string class based on SmartArray<char>

	Since it's not clear how to read and write a leda_string, I prefer
	to use this simple class to deal with strings.

	/see SmartArray.
*/

class String : public SmartArray<char>
{
public:
	static int CountDigits(int n)
	{
		int numDigit = 1;

		while ((n /= 10) > 0)
			numDigit++;

		return numDigit;
	}

public:
    String(const String& str) : SmartArray<char>(str) {}

	String(const char* sz = NULL)
	{
		operator=((sz) ? sz:"");
	}

	String(const char* szPrefix, const char* szSubfix)
	{
		const int n0 = strlen(szPrefix);
		const int n1 = strlen(szSubfix);
		const int len = n0 + n1;

		Resize(len + 1);

		int i, j;

		for (i = 0; i < n0; i++)
			operator[](i) = szPrefix[i];

		for (j = 0; j < n1; j++, i++)
			operator[](i) = szSubfix[j];

		operator[](i) = '\0';
	}

	String(const char* sz, const int num)
	{
		SetEqual(sz, num);
	}

	String(int n, int nBuffSize)
	{
		char* szBuff = new char[nBuffSize];

		if (szBuff)
		{
			sprintf(szBuff, "%d", n);
			operator=((const char*) szBuff);
			delete[] szBuff;
		}
		else
		{
			operator=("");
		}
	}

	String(int nSize) : SmartArray<char>(nSize > 0 ? nSize:1)
	{
		operator[](0) = '\0';
	}

	bool IsEmpty() const
	{
		ASSERT(GetSize() > 0);

		return GetSize() == 1;
	}

	void Clear()
	{
		if (!IsEmpty())
		{
			Resize(1);
			operator[](0) = '\0';
		}
	}

	//! Copy string including the terminating null character
	String& operator=(const char* sz)
	{
		int i, nLen;

		for (nLen = 0; sz[nLen] != '\0'; nLen++);

		Resize(++nLen);

		for (i = 0; i < nLen; i++)
			operator[](i) = sz[i];

		return *this;
	}

	String& operator=(int n)
	{
		char szBuff[100];
		sprintf(szBuff, "%d", n);
		return operator=((const char*) szBuff);
	}

	String& operator=(const double& n)
	{
		char szBuff[100];
		sprintf(szBuff, "%.3f", n);
		return operator=((const char*) szBuff);
	}

	String& SetEqual(const char* sz, const int num)
	{
		const int buffSize = strlen(sz) + CountDigits(num) + 1;
		char* szBuff = new char[buffSize];

		sprintf(szBuff, "%s%d", sz, num);

		ASSERT(strlen(szBuff) == buffSize - 1);

		operator=(szBuff);

		delete[] szBuff;

		return *this;
	}

	String operator+(const int num)
	{
		return String(GetData(), num);
	}

	String& operator+=(const char* sz)
	{
		int nLen1 = Len();
		int nLen2 = strlen(sz);
		String str;
		int i;

		str.Resize(nLen1 + nLen2 + 1);

		for (i = 0; i < nLen1; i++)
			str[i] = operator[](i);

		for (i = 0; i < nLen2; i++)
			str[i + nLen1] = sz[i];

		str[nLen1 + nLen2] = '\0';

		return operator=(str);
	}

	operator const char*() const { return GetData(); }

	const char* c_str() const { return GetData(); }

	bool operator==(const char* sz) const { return !strcmp(GetData(), sz); }
	bool operator!=(const char* sz) const { return !operator==(sz); }

	bool operator<(const String& rhs)
	{
		const int sz = (Size() < rhs.Size()) ? Size() : rhs.Size();
		char a, b;

		for (int i = 0; i < sz; i++)
		{
			a = operator[](i);
			b = rhs[i];

			if (a < b)
				return true;
			else if (a > b)
				return false;
		}

		return (Size() < rhs.Size());
	}

	int Len() const { return strlen(GetData()); }
	int length() const { return Len(); }

	std::istream& Read(std::istream& is)
	{
		SmartArray<char>::Read(is);

		// Ensure is a zero terminated string
		if (GetSize() == 0)
		{
			Resize(1);
			operator[](0) = '\0';
		}

		return is;
	}

	friend std::ostream& operator<<(std::ostream &os, const String& str)
	{
		return os << (const char*) str;
	}
};
} //namespace dml

#endif //_DML_STRING_H_
