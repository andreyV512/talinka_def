#pragma hdrstop
#include "uCCOMPort_real.h"
#pragma package(smart_init)
#include "uCCOMPort.h"
#pragma package(smart_init)

// ---------------------------------------------------------------------------
CCOMPort_real::CCOMPort_real(AnsiString _port, int _BaudRate, int _ByteSize,
	int _Parity, int _StopBits, int _ReadIntervalTimeout,
	int _ReadTotalTimeoutConstant, int _ReadTotalTimeoutMultiplier,
	AnsiString* _error_msg)
{
	*_error_msg = "";
	WideString w = "\\\\.\\";
	w += _port;
	handleCom = CreateFile(w.c_bstr(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, 0, 0);
	if (handleCom == INVALID_HANDLE_VALUE)
	{
		*_error_msg = "CCOMPort::Create: Не могу открыть порт: ";
		*_error_msg += _port;
		return;
	}
	DCB dcb;
	GetCommState(handleCom, &dcb);
	dcb.BaudRate = _BaudRate;
	dcb.Parity = (BYTE)_Parity;
	dcb.StopBits = (BYTE)_StopBits;
	dcb.ByteSize = (BYTE)_ByteSize;
	if (SetCommState(handleCom, &dcb) == 0)
	{
		*_error_msg = "CCOMPort::Create: ";
		*_error_msg += _port;
		*_error_msg += ": не могу выставить состояние";
		return;
	}
	COMMTIMEOUTS t;
	GetCommTimeouts(handleCom, &t);
	t.ReadIntervalTimeout = _ReadIntervalTimeout;
	t.ReadTotalTimeoutMultiplier = _ReadTotalTimeoutMultiplier;
	t.ReadTotalTimeoutConstant = _ReadTotalTimeoutConstant;
	if (SetCommTimeouts(handleCom, &t) == 0)
	{
		*_error_msg = "CCOMPort::Create: ";
		*_error_msg += _port;
		*_error_msg += ": не могу выставить задержки";
		return;
	}
	if (SetCommMask(handleCom, 0) == 0)
	{
		*_error_msg = "CCOMPort::Create: ";
		*_error_msg += _port;
		*_error_msg += ": не могу выставить маску событий";
		return;
	}
	OnProtocol = NULL;
}

CCOMPort_real* CCOMPort_real::Create(AnsiString _port, int _BaudRate, int _ByteSize,
	int _Parity, int _StopBits, int _ReadIntervalTimeout,
	int _ReadTotalTimeoutConstant, int _ReadTotalTimeoutMultiplier,
	AnsiString* _error_msg)
{
	*_error_msg = "";
	CCOMPort_real* P = new CCOMPort_real(_port,_BaudRate,_ByteSize,_Parity,_StopBits,_ReadIntervalTimeout,
	_ReadTotalTimeoutConstant,_ReadTotalTimeoutMultiplier,_error_msg);
	if((*_error_msg).Length()!=0)
	{
		delete P;
		return(NULL);
	}
	return (P);
}

CCOMPort_real* CCOMPort_real::Create(TIniFile* _ini, AnsiString _ini_section,
	AnsiString* _error_msg)
{
	if (!_ini->ValueExists(_ini_section, "PortName"))
		_ini->WriteString(_ini_section, "PortName", "COM3");
	if (!_ini->ValueExists(_ini_section, "BaudRate"))
		_ini->WriteInteger(_ini_section, "BaudRate", 19200);
	if (!_ini->ValueExists(_ini_section, "DataBits"))
		_ini->WriteInteger(_ini_section, "DataBits", 7);
	if (!_ini->ValueExists(_ini_section, "Parity"))
		_ini->WriteInteger(_ini_section, "Parity", 2);
	if (!_ini->ValueExists(_ini_section, "StopBits"))
		_ini->WriteInteger(_ini_section, "StopBits", 0);
	if (!_ini->ValueExists(_ini_section, "ReadIntervalTimeout"))
		_ini->WriteInteger(_ini_section, "ReadIntervalTimeout", 500);
	if (!_ini->ValueExists(_ini_section, "ReadTotalTimeoutConstant"))
		_ini->WriteInteger(_ini_section, "ReadTotalTimeoutConstant", 500);
	if (!_ini->ValueExists(_ini_section, "ReadTotalTimeoutMultiplier"))
		_ini->WriteInteger(_ini_section, "ReadTotalTimeoutMultiplier", 0);
	AnsiString port = _ini->ReadString(_ini_section, "PortName", "COM3");
	int BaudRate = _ini->ReadInteger(_ini_section, "BaudRate", 19200);
	int ByteSize = _ini->ReadInteger(_ini_section, "DataBits", 7);
	int Parity = _ini->ReadInteger(_ini_section, "Parity", 2);
	int StopBits = _ini->ReadInteger(_ini_section, "StopBits", 0);
	int ReadIntervalTimeout = _ini->ReadInteger(_ini_section,
		"ReadIntervalTimeout", 500);
	int ReadTotalTimeoutConstant =
		_ini->ReadInteger(_ini_section, "ReadTotalTimeoutConstant", 500);
	int ReadTotalTimeoutMultiplier =
		_ini->ReadInteger(_ini_section, "ReadTotalTimeoutMultiplier", 0);

	*_error_msg = "";
	CCOMPort_real* P = new CCOMPort_real(port,BaudRate,ByteSize,Parity,StopBits,ReadIntervalTimeout,
	ReadTotalTimeoutConstant,ReadTotalTimeoutMultiplier,_error_msg);
	if((*_error_msg).Length()!=0)
	{
		delete P;
		return(NULL);
	}
	return (P);
}

// ---------------------------------------------------------------------------
CCOMPort_real::~CCOMPort_real(void)
{
	CloseHandle(handleCom);
}

// ---------------------------------------------------------------------------
bool CCOMPort_real::read_s(unsigned char* _s, int _n)
{
	unsigned long ln = _n;
	ReadFile(handleCom, _s, _n, &ln, NULL);
	if (OnProtocol != NULL)
	{
		AnsiString a;
		for (unsigned long i = 0; i < ln; i++)
		{
			a.printf("r[%d]: %X", (int)i, _s[i]);
			OnProtocol(a);
		}
	}
	if (ln < (unsigned long)_n)
		return (false);
	return (true);
}

// ------------------------------------------------------------------------------
bool CCOMPort_real::write_s(unsigned char* _s, int _n)
{
	unsigned long ln = _n;
	WriteFile(handleCom, _s, _n, &ln, NULL);
	if (OnProtocol != NULL)
	{
		AnsiString a;
		for (unsigned long i = 0; i < ln; i++)
		{
			a.printf("w[%d]: %X", (int)i, _s[i]);
			OnProtocol(a);
		}
	}
	if (ln < (unsigned long)_n)
		return (false);
	return (true);
}

// ------------------------------------------------------------------------------
void CCOMPort_real::ClearBuffer(void)
{
	PurgeComm(handleCom, PURGE_RXCLEAR);
}
