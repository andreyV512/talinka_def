// ---------------------------------------------------------------------------
#ifndef A1730H
#define A1730H
#include <System.hpp>
class A1730
{
public:
	virtual ~A1730(void){};
	virtual DWORD Read(void)=0;
	virtual DWORD ReadOut(void)=0;
	virtual void Write(DWORD _v)=0;
};
#endif
