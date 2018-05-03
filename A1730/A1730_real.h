//---------------------------------------------------------------------------
#ifndef A1730_realH
#define A1730_realH
//---------------------------------------------------------------------------
#include "A1730.h"
//#include "Inc/BDaqCL.h"
#include "inc/bdaqctrl.h"
using namespace Automation::BDaq;
/*
class A1730_real:public A1730
{
private:
	BDaqDevice *device;
	BDaqDio *dio;
	long startPort;
	long portCount;
	void pr(AnsiString _msg);
public:
	A1730_real(int _DevNum);
	~A1730_real(void);
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
};
*/
class A1730_real:public A1730
{
private:
	InstantDiCtrl* instantDiCtrl;
	InstantDoCtrl* instantDoCtrl;
	long startPort;
	long portCount;
	void pr(AnsiString _msg);
public:
	A1730_real(int _DevNum);
	~A1730_real(void);
	DWORD Read(void);
	DWORD ReadOut(void);
	void Write(DWORD _v);
};

#endif
