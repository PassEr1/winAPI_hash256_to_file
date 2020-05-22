#pragma once
#include  <Windows.h>
#include <iostream>
#include <wincrypt.h>
#include <bcrypt.h>
using namespace std;

#define SUCESS(status) (((status)) >= 0)

class MD5Calculator {

public:
	MD5Calculator()
	{
		fnSetAlgorithmProvider(&m_lpAlgorithmProvider);		
		fnSetHashMaker();
		fnAllocateBufferForHashResult();
	}
	~MD5Calculator()
	{
		HeapFree(GetProcessHeap(), 0, m_pbHashResultBuffer);
		HeapFree(GetProcessHeap(), 0, m_lpbHashMaker);
		BCryptDestroyHash(m_phHash);
		BCryptCloseAlgorithmProvider(m_lpAlgorithmProvider, 0);
	}

	

	void fnUpdate(PBYTE data, DWORD valid_length)
	{
		
		DWORD cbHashValueLen;
		DWORD cbBytesCopiedToHashObjectLen;
		DWORD cbHashResult;
		DWORD cbBytesCopiedTocbHashResult;	

		//hash the data //may be repeated more than once times
		
		if (!SUCESS(
			BCryptHashData(
			m_phHash,
			(PUCHAR)data,
			valid_length,
			0)
		))
		{
			m_bIsSomethingWentWorng = true;
		}
	}

	void fnDigest()
	{
		if (m_bIsSomethingWentWorng)
		{
			cout << "Error! " << endl;
			return;
		}

		else
		{
			BCryptFinishHash(
				m_phHash,
				m_pbHashResultBuffer,
				m_cbHashValueLen,
				0
			);
		}

		for (int i = 0; i < m_cbHashValueLen; i++)
		{
			printf("%.2x", ((UCHAR*)m_pbHashResultBuffer)[i]);
		}

	}


DWORD m_cbHashValueLen;

private:
	void fnSetAlgorithmProvider(BCRYPT_ALG_HANDLE* lpAlgorithmProvider)
	{
		if (!SUCESS(BCryptOpenAlgorithmProvider(
			lpAlgorithmProvider,
			BCRYPT_MD5_ALGORITHM,
			NULL,
			0)
			))
		{
			m_bIsSomethingWentWorng = true;
		}
	}

	DWORD fnGetPropertyFromAlgorithem(BCRYPT_ALG_HANDLE lpAlgorithmProvider, PCWSTR propertyName)
	{
		DWORD cbData;
		DWORD cbDataLength;

		if(!SUCESS(BCryptGetProperty(
			lpAlgorithmProvider,
			propertyName,
			(PUCHAR)&cbData,
			sizeof(cbData),
			&cbDataLength,
			0
		)))
		{
			m_bIsSomethingWentWorng = true;
		}

		return cbData;
	}

	void fnSetHashMaker()
	{
		DWORD cbSizeOfHashMakerObject;
		
		if (m_bIsSomethingWentWorng) 
		{
			return;
		}

		cbSizeOfHashMakerObject = fnGetPropertyFromAlgorithem(m_lpAlgorithmProvider, BCRYPT_OBJECT_LENGTH);

		//Allocate Memory For Hash Maker
		m_lpbHashMaker = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cbSizeOfHashMakerObject);

		//create Hash maker
		BCryptCreateHash(
			m_lpAlgorithmProvider,
			&m_phHash,
			m_lpbHashMaker,
			cbSizeOfHashMakerObject,
			NULL,
			NULL,
			0
		);
	}
	

	void fnAllocateBufferForHashResult()
	{
		if (m_bIsSomethingWentWorng)
		{
			return;
		}

		m_cbHashValueLen = fnGetPropertyFromAlgorithem(m_lpAlgorithmProvider, BCRYPT_HASH_LENGTH);

		//allocate the buffer for the HASH result value
		m_pbHashResultBuffer = (LPBYTE)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			m_cbHashValueLen);
	}


	BCRYPT_ALG_HANDLE m_lpAlgorithmProvider = NULL;
	PBYTE m_lpbHashMaker = NULL;
	BCRYPT_HASH_HANDLE m_phHash;
	LPBYTE m_pbHashResultBuffer = NULL;

	bool m_bIsSomethingWentWorng = false;


};

