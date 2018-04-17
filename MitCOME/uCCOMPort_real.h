#ifndef uCCOMPort_realH
#define uCCOMPort_realH
// ---------------------------------------------------------------------------
#include<system.hpp>
#include <IniFiles.hpp>
#include "uCCOMPort.h"

// ---------------------------------------------------------------------------
class CCOMPort_real : public CCOMPort
{
private:
	AnsiString COM;
	HANDLE handleCom;
	bool verbal;

	CCOMPort_real(AnsiString _port, int _BaudRate, int _ByteSize, int _Parity,
		int _StopBits, int _ReadIntervalTimeout, int _ReadTotalTimeoutConstant,
		int _ReadTotalTimeoutMultiplier,AnsiString* _error_msg);

public:
	static CCOMPort_real* Create(AnsiString _port, int _BaudRate, int _ByteSize,
		int _Parity, int _StopBits, int _ReadIntervalTimeout,
		int _ReadTotalTimeoutConstant, int _ReadTotalTimeoutMultiplier,
		AnsiString* _error_msg);
	static CCOMPort_real* Create(TIniFile* _ini, AnsiString _ini_section,
		AnsiString* _error_msg);
	~CCOMPort_real(void);
	bool read_s(unsigned char* _s, int _n);
	bool write_s(unsigned char* _s, int _n);
	void ClearBuffer(void);
};

#endif
