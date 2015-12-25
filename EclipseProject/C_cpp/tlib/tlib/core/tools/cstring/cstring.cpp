#include "cstring.h"
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef BYTE
#define BYTE unsigned char
#endif

#define DATE_LEN 8  //日期字符串长度
#define TIME_LEN 6  //时间字符串长度

int _afxInitData[] = { -1, 0, 0, 0 };
CStringData _InitData;
//CStringData* _afxDataNil = (CStringData*)&_afxInitData;
CStringData* _afxDataNil = &_InitData;
void UPPER(char* s) {
	if (s == NULL)
		return;
	while (*s != 0) {
		if (*s >= 'a' && *s <= 'z')
			*s = *s + 'A' - 'a';
		s++;
	}
}

void LOWER(char* s) {
	if (s == NULL)
		return;
	while (*s != 0) {
		if (*s >= 'A' && *s <= 'Z')
			*s = *s + 'a' - 'A';
		s++;
	}
}

int NameList::GetItemCount() {
	int iCnt = 0;
	int iPos = m_strList.Find(m_strSeparator);
	while (iPos >= 0) {
		iCnt++;
		iPos += strlen(m_strSeparator);
		iPos = m_strList.Find(m_strSeparator, iPos);
	}
	return iCnt;
}

CString::CString() {
	Init();
}

CString::CString(const CString& stringSrc) {
	if (stringSrc.GetData()->nRefs >= 0 && stringSrc.GetData() != _afxDataNil) {
		m_pchData = stringSrc.m_pchData;
		GetData()->nRefs++;
	} else {
		Init();
		*this = stringSrc.m_pchData;
	}
}
void CString::Init() {
	m_pchData = _afxDataNil->data();
}

void CString::AllocBuffer(int nLen)
// always allocate one extra character for '\0' termination
// assumes [optimistically] that data length will equal allocation length
		{
	if (nLen == 0)
		Init();
	else {
		CStringData* pData;
		pData = (CStringData*) new BYTE[sizeof(CStringData) + (nLen + 1)];
		pData->nAllocLength = nLen;
		pData->nRefs = 1;
		pData->data()[nLen] = '\0';
		pData->nDataLength = nLen;
		m_pchData = pData->data();
	}
}

void CString::FreeData(CStringData* pData) {
	delete[] (BYTE*) pData;
}

void CString::Release() {
	if (GetData() != _afxDataNil) {
		if (--GetData()->nRefs <= 0)
			FreeData(GetData());
		Init();
	}
}

void CString::Release(CStringData* pData) {
	if (pData != _afxDataNil) {
		if (pData->nRefs == 0)
			return;
		if (--pData->nRefs <= 0)
			FreeData(pData);
	}
}

void CString::Empty() {
	if (GetData()->nDataLength == 0)
		return;
	if (GetData()->nRefs >= 0)
		Release();
	else
		*this = _afxDataNil->data();
}

void CString::CopyBeforeWrite() {
	if (GetData()->nRefs > 1) {  // It's the time to allocate exclusive memory
		CStringData* pData = GetData();
		Release();
		AllocBuffer(pData->nDataLength);
		memcpy(m_pchData, pData->data(), pData->nDataLength + 1);
	}
}

void CString::AllocBeforeWrite(int nLen) {
	if (GetData()->nRefs > 1 || nLen > GetData()->nAllocLength) {
		Release();
		AllocBuffer(nLen);
	}
}

CString::~CString()
//  free any attached data
{
	if (GetData() != _afxDataNil) {
		if (--GetData()->nRefs <= 0)
			FreeData(GetData());
	}
}

//////////////////////////////////////////////////////////////////////////////
// Helpers for the rest of the implementation

void CString::AllocCopy(CString& dest, int nCopyLen, int nCopyIndex,
		int nExtraLen) const {
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string

	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0) {
		dest.Init();
	} else {
		dest.AllocBuffer(nNewLen);
		memcpy(dest.m_pchData, m_pchData + nCopyIndex, nCopyLen);
	}
}

CString::CString(LPCSTR lpsz) {
	Init();
	int nSrcLen = lpsz != NULL ? strlen(lpsz) : 0;
	if (nSrcLen != 0) {
		AllocBuffer(nSrcLen);
		memcpy(m_pchData, lpsz, nSrcLen + 1);
	}
}

CString::CString(LPCSTR lpsz, int nLength) {
	Init();
	int nSrcLen = lpsz != NULL ? strlen(lpsz) : 0;
	if (nLength < nSrcLen && nLength >= 0)
		nSrcLen = nLength;
	if (nSrcLen != 0) {
		AllocBuffer(nSrcLen);
		memcpy(m_pchData, lpsz, nSrcLen);
		m_pchData[nSrcLen] = '\0';
	}
}

CString::CString(char ch, int nLength) {
	Init();
	if (nLength >= 1) {
		AllocBuffer(nLength);
		memset(m_pchData, ch, nLength);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Assignment operators
//  All assign a new value to the string
//      (a) first see if the buffer is big enough
//      (b) if enough room, copy on top of old buffer, set size and type
//      (c) otherwise free old string data, and create a new one
//
//  All routines return the new string (but as a 'const CString&' so that
//      assigning it again will cause a copy, eg: s1 = s2 = "hi there".
//

void CString::AssignCopy(int nSrcLen, LPCSTR lpszSrcData) {
	AllocBeforeWrite(nSrcLen);
	memcpy(m_pchData, lpszSrcData, nSrcLen);
	GetData()->nDataLength = nSrcLen;
	m_pchData[nSrcLen] = '\0';
}

const CString& CString::operator=(const CString& stringSrc) {
	if (m_pchData != stringSrc.m_pchData) {
		if ((GetData()->nRefs < 0 && GetData() != _afxDataNil)
				|| stringSrc.GetData()->nRefs < 0) {
			// actual copy necessary since one of the strings is locked
			AssignCopy(stringSrc.GetData()->nDataLength, stringSrc.m_pchData);
		} else {
			// can just copy references around
			Release();
			m_pchData = stringSrc.m_pchData;
			GetData()->nRefs++;
		}
	}
	return *this;
}

const CString& CString::operator=(LPCSTR lpsz) {
	AssignCopy(strlen(lpsz), lpsz);
	return *this;
}

const CString& CString::operator=(char ch) {
	AssignCopy(1, &ch);
	return *this;
}

//////////////////////////////////////////////////////////////////////////////
// concatenation

// NOTE: "operator+" is done as friend functions for simplicity
//      There are three variants:
//          CString + CString
// and for ? = TCHAR, LPCSTR
//          CString + ?
//          ? + CString

void CString::ConcatCopy(int nSrc1Len, LPCSTR lpszSrc1Data, int nSrc2Len,
		LPCSTR lpszSrc2Data) {
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new CString object

	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0) {
		AllocBuffer(nNewLen);
		memcpy(m_pchData, lpszSrc1Data, nSrc1Len);
		memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len);
	}
}

CString operator+(const CString& string1, const CString& string2) {
	CString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData,
			string2.GetData()->nDataLength, string2.m_pchData);
	return s;
}

CString operator+(const CString& string, LPCSTR lpsz) {
	CString s;
	s.ConcatCopy(string.GetData()->nDataLength, string.m_pchData,
			CString::SafeStrlen(lpsz), lpsz);
	return s;
}

CString operator+(LPCSTR lpsz, const CString& string) {
	CString s;
	s.ConcatCopy(CString::SafeStrlen(lpsz), lpsz, string.GetData()->nDataLength,
			string.m_pchData);
	return s;
}

CString operator+(const CString& string1, char ch) {
	CString s;
	s.ConcatCopy(string1.GetData()->nDataLength, string1.m_pchData, 1, &ch);
	return s;
}

CString operator+(char ch, const CString& string) {
	CString s;
	s.ConcatCopy(1, &ch, string.GetData()->nDataLength, string.m_pchData);
	return s;
}

//////////////////////////////////////////////////////////////////////////////
// concatenate in place

void CString::ConcatInPlace(int nSrcLen, LPCSTR lpszSrcData) {
	//  -- the main routine for += operators

	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)
		return;

	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetData()->nRefs > 1
			|| GetData()->nDataLength + nSrcLen > GetData()->nAllocLength) {
		// we have to grow the buffer, use the ConcatCopy routine
		CStringData* pOldData = GetData();
		ConcatCopy(GetData()->nDataLength, m_pchData, nSrcLen, lpszSrcData);
		CString::Release(pOldData);
	} else {
		// fast concatenation when buffer big enough
		memcpy(m_pchData + GetData()->nDataLength, lpszSrcData, nSrcLen);
		GetData()->nDataLength += nSrcLen;
		m_pchData[GetData()->nDataLength] = '\0';
	}
}

const CString& CString::operator+=(LPCSTR lpsz) {
	ConcatInPlace(SafeStrlen(lpsz), lpsz);
	return *this;
}

const CString& CString::operator+=(char ch) {
	ConcatInPlace(1, &ch);
	return *this;
}

const CString& CString::operator+=(const CString& string) {
	ConcatInPlace(string.GetData()->nDataLength, string.m_pchData);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Advanced direct buffer access

LPTSTR CString::GetBuffer(int nMinBufLength) {
	if (GetData()->nRefs > 1 || nMinBufLength > GetData()->nAllocLength) {
		// we have to grow the buffer
		CStringData* pOldData = GetData();
		int nOldLen = GetData()->nDataLength;   // AllocBuffer will tromp it
		if (nMinBufLength < nOldLen)
			nMinBufLength = nOldLen;
		AllocBuffer(nMinBufLength);
		memcpy(m_pchData, pOldData->data(), (nOldLen + 1));
		GetData()->nDataLength = nOldLen;
		CString::Release(pOldData);
	}
	return m_pchData;
}

void CString::ReleaseBuffer(int nNewLength) {
	CopyBeforeWrite();  // just in case GetBuffer was not called

	if (nNewLength == -1)
		nNewLength = strlen(m_pchData); // zero terminated

	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
}

LPTSTR CString::GetBufferSetLength(int nNewLength) {
	if (nNewLength < 0)
		return NULL;
	GetBuffer(nNewLength);
	GetData()->nDataLength = nNewLength;
	m_pchData[nNewLength] = '\0';
	return m_pchData;
}

void CString::FreeExtra() {
	if (GetData()->nDataLength != GetData()->nAllocLength) {
		CStringData* pOldData = GetData();
		AllocBuffer(GetData()->nDataLength);
		memcpy(m_pchData, pOldData->data(), pOldData->nDataLength);
		CString::Release(pOldData);
	}
}

LPTSTR CString::LockBuffer() {
	LPTSTR lpsz = GetBuffer(0);
	GetData()->nRefs = -1;
	return lpsz;
}

void CString::UnlockBuffer() {
	if (GetData()->nRefs != -1)
		return;
	if (GetData() != _afxDataNil)
		GetData()->nRefs = 1;
}

///////////////////////////////////////////////////////////////////////////////
// Commonly used routines (rarely used routines in STREX.CPP)

int CString::Find(char ch) const {
	return Find(ch, 0);
}

int CString::Find(char ch, int nStart) const {
	int nLength = GetData()->nDataLength;
	if (nStart >= nLength)
		return -1;

	// find first single character
	LPTSTR lpsz = strchr(m_pchData + nStart, ch);

	// return -1 if not found and index otherwise
	return (lpsz == NULL) ? -1 : (int) (lpsz - m_pchData);
}

int CString::FindOneOf(LPCSTR lpszCharSet) const {
	LPTSTR lpsz = strpbrk(m_pchData, lpszCharSet);
	return (lpsz == NULL) ? -1 : (int) (lpsz - m_pchData);
}

int CString::Find(LPCSTR lpszSub) const {
	return Find(lpszSub, 0);
}

int CString::Find(LPCSTR lpszSub, int nStart) const {
	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;

	// find first matching substring
	LPTSTR lpsz = strstr(m_pchData + nStart, lpszSub);

	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int) (lpsz - m_pchData);
}

void CString::MakeUpper() {
	CopyBeforeWrite();
	UPPER(m_pchData);
}

void CString::MakeLower() {
	CopyBeforeWrite();
	LOWER(m_pchData);
}

int CString::Replace(LPCSTR lpszOld, LPCSTR lpszNew) {
	// can't have empty or NULL lpszOld

	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);

	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd) {
		while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL) {
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += strlen(lpszStart) + 1;
	}

	// if any changes were made, make them
	if (nCount > 0) {
		CopyBeforeWrite();

		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1) {
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength);
			CString::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;

		// loop again to actually do the work
		while (lpszStart < lpszEnd) {
			while ((lpszTarget = strstr(lpszStart, lpszOld)) != NULL) {
				int nBalance = nOldLength
						- (lpszTarget - m_pchData + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
						nBalance);
				memcpy(lpszTarget, lpszNew, nReplacementLen);
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += strlen(lpszStart) + 1;
		}
		GetData()->nDataLength = nNewLength;
	}

	return nCount;
}

CString CString::Mid(int nFirst) const {
	return Mid(nFirst, GetData()->nDataLength - nFirst);
}

CString CString::Mid(int nFirst, int nCount) const {
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetData()->nDataLength)
		nCount = GetData()->nDataLength - nFirst;
	if (nFirst > GetData()->nDataLength)
		nCount = 0;

	// optimize case of returning entire string
	if (nFirst == 0 && nFirst + nCount == GetData()->nDataLength)
		return *this;

	CString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	return dest;
}

CString CString::Right(int nCount) const {
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	CString dest;
	AllocCopy(dest, nCount, GetData()->nDataLength - nCount, 0);
	return dest;
}

CString CString::Left(int nCount) const {
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;

	CString dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}

void CString::TrimRight() {
	// find beginning of trailing spaces by starting at beginning (DBCS aware)

	CopyBeforeWrite();
	LPTSTR lpsz = m_pchData;
	LPTSTR lpszLast = NULL;

	while (*lpsz != '\0') {
		if (isspace(*lpsz)) {
			if (lpszLast == NULL)
				lpszLast = lpsz;
		} else
			lpszLast = NULL;
		lpsz++;
	}

	if (lpszLast != NULL) {
		// truncate at trailing space start
		*lpszLast = '\0';
		GetData()->nDataLength = lpszLast - m_pchData;
	}
}

void CString::TrimLeft() {
	// find first non-space character

	CopyBeforeWrite();
	LPCSTR lpsz = m_pchData;

	while (isspace(*lpsz))
		lpsz++;

	if (lpsz != m_pchData) {
		// fix up data and length
		int nDataLength = GetData()->nDataLength - (lpsz - m_pchData);
		memmove(m_pchData, lpsz, (nDataLength + 1));
		GetData()->nDataLength = nDataLength;
	}
}

void CString::Trim() {
	TrimLeft();
	TrimRight();
}

// formatting (using wsprintf style formatting)
#define MAX_STRLEN_IN_FORMAT 4096
void CString::Format(LPCSTR lpszFormat, ...) {
	va_list argList;
	va_start(argList, lpszFormat);
	GetBuffer(MAX_STRLEN_IN_FORMAT);
	vsprintf(m_pchData, lpszFormat, argList);
	ReleaseBuffer();
	va_end(argList);
}

CString itoa0(int iNo, int iLen) {
	char szNo[20];
	itoa(iNo, szNo, 10);
	CString strResult = szNo;
	strResult = CString('0', iLen - strResult.GetLength()) + strResult;
	return strResult;
}

CString itoa(int iNo) {
	char szNo[20];
	itoa(iNo, szNo, 10);
	return szNo;
}

void itoa(int iNo, char* buffer, int iDec) {
	char buff[20];
	int i = 0;
	if (iNo < 0) {
		buff[0] = '-';
		iNo = -iNo;
		i++;
	}
	do {
		int iDigit = iNo % iDec;
		buff[i] = iDigit + '0';
		iNo = iNo / iDec;
		i++;
	} while (iNo != 0);
	buff[i] = 0;
	int iLen = strlen(buff);
	buffer[iLen] = 0;
	for (i = 0; i < iLen; i++)
		buffer[iLen - i - 1] = buff[i];
}

BOOL CString::IsDigit() const {
	for (int i = 0; i < GetLength(); i++) {
		if (!isdigit(m_pchData[i]))
			return FALSE;
	}
	return TRUE;
}

BOOL CString::IsDecimal(char chDot) const {
	int iDotCount = 0;
	int iSignCount = 0;
	int iLen = GetLength();
	for (int i = 0; i < iLen; i++) {
		if (m_pchData[i] == chDot)
			iDotCount++;
		else if (m_pchData[i] == '-' || m_pchData[i] == '+') {
			if (i != 0 || iLen == 1)
				return FALSE;
			else
				iSignCount++;
		} else if (!isdigit(m_pchData[i]))
			return FALSE;
	}
	if (iDotCount > 1 || iSignCount > 1)
		return FALSE;
	else
		return TRUE;
}

BOOL atot(tm& cDatetime, const CString& strADate, const CString& strATime,
		BOOL bHasCentury) {
	cDatetime.tm_isdst = 0;
	CString strDate, strTime;
	if (strADate.IsEmpty()) {
		strDate = "000000";
		if (bHasCentury)
			strDate += "00";
	} else {
		strDate = strADate;
		if (!bHasCentury)
			strDate = "20" + strDate;
	}

	if (strATime.IsEmpty())
		strTime = "000000";
	else
		strTime = strATime;

	if (!strDate.IsDigit() || !strTime.IsDigit())
		return FALSE;

	cDatetime.tm_year = atoi(strDate.Left(4)) - 1900;
	cDatetime.tm_mon = atoi(strDate.Mid(4, 2)) - 1;
	cDatetime.tm_mday = atoi(strDate.Right(2));
	cDatetime.tm_hour = atoi(strTime.Left(2));
	cDatetime.tm_min = atoi(strTime.Mid(2, 2));
	cDatetime.tm_sec = atoi(strTime.Right(2));
	return (mktime(&cDatetime) != -1);
}

BOOL ttoa(tm& cDatetime, CString& strDate, CString& strTime, BOOL bHasCentury) {
	if (mktime(&cDatetime) == -1)
		return FALSE;
	if (bHasCentury)
		strDate = itoa0(cDatetime.tm_year + 1900, 4);
	else
		strDate = itoa0(cDatetime.tm_year - 100, 2);
	strDate += itoa0(cDatetime.tm_mon + 1, 2) + itoa0(cDatetime.tm_mday, 2);
	strTime = itoa0(cDatetime.tm_hour, 2) + itoa0(cDatetime.tm_min, 2)
			+ itoa0(cDatetime.tm_sec, 2);
	return TRUE;
}
