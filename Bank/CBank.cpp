// ---------------------------------------------------------------------------
#pragma hdrstop
#include "CBank.h"
#include "Protocol.h"
#include "uFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CBank::CBank(int _max_zones, int _sensors)
{
	cs = new TCriticalSection();
	if (_max_zones < 0)
		_max_zones = 0;
	if (_sensors < 0)
		_sensors = 0;
	sensors = (unsigned int)_sensors;
	Source.resize(_max_zones);
	for (int i = 0; i < _max_zones; i++)
		Source[i].resize(sensors);
	zones = 0;
	last = 0;
}

// ---------------------------------------------------------------------------
CBank::~CBank(void)
{
	delete cs;
}

// ---------------------------------------------------------------------------
void CBank::Clear(void)
{
	cs->Enter();
	{
		for (unsigned int i = 0; i < Source.size(); i++)
		{
			for (unsigned int j = 0; j < Source[i].size(); j++)
				Source[i][j].clear();
		}
		last = 0;
		zones = 0;
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void CBank::AddZone(vector<vector<double> >* new_data)
{
	AnsiString b;
	b="CBank::AddZone: sensors=";
	b+=sensors;
	TPr::pr(b);
	cs->Enter();
	{
		if (zones >= Source.size())
		{
			AnsiString a = "CBank::AddZone: Количество зон ";
			a += zones;
			a += " больше или равно максимума ";
			a += Source.size();
			FATAL(a);
		}
		if (sensors != (*new_data).size())
		{
			AnsiString a = "CBank::AddZone: Количество датчиков ";
			a += (*new_data).size();
			a += " не равно ";
			a += sensors;
			FATAL(a);
		}
		for (unsigned int i = 0; i < sensors; i++)
			Source[zones][i] = (*new_data)[i];
		zones++;
	} cs->Leave();
	TPr::pr("CBank::AddZone: finished");
}

// ---------------------------------------------------------------------------
int CBank::GetNextZone(vector<vector<vector<double> > > * _source)
{
	int ret=-1;
	cs->Enter();
	{
		if (zones == 0)
		{
			ret = -1;
			goto M1;
		}
		if (last == zones)
		{
			ret = -1;
			goto M1;
		}
		for (unsigned int s = 0; s < sensors; s++)
			(*_source)[last][s]=Source[last][s];
		ret=last;
		last++;
M1:
	} cs->Leave();
	AnsiString a="CBank::GetNextZone ret=";
	a+=ret;
	TPr::pr(a);
	return (ret);
}
