#ifndef A1730_pseudoH
#define A1730_pseudoH
#include "A1730.h"
class A1730_pseudo:public A1730
{
public:
	A1730_pseudo(int _DevNum){};
	~A1730_pseudo(void){};
	DWORD Read(void){return(0);};
	DWORD ReadOut(void){return(0);};
	void Write(DWORD _v){};
};

#endif
