// ------------------------------------------------------------------------------

#pragma hdrstop

#include "Inverter.h"
#include <IniFiles.hpp>
#include "Global.h"
#include "SignalListDef.h"
//#include "A1730.h"

// ------------------------------------------------------------------------------

#pragma package(smart_init)
Inverter *frConverter;

Inverter::Inverter()
{
/*
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	AnsiString ini_section = "Inverter";
	if (!ini->ValueExists(ini_section, "Abonent"))
		ini->WriteInteger(ini_section, "Abonent", 2);
	if (!ini->ValueExists(ini_section, "timeout"))
		ini->WriteInteger(ini_section, "timeout", 1);
	inverter = new CInv();
//	inverter->OnProtocol = OnProtocol;
	inverter->abonent = ini->ReadInteger(ini_section, "Abonent", 2);
	inverter->timeout = ini->ReadInteger(ini_section, "timeout", 1);
	inverter->Open(ini,"Inverter");
	delete ini;
	testThread = NULL;
	cs = new TCriticalSection();
	*/
}

Inverter::~Inverter()
{
/*
	if (testThread != NULL)
		delete testThread;
	delete inverter;
	delete cs;
	*/
}

bool Inverter::setParameterSpeed(int number, int value)
{
/*
	if ((number < 4) || (number > 6))
		return false;
	bool ret;
	cs->Enter();
	{
		ret = inverter->SetFrequency(IntToStr(80 + number), value * 100);
	} cs->Leave();
	return (ret);
	*/
	if((unsigned)value > 7) value = 7;
	SLD->oRL->Set(0 != (value & 1));
	SLD->oRM->Set(0 != (value & 2));
	SLD->oRH->Set(0 != (value & 4));
	return true;
}

// ------------------------------------------------------------------------------
int Inverter::getParameterSpeed(int number)
{
/*
	int ret;
	cs->Enter();
	{
		ret = (int)inverter->GetFrequency("0" + IntToStr(number)) / 100;
	} cs->Leave();
	return (ret);
	*/
	unsigned t = (SLD->oRL->Get() ? 1: 0)
			   | (SLD->oRM->Get() ? 2: 0)
			   | (SLD->oRH->Get() ? 4: 0)
	;
	return t;
}

// ------------------------------------------------------------------------------
bool Inverter::startRotation()
{
/*
	if (testThread != NULL)
		return (true);
	inverter->Reset9966();
	Sleep(1000);
	inverter->SetMode(0);
	inverter->oSTF = true;
	inverter->oRH = true;
	inverter->oMRS = false;
	if (!inverter->StateWrite())
		return false;
	testThread = new rotationThread(inverter, cs);
	return true;
	*/
	bool b = SLD->oRL->Get() || SLD->oRM->Get() || SLD->oRH->Get();
	SLD->oSTF->Set(true);
	return b;
}

// ------------------------------------------------------------------------------
bool Inverter::stopRotation()
{
/*
	if (testThread == NULL)
		return (true);
	delete testThread;
	testThread = NULL;
	inverter->oSTF = false;
	inverter->oRH = false;
	if (!inverter->StateWrite())
		return false;
	return true;
	*/
	SLD->oSTF->Set(false);

	SLD->oRL->Set(false);
	SLD->oRM->Set(false);
	SLD->oRH->Set(false);
	return true;
}

// ------------------------------------------------------------------------------
bool Inverter::stateRead()
{
/*
	bool ret;
	cs->Enter();
	{
		ret = inverter->StateRead();
	} cs->Leave();
	return (ret);
	*/
	return true;
}

// ------------------------------------------------------------------------------
bool Inverter::NETManage()
{
/*
	bool ret;
	cs->Enter();
	{
		ret = inverter->SetMode(0);
	} cs->Leave();
	return (ret);
	*/
	return true;
}

// ------------------------------------------------------------------------------
bool Inverter::ResetErrors()
{
/*
	bool ret;
	cs->Enter();
	{
		ret = inverter->Reset9966();
	} cs->Leave();
	return (ret);
	*/
	return true;
}

void Inverter::OnProtocol(AnsiString _msg)
{
	TPr::pr(_msg);
}

// ******************************************************************************
/*
__fastcall rotationThread::rotationThread(CInv* _inv, TCriticalSection* _cs)
{
	inv = _inv;
	cs = _cs;
}

void __fastcall rotationThread::Execute()
{
	for (; ;)
	{
		cs->Enter();
		{
			inv->StateRead();
		} cs->Leave();
		if (Terminated)
			break;
		Sleep(500);
	}
}
*/
