#pragma once
#include  <Windows.h>
#include <iostream>
#include <string>
#include <iostream>
#include "MD5_calculator.h"

using namespace std;

typedef struct ULONGLONG_TO_TWO_DWORD
{
	DWORD LOW;
	DWORD HIGH;
}ULONGLONG_TO_TWO_DWORD;

typedef struct CHUNK{
	BYTE* azBuffer=NULL;
	DWORD cbLen;

}CHUNK;

class HashFile
{
public:

	HashFile(const std::wstring& sFileName)
		:m_sFileName(sFileName)
	{
		m_cbBytesRead = 0;
		fnLoadFile();
	}

	void fnPrintHash()
	{
		CHUNK chunkRead;
		MD5Calculator md5Calculator;

		while (0 != (chunkRead = fnReadChunk(md5Calculator.m_cbHashValueLen)).cbLen)
		{
			md5Calculator.fnUpdate((PBYTE)chunkRead.azBuffer, chunkRead.cbLen);
			HeapFree(GetProcessHeap(), 0, chunkRead.azBuffer);
		}

		if (chunkRead.cbLen)
			md5Calculator.fnDigest();
		else
			wcout << L"Error! no data could be read." << endl;
	}

	~HashFile()
	{
		CloseHandle(m_hFileHandler);
		m_hFileHandler = NULL;
	}



	


private:

	void fnLoadFile()
	{
		m_hFileHandler = CreateFileW(
			m_sFileName.c_str(),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}

	CHUNK fnReadChunk(DWORD cbSize)
	{
		PBYTE caBuffer = NULL;
		OVERLAPPED readDetails = { 0 };
		DWORD cbActualBytesRead = 0;

		if (m_hFileHandler == INVALID_HANDLE_VALUE)
		{
			return CHUNK{ NULL, 0 };
		}

		readDetails.Offset = ((ULONGLONG_TO_TWO_DWORD*)&m_cbBytesRead)->LOW;
		readDetails.OffsetHigh = ((ULONGLONG_TO_TWO_DWORD*)&m_cbBytesRead)->HIGH;

		caBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbSize);
		if (!ReadFile(m_hFileHandler, caBuffer, cbSize, &cbActualBytesRead, &readDetails))
		{
			return  CHUNK{ NULL, 0 };
		}

		m_cbBytesRead += cbActualBytesRead;
		return  CHUNK{ caBuffer, cbActualBytesRead };
	}

	const std::wstring m_sFileName;
	ULONGLONG m_cbBytesRead;
	HANDLE m_hFileHandler;


};
