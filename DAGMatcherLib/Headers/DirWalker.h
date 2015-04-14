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
 * $Log: DirWalker.h,v $
 * Revision 1.2  2002/05/16 22:49:29  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/

#ifndef __DIR_WALKER_H__
#define __DIR_WALKER_H__

#include <string>
//#include <map>
#include <list>

#define MAX_PATH_SIZE 260 // ie, MAX_PATH in Windows.h

#ifdef WIN32
typedef void* HANDLE;
struct _WIN32_FIND_DATAA;
#else
# include <sys/types.h>
# include <dirent.h>
typedef DIR* HANDLE;
#endif

/*!
	DirWalker::iterator it = dw.params.begin();

	for (it = dw.params.begin(); it != dw.params.end(); ++it)
	{
		it->first // param name (std::string)
		it->second // param value (int)
	}
*/
class DirWalker
{
	HANDLE hFile;
	char szDirPath[MAX_PATH_SIZE];
	size_t nDirPathLen;

#ifdef WIN32
	_WIN32_FIND_DATAA* m_pFindData;
#endif

public:
	//typedef std::map<std::string, int> PARAM_MAP;
	typedef std::list< std::pair<std::string, int> > PARAM_MAP;
	typedef PARAM_MAP::const_iterator const_iterator;
	typedef PARAM_MAP::iterator iterator;

	std::string path, name, ext;
	PARAM_MAP params;

public:
	DirWalker();
	~DirWalker();

	const char* GetDirPath() { return szDirPath; }
	int GetDirPathLen() { return nDirPathLen; }

	bool OpenDir(const char* szDirName);
	void CloseDir();
	bool GetNextFileOrDir(char* szFileName, size_t buffSize);
	bool GetNextFile(char* szFileName, size_t buffSize, char* szFileExt = NULL);
	bool GetNextDir(char* szFileName, size_t buffSize);

	bool ParseFileName(const char* szFileName);
	bool ParseFileNameParams();

	void AddToPath(const char* subPath);

	static bool HasWildcards(const char* szPath);
	static bool HasWildcards(const char* szPath, int* pLastFileSepPos);
	static bool IsDirectory(const char* szFileName);
	static const char* FindFileExtension(const char* szFileName);
	static bool CompareFileExtension(const char* szFileName, const char* szFileExt);
	static void ChangeFileExt(const char* szFileName, const char* szExt, char* szNewName);
	static int CompareLastModifDate(const char* szFileName1, const char* szFileName2);
	static bool CheckFilePath(const char* szFullFileName);
	static void SplitPrefixAndNumber(const char* szText, char* szPrefix, int* pNumber);

	int CountNumberedFiles(const char* szDirName, const char* szPrefix,
		const char* szFileExt, int* pMaxIdx = NULL, int* pMinIdx = NULL);

	static int CreateUniqueFileName(const char* szDirName, const char* szPrefix,
		const char* szFileExt, char szFileName[]);

	static int MakeNameUnique(std::string* pStrFileName);

	static std::string AppendToFileName(const char* szFileName, const char* szSubfix,
		const char* szNewFileExtension = NULL);

	static std::string AppendToExtension(const char* szFileName, int num);

	static int FileSize(const char* szFileName);

	//! Returns true iff the file exists and has non-zero size
	static bool CheckFileExist(const char* szFileName)
	{
		return (FileSize(szFileName) > 0);
	}

	static void RemoveFileExt(const char* szFileName, char* szNewName)
	{
		ChangeFileExt(szFileName, NULL, szNewName);
	}

	static std::string RemoveFileExt(const char* szFileName)
	{
		DirWalker dw;
		dw.ParseFileName(szFileName);
		std::string tmp = dw.path;
		//std::string tmp = dw.path + dw.name;
		return tmp;
	}

	static std::string GetName(const char* szFileName)
	{
		DirWalker dw;
		dw.ParseFileName(szFileName);
		return dw.name;
	}

	static std::string GetPath(const char* szFileName)
	{
		DirWalker dw;
		dw.ParseFileName(szFileName);
		return dw.path;
	}

	static std::string GetExtension(const char* szFileName)
	{
		DirWalker dw;
		dw.ParseFileName(szFileName);
		return dw.ext;
	}
};

#endif //__DIR_WALKER_H__
