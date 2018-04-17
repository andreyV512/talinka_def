#pragma hdrstop
#include "LCardData.h"
#include "uFunctions.h"
#pragma package(smart_init)
#include <cmath.h>
#include <stdlib.h>

LCardData* lcard=NULL;
// ---------------------------------------------------------------------------
LCardData::LCardData(TIniFile* _ini)
{
	ini = _ini;
	lcard = new RLCard502();
	LoadSettings();
}

// ---------------------------------------------------------------------------
LCardData::~LCardData()
{
	delete lcard;
}

// ---------------------------------------------------------------------------
void LCardData::LoadSettings(void)
{
	Parameters.LoadSettings(ini);
	CVoltPercent.resize(Parameters.CSensors);
	LVoltPercent.resize(Parameters.LSensors);
	CZone.resize(Parameters.CSensors);
	LZone.resize(Parameters.LSensors);
	double K[6] =
	{10, 20, 50, 100, 200, 500};
	for (int i = 0; i < Parameters.CSensors; i++)
	{
		CZone[i].reserve(LCard502_INIT_SIZE);
		CVoltPercent[i] = K[Parameters.channels[i].range];
	}
	for (int i = 0; i < Parameters.LSensors; i++)
	{
		LZone[i].reserve(LCard502_INIT_SIZE);
		LVoltPercent[i] = K[Parameters.channels[i + Parameters.CSensors].range];
	}
	IsCrossStarted = false;
	IsLineStarted = false;
	lcard->LoadSettings(&Parameters);
}

// ---------------------------------------------------------------------------
void LCardData::ClearCross(void)
{
	for (unsigned int i = 0; i < CZone.size(); i++)
		CZone[i].clear();
}
// ---------------------------------------------------------------------------
void LCardData::ClearLine(void)
{
	for (unsigned int i = 0; i < LZone.size(); i++)
		LZone[i].clear();
}
// ---------------------------------------------------------------------------
void LCardData::StartCross(void)
{
	IsCrossStarted = true;
	ClearCross();
	lcard->Start();
}

// ---------------------------------------------------------------------------
void LCardData::StopCross(void)
{
	IsCrossStarted = false;
	if (!IsLineStarted)
		lcard->Stop();
}

// ---------------------------------------------------------------------------
void LCardData::StartLine(void)
{
	IsLineStarted = true;
	ClearLine();
	lcard->Start();
}

// ---------------------------------------------------------------------------
void LCardData::StopLine(void)
{
	IsLineStarted = false;
	if (!IsCrossStarted)
		lcard->Stop();
}

// ---------------------------------------------------------------------------
bool LCardData::Read(void)
{
	int size;
	double* buf = lcard->Read(&size);
	if (size < 0)
	{
		LastError=lcard->LastError;
		return (false);
	}
	int packet_size = CZone.size() + LZone.size();
	int packets = size / packet_size;
	if (IsCrossStarted)
	{
		unsigned int size_new = CZone[0].size() + packets;
		if (CZone[0].capacity() < size_new)
		{
			size_new *= (1 + LCard502_ADD_PERCENT / 100);
			for (unsigned int s = 0; s < CZone.size(); s++)
				CZone[s].reserve(size_new);
		}
		for (int p = 0; p < packets; p++)
		{
			double* pbuf = buf + p * packet_size;
			for (unsigned int s = 0; s < CZone.size(); s++)
				CZone[s].push_back(pbuf[s]*CVoltPercent[s]);
		}
	}
	if (IsLineStarted)
	{
		unsigned int size_new = LZone[0].size() + packets;
		if (LZone[0].capacity() < size_new)
		{
			size_new *= (1 + LCard502_ADD_PERCENT / 100);
			for (unsigned int s = 0; s < LZone.size(); s++)
				LZone[s].reserve(size_new);
		}
		for (int p = 0; p < packets; p++)
		{
			double* pbuf = buf + p * packet_size + CZone.size();
			for (unsigned int s = 0; s < LZone.size(); s++) //todo убрать потом!
			{
				if(s == 0)
				{
					srand(time(NULL));
					double coeff = (rand()%6)/100.0+0.9;
					LZone[0].push_back(pbuf[2]*LVoltPercent[0]*coeff);
				}
				else LZone[s].push_back(pbuf[s]*LVoltPercent[s]);
			}
		}
	}
	return (true);
}
// ---------------------------------------------------------------------------
double LCardData::GetValue(int _ch)
{
	lcard->LoadOtherSettings();
	double ret=0;
	for(int i=0;i<5;i++)
		ret+=lcard->GetValue(_ch);
	return(ret/5);
}
