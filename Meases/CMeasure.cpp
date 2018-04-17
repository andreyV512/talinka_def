#pragma hdrstop
#include "CMeasure.h"
#pragma package(smart_init)
CMeasure::CMeasure(void)
{
	v=0;
}
CMeasure::CMeasure(const CMeasure& _measure)
{
	v=_measure.v;
}
CMeasure& CMeasure:: operator=(const CMeasure&_measure)
{
	if(this==&_measure)
		return (*this);
	v=_measure.v;
	return (*this);
}
