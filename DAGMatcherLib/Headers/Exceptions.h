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
 * Author: Diego Macrini (dmac@cs.toronto.edu)
 *
 * $Log: Exceptions.h,v $
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include "stdafx.h"

class ExceptionInfo
{
public:
	const char* m_szText;
	const char* m_szFileName;
	int			m_nLine;

	ExceptionInfo()
	{
		m_szText = "Default exception message.";
		m_nLine = 0;
		m_szFileName = "";
	}

	ExceptionInfo(const char* szText, const char* szFileName, int nLine)
	{
		m_szText = szText;
		m_szFileName = szFileName;
		m_nLine = nLine;
	}

	ExceptionInfo(const ExceptionInfo& e)
	{
		*this = e;
	}

	ExceptionInfo& operator=(const ExceptionInfo& e)
	{
		m_szText = e.m_szText;
		m_szFileName = e.m_szFileName;
		m_nLine = e.m_nLine;

		return *this;
	}

	void Print(std::ostream& os = std::cerr)
	{
		os << "\n\nException occurred at line " << m_nLine << " in " << m_szFileName
			<< ": " << m_szText << std::endl << std::endl;
	}
};

#define THROW_EXCEPTION(A) throw ExceptionInfo(A, __FILE__, __LINE__)

#endif // _EXCEPTIONS_H_
