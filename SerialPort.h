#pragma once


#ifndef SERIALPORT_H_  
#define SERIALPORT_H_  

#include <Windows.h>  


class CSerialPort
{
public:
	CSerialPort(void);
	~CSerialPort(void);

public:
	bool OpenPort(UINT  portNo);

	bool IsOpen();

	void ClosePort();

	bool WriteData(unsigned char* pData, unsigned int length);

	UINT GetBytesInCOM();

	bool ReadChar(char &cRecved);

private:
	bool InitPort(UINT  portNo = 1, UINT  baud = CBR_115200, char  parity = 'N', UINT  databits = 8, UINT  stopsbits = 1, DWORD dwCommEvents = EV_RXCHAR);

	HANDLE  m_hComm;

};

#endif //SERIALPORT_H_ 
