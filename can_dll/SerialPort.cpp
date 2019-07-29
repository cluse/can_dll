

#include "StdAfx.h"  
#include "SerialPort.h"  
#include <process.h>  
#include <iostream>  


CSerialPort::CSerialPort(void)
{
	m_hComm = INVALID_HANDLE_VALUE;
}

CSerialPort::~CSerialPort(void)
{
	ClosePort();
}


//-------------------------------
bool CSerialPort::InitPort(UINT portNo, UINT baud, char parity,
	UINT databits, UINT stopsbits, DWORD dwCommEvents)
{
	char szDCBparam[50];
	sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);

	char szPort[50];
	//sprintf_s(szPort, "COM%d", portNo);
	sprintf_s(szPort, "\\\\.\\COM%d", portNo);

	m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */
		GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
		0,                            /** 共享模式,0表示不共享 */
		NULL,                         /** 安全性设置,一般使用NULL */
		OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
		0,
		0);

	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BOOL bIsSuccess = TRUE;
	/*if (bIsSuccess )
	{
	bIsSuccess = SetupComm(m_hComm,10,10);
	}*/

	COMMTIMEOUTS  CommTimeouts;
	CommTimeouts.ReadIntervalTimeout = 0;
	CommTimeouts.ReadTotalTimeoutMultiplier = 0;
	CommTimeouts.ReadTotalTimeoutConstant = 0;
	CommTimeouts.WriteTotalTimeoutMultiplier = 0;
	CommTimeouts.WriteTotalTimeoutConstant = 0;
	if (bIsSuccess)
	{
		bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);
	}

	DCB  dcb;
	if (bIsSuccess)
	{
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, NULL, 0);
		wchar_t *pwText = new wchar_t[dwNum];
		if (!MultiByteToWideChar(CP_ACP, 0, szDCBparam, -1, pwText, dwNum))
		{
			bIsSuccess = TRUE;
		}
		bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb);
		//dcb.fRtsControl = RTS_CONTROL_ENABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		delete[] pwText;
	}

	if (bIsSuccess)
	{
		bIsSuccess = SetCommState(m_hComm, &dcb);
	}

	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return bIsSuccess == TRUE;
}


//-------------------------------
bool CSerialPort::OpenPort(UINT portNo)
{
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return InitPort(portNo);
	}
	else
	{
		return true;
	}
}

bool CSerialPort::IsOpen()
{
	return (m_hComm != INVALID_HANDLE_VALUE);
}

void CSerialPort::ClosePort()
{
	if (m_hComm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComm);
		m_hComm = INVALID_HANDLE_VALUE;
	}
}


UINT CSerialPort::GetBytesInCOM()
{
	DWORD dwError = 0;
	COMSTAT  comstat;
	memset(&comstat, 0, sizeof(COMSTAT));
	UINT BytesInQue = 0;
	if (ClearCommError(m_hComm, &dwError, &comstat))
	{
		BytesInQue = comstat.cbInQue;
	}
	return BytesInQue;
}

bool CSerialPort::ReadChar(char &cRecved)
{
	BOOL  bResult = TRUE;
	DWORD BytesRead = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);
	if ((!bResult))
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		return false;
	}
	return (BytesRead == 1);
}

bool CSerialPort::WriteData(unsigned char* pData, unsigned int length)
{
	BOOL   bResult = TRUE;
	DWORD  BytesToSend = 0;
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	int len = strlen((const char*)pData);
	bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);
	if (!bResult)
	{
		DWORD dwError = GetLastError();
		PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);
		return false;
	}
	return true;
}