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
 * $Log: DirWalker.cpp,v $
 * Revision 1.2  2002/05/16 22:49:29  dmac
 * Several bugs fixed and a new DAG-derived class added
 *
 * Revision 1.1.1.1  2002/03/10 07:38:07  dmac
 * Imported sources
 *
 *
 *-----------------------------------------------------------------------*/
//#include <iostream>
#include "stdafx.h"
# include <windows.h>

DirWalker::DirWalker()
{
#ifdef WIN32
	m_pFindData = new WIN32_FIND_DATA;
#endif

	hFile = INVALID_HANDLE_VALUE;
	*szDirPath = '\0';
	nDirPathLen = 0;
}

DirWalker::~DirWalker()
{
	CloseDir();

#ifdef WIN32
	delete m_pFindData;
#endif
}

/*static*/
bool DirWalker::CheckFilePath(const char* szFullFileName)
{
	int nLen = strlen(szFullFileName);
#ifdef WIN32
	char cWrongFileSep = '/';
#else
	char cWrongFileSep = '\\';
#endif
	for (int i = 0; i < nLen; i++)
	{
		if (szFullFileName[i] == cWrongFileSep)
		{
			std::cerr << "\nError: path '" << szFullFileName
				<< "' contains one or more " << cWrongFileSep
				<< " characters.\nUse " << FILE_SEP
				<< " as the file-path separator character." << std::endl;
			return false;
		}
	}

	return true;
}

bool DirWalker::OpenDir(const char* szDirName)
{
	CloseDir();

	if (!CheckFilePath(szDirName))
		return false;

	nDirPathLen = strlen(szDirName);

#ifdef WIN32
	int nLastFileSepPos;

	if (HasWildcards(szDirName, &nLastFileSepPos))
	{
		hFile = FindFirstFile(szDirName, m_pFindData);
		nDirPathLen = nLastFileSepPos + 1;
	}
	else
	{
		char szDirNameWC[MAX_PATH_SIZE]; // needs the wildcard '*' to see all files
		strcpy(szDirNameWC, szDirName);

		if (szDirPath[nDirPathLen - 1] != FILE_SEP)
			strcat(szDirNameWC, "\\*");
		else
			strcat(szDirNameWC, "*");

		hFile = FindFirstFile(szDirNameWC, m_pFindData); // perhaps szDirName + "*.*"
	}
#else
	hFile = opendir(szDirName);
#endif

	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (nDirPathLen > 0)
		{
			memcpy(szDirPath, szDirName, nDirPathLen);
		}
		else
		{
			szDirPath[0] = '.';
			nDirPathLen = 1;
		}

		if (szDirPath[nDirPathLen - 1] != FILE_SEP)
			szDirPath[nDirPathLen++] = FILE_SEP;

		szDirPath[nDirPathLen] = '\0';

		return true;
	}

	return false;
}

void DirWalker::CloseDir()
{
	if (hFile != INVALID_HANDLE_VALUE)
	{
#ifdef WIN32
		FindClose(hFile);
#else
		closedir(hFile);
#endif
		hFile = INVALID_HANDLE_VALUE;
		*szDirPath = '\0';
		nDirPathLen = 0;
	}
}

/*!
	\brief Returns the next file or subdirectory in the opened directory. The
	subdirectory may be '.' or '..'.

	@param szFileName pointer to a buffer where the full file name is to be written.

	@buffSize size of the buffer. The buffer has to be large enough to hold the path
	characters, otherwise false is returned.
*/
bool DirWalker::GetNextFileOrDir(char* szFileName, size_t buffSize)
{
	const char* sz;
	size_t len;

	// Clear out szFileName before hand in case something goes wrong.
	*szFileName = '\0';

	// Read next file
#ifdef WIN32
	// We should already have a filename...
	if (*m_pFindData->cFileName == '\0')
		return false;

	sz = m_pFindData->cFileName;
	// below, and just before returning, read the next file...
#else
	struct dirent* dp;

	if ((dp = readdir(hFile)) == 0)
		return false;

	sz = dp->d_name;
#endif

	len = strlen(sz);

	if (nDirPathLen + len > buffSize - 1)
		return false;

	if (sz[0] == '.' && (len == 1 || (len == 2 && sz[1] == '.')))
	{
		memcpy(szFileName, sz, len + 1);
	}
	else
	{
		memcpy(szFileName, szDirPath, nDirPathLen);
		memcpy(szFileName + nDirPathLen, sz, len + 1);
	}

#ifdef WIN32
	// Read the next file, so that we have it for the next call
	if (FindNextFile(hFile, m_pFindData) == 0)
		*m_pFindData->cFileName = '\0'; // make sure filename is empty for next call
#endif

	return true;
}

/*!
	\brief Returns the next file in the opened directory.

	@param szFileName pointer to a buffer where the full file name is to be written.

	@buffSize size of th ebuffer poninted by szFileName.

	@szFileExt pointer to a buffer in which the file extention is to be stored.
	The buffer has to be large enough to hold 3 characters. If szFileExt is NULL,
	no file extention is returned.
*/
bool DirWalker::GetNextFile(char* szFileName, size_t buffSize, char* szFileExt /*=NULL*/)
{
	bool bSuccess;

	do {
		bSuccess = GetNextFileOrDir(szFileName, buffSize);
	} while(bSuccess && IsDirectory(szFileName));

	if (szFileExt && bSuccess)
	{
		*szFileExt = '\0'; // Init the string
		size_t len = strlen(szFileName + nDirPathLen);
		char* pEnd = szFileName + nDirPathLen + len - 1;

		for (int i = 0; i < 4; i++, pEnd--)
		{
			if (*pEnd == '.')
			{
				strcpy(szFileExt, pEnd + 1);
				break;
			}
		}
	}

	return bSuccess;
}

bool DirWalker::GetNextDir(char* szFileName, size_t buffSize)
{
	bool bSuccess, bDotDirectory;

	do {
		bSuccess = GetNextFileOrDir(szFileName, buffSize);

		// Ignore dot and dot-dot directory
		if (!strcmp(szFileName, ".") || !strcmp(szFileName, ".."))
			bDotDirectory = true;
		else
			bDotDirectory = false;
	} while(bSuccess && (bDotDirectory || !IsDirectory(szFileName)));

	return bSuccess;
}

/*!
	@brief Decomposes the given file name into (path, name, extension) parts.

	These components are stored in the member variables path, name, ext.
*/
bool DirWalker::ParseFileName(const char* szFileName)
{
	using namespace std;

	string str(szFileName);
	string::size_type extPos = str.rfind('.');
	string::size_type namePos = str.rfind(FILE_SEP, extPos);

	if (namePos == string::npos)
		namePos = 0;
	else
		namePos++;

	// If there is no extension, we have a special case
	if (extPos == string::npos)
	{
		ext.clear();
		name = str.substr(namePos);
	}
	else
	{
		ext = str.substr(extPos + 1, str.size() - extPos + 1);
		name = str.substr(namePos, extPos - namePos);
	}

	if (namePos > 0)
		path = str.substr(0, namePos);
	else
		path.clear();

	//cerr << "[" << path << "]" << "[" << name << "]" << "[" << ext << "]" << endl;

	return true;
}

/*!
	@brief Parses a file name of the form prefixNumer0Param1Number1Param2Number2....

	The results are left in the 'params' map, such that the first key in the map is prefix
	and its value is Number0. The remaining param-number pairs are added in order. The map
	can then be accessed sequentially or by key.

	@return the 'params' maps is filled in ONLY with the string-value pairs in the file name. That is,
	params is empty if there is no valied pair, and params would not include trailing text in the name.
*/
bool DirWalker::ParseFileNameParams()
{
	using namespace std;

	string prefix("null");
	string number;
	bool bLastChar, bIsNum, bReadingText = true;

	params.clear();

	for (unsigned int s = 0, e = 1; e < name.size(); e++)
	{
		bLastChar = (e == name.size() - 1);
		bIsNum = (isdigit(name[e]) != 0);

		if (bReadingText && (bIsNum || bLastChar))
		{
			prefix = name.substr(s, (bIsNum) ? e-s:e-s+1);
			s = e;
			bReadingText = false;

			// Handle special case: single last digit.
			if (bIsNum && bLastChar) e--;
		}
		else if (!bReadingText && (!bIsNum ||  bLastChar))
		{
			number = name.substr(s, (!bIsNum) ? e-s:e-s+1);
			//params[prefix] = atoi(number.c_str());
			params.push_back(std::make_pair(prefix, atoi(number.c_str())));
			s = e;
			bReadingText = true;
		}
	}

	return params.empty();
}

/*!
	Concatenates 'subPath' at the end of the 'path' member variable
*/
void DirWalker::AddToPath(const char* szSubPath)
{
	unsigned int n = path.size();

	if (n > 0 && path[n - 1] != FILE_SEP)
		path.append(1, FILE_SEP);

	path.append(szSubPath);
}

/*!
	@brief Finds wildcards {'*', '?'} in the path. Returns false if there are no
	wildcards and true otherwise.
*/
/*static*/
bool DirWalker::HasWildcards(const char* szPath)
{
	for (; *szPath != '\0'; szPath++)
	{
		if (*szPath == '*' || *szPath == '?')
			return true;
	}

	return false;
}

/*!
	@brief Checks whether the path contains wildcards.

	@return pLastFileSepPos is set to th eposition of the last FILE_SEP
	        character or -1 if there is no such character.
*/
/*static*/
bool DirWalker::HasWildcards(const char* szPath, int* pLastFileSepPos)
{
	*pLastFileSepPos = -1;

	for (int i = 0; *szPath != '\0'; szPath++, i++)
	{
		if (*szPath == '*' || *szPath == '?')
			return true;

		if (*szPath == FILE_SEP)
			*pLastFileSepPos = i;
	}

	return false;
}

/*static*/
bool DirWalker::IsDirectory(const char* szFileName)
{
#ifdef WIN32
	DWORD info = GetFileAttributes(szFileName);
	return (info & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
	struct stat info;

	if (stat(szFileName, &info) != 0)
	{
	    ShowError(strerror(errno));
		return false;
	}
	else
	{
		return (info.st_mode & S_IFDIR) != 0;
	}
#endif
}

/*static*/
const char* DirWalker::FindFileExtension(const char* szFileName)
{
	size_t len = strlen(szFileName);
	const char* pEnd = szFileName + len - 1;

	for (; pEnd > szFileName && *pEnd != FILE_SEP; pEnd--)
		if (*pEnd == '.')
			return pEnd + 1;

	return szFileName + len;
}

/*static*/
bool DirWalker::CompareFileExtension(const char* szFileName, const char* szFileExt)
{
	return strcmp(szFileExt, FindFileExtension(szFileName)) == 0;
}

/*!
	@brief Changes the extension component of the given file name by replacing
	it with the given extension, or removing it if szExt is NULL.
*/
/*static*/
void DirWalker::ChangeFileExt(const char* szFileName, const char* szExt, char* szNewName)
{
	const char* szOldExt = FindFileExtension(szFileName);
	int nBytes = szOldExt - szFileName;

	memcpy(szNewName, szFileName, nBytes);

	if (szExt)
	{
		if (szNewName[nBytes - 1] != '.')
			szNewName[nBytes++] = '.';

		strcpy(szNewName + nBytes, szExt);
	}
	/* szExt == NULL*/
	else if (szNewName[nBytes - 1] == '.')
		szNewName[nBytes - 1] = '\0';
	else
		szNewName[nBytes] = '\0';
}

/*!
	@brief Returs the last-modification time difference between szFileName1 and szFileName2
	returns:
		-1 First file time is earlier than second file time.
		0 First file time is equal to second file time.
		1 First file time is later than second file time.
*/
/*static*/
int DirWalker::CompareLastModifDate(const char* szFileName1, const char* szFileName2)
{
#ifndef WIN32
	struct stat info1, info2;

	info1.st_mtime = info2.st_mtime = 0;

	stat(szFileName1, &info1);
	stat(szFileName2, &info2);

	return info1.st_mtime - info2.st_mtime;
#else
	WIN32_FIND_DATA findData1, findData2;
	HANDLE hFile;

	if ((hFile = FindFirstFile(szFileName1, &findData1)) == INVALID_HANDLE_VALUE)
		return 1;

	FindClose(hFile);

	if ((hFile = FindFirstFile(szFileName2, &findData2)) == INVALID_HANDLE_VALUE)
		return 1;

	FindClose(hFile);

	return CompareFileTime(&findData1.ftLastWriteTime, &findData2.ftLastWriteTime);
#endif
}

/*!
	@brief Makes the given name unique by adding a unique ID as a subfix
	of the file name if necessary. The ID is choose as one plus
	the maximum ID dound in the given path.

	@return the chosen ID or -1 if no ID was added.
*/
/*static*/
int DirWalker::MakeNameUnique(std::string* pStrFileName)
{
	char szNewFileName[MAX_PATH_SIZE];
	DirWalker dw;

	ASSERT(pStrFileName);

	dw.ParseFileName(pStrFileName->c_str());

	int id = CreateUniqueFileName(dw.path.c_str(), dw.name.c_str(),
		dw.ext.c_str(), szNewFileName);

	if (id >= 0)
		*pStrFileName = szNewFileName;

	return id;
}

/*!
	@brief Finds the maximum subfix of any file that has
	the given name prefix and extension

	@return -1 if the given directory is invalid
			0 if there is no file that matches the name
			n in [1, MAXINT] s.t. n is the number of matching subfix found
*/
int DirWalker::CountNumberedFiles(const char* szDirName, const char* szPrefix,
								  const char* szFileExt,
								  int* pMaxIdx, int* pMinIdx)
{
	// If we are given an empty path, set it to the currrent path
	if (*szDirName == '\0')
		szDirName = "."; // ie, just copy ptr adress no str content

	// Init the max and min indices if given
	if (pMaxIdx)
		*pMaxIdx = -1;

	if (pMinIdx)
		*pMinIdx = -1;

	// Open the given directory
	if (!IsDirectory(szDirName) || !OpenDir(szDirName))
		return -1;

	char szFileName[MAX_PATH_SIZE];
	int nPrefixLen = strlen(szPrefix);
	int nPathLen   = GetDirPathLen();
	char szExt[10];
	int n, nCount = 0;

	while (GetNextFile(szFileName, MAX_PATH_SIZE, szExt))
	{
		// Find the maximum subfix of any file that has
		// the given name prefix and extension
		if (!strncmp(szPrefix, szFileName + nPathLen, nPrefixLen) &&
			!strcmp(szFileExt, szExt))
		{
			nCount++;

			// make use of the fact that atoi ignores characters
			n = atoi(szFileName + nPathLen + nPrefixLen);

			if (pMaxIdx && n > *pMaxIdx)
				*pMaxIdx = n;

			if (pMinIdx && (*pMinIdx < 0 || n < *pMinIdx))
				*pMinIdx = n;
		}
	}

	return nCount;
}

/*!
	\brief Creates a unique file name with the given prefix and extension of the
	form 'szDirNameprefix.ext' or 'szDirNameprefixID.ext'. Here ID is added to make
	the file name unique in the given directory.

	ID is the next number corresponding to the gratest ID of file names
	in the given directory with the same name form.

	\param szDirName directory name.
	\param szPrefix prefix for the file
	\param szFileExt extension of the file
	\param szFileName buffer where the created file name is stored.
		    Should be of size MAX_PATH_SIZE.
	\return the ID added to the file name, or -1 if no ID was added.
*/
/*static*/
int DirWalker::CreateUniqueFileName(const char* szDirName, const char* szPrefix,
									const char* szFileExt, char szFileName[])
{
	DirWalker dw;

	int nMax, nCount;

	nCount = dw.CountNumberedFiles(szDirName, szPrefix, szFileExt, &nMax);

	if (nCount == -1)
		*szFileName = '\0';	// init return variable to a valid string
	else if (nCount == 0)
		sprintf(szFileName, "%s%s.%s", dw.GetDirPath(), szPrefix, szFileExt);
	else
		sprintf(szFileName, "%s%s%d.%s", dw.GetDirPath(), szPrefix, ++nMax, szFileExt);

	return nMax;
}

/*!
	@bried Appends szSubfix to the file name. If szNewFileExtension is not NULL,
	it is used as the new extension of the file.
*/
/*static*/
std::string DirWalker::AppendToFileName(const char* szFileName, const char* szSubfix,
	const char* szNewFileExtension /*=NULL*/)
{
	DirWalker dw;

	dw.ParseFileName(szFileName);

	std::string newFileName = dw.path + dw.name + szSubfix;

	if (szNewFileExtension != NULL)
	{
		newFileName += ".";
		newFileName += szNewFileExtension;
	}
	else if (dw.ext.length() > 0)
	{
		newFileName += ".";
		newFileName += dw.ext;
	}

	return newFileName;
}

/*!
	@bried Appends a number to the file name extension.
*/
/*static*/
std::string DirWalker::AppendToExtension(const char* szFileName, int num)
{
	char szNewFileName[MAX_PATH_SIZE];

	sprintf(szNewFileName, "%s%d", szFileName, num);

	return szNewFileName;
}

/*!
	Assumes that the given text is of the form PrefixNumber, and splits
	it accordingly.

	It is assume that the buffer pointed by szPrefix is big enough to store
	the prefix.
*/
/*static*/
void DirWalker::SplitPrefixAndNumber(const char* szText, char* szPrefix, int* pNumber)
{
	const int len = strlen(szText);

	*szPrefix = '\0';
	*pNumber = -1;

	for (int i = 0; i < len; i++)
	{
		if (isdigit(szText[i]))
		{
			*pNumber = atoi(szText + i);

			if (i > 0)
			{
				strncpy(szPrefix, szText, i);
				szPrefix[i] = '\0';
			}

			break;
		}
	}
}

/*!
	Returs the size of the file of -1 if the file does not exist
*/
/*static*/
int DirWalker::FileSize(const char* szFileName)
{
	std::fstream auxFile;

	// Set 'ate' (at end) mode so that tellg > 0 if file is not empty
	auxFile.open(szFileName, std::ios_base::in | std::ios_base::binary
		| std::ios_base::ate);

	return (auxFile.fail()) ? -1 : (int)auxFile.tellg();
}

/*!The following sample code will search the current  directory
     for the entry name:*/
/*int ReadDir(char* name)
{
	DIR* dirp = opendir(".");
	struct dirent* dp;
	int errno;

	while (dirp)
	{
		errno = 0;

		if ((dp = readdir(dirp)) != NULL)
		{
			DBG_MSG(dp->d_name)
			if (strcmp(dp->d_name, name) == 0)
			{
				closedir(dirp);
				return FOUND;
			}
		}
		else
		{
			if (errno == 0)
			{
				closedir(dirp);
				return NOT_FOUND;
			}

			closedir(dirp);

			return READ_ERROR;
		}
	}

	return OPEN_ERROR;
}*/