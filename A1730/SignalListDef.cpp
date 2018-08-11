#pragma hdrstop
#include "SignalListDef.h"
#pragma package(smart_init)
SignalListDef* SLD = NULL;

SignalListDef::SignalListDef(TIniFile* _ini) : SignalList(_ini)
{
	iCC = Find("ЦЕПИ УПРАВЛЕНИЯ", true);
	iLPCHA = Find("Прод ПЧ А", true);
	iLPCHRUN = Find("Прод ПЧ RUN", true);
	iCSTROBE = Find("СТРОБ 2", true);
	iLSTROBE = Find("СТРОБ 1", true);
	iCCYCLE = Find("ЦИКЛ 2", true);
	iLCYCLE = Find("ЦИКЛ 1", true);
	iREADY = Find("Готовность", true);
	iCCONTROL = Find("КОНТРОЛЬ 2", true);
	iLCONTROL = Find("КОНТРОЛЬ 1", true);
   //	iSOP = Find("РЕЖИМ СОП", true);
	iSQ1 = Find("SQ21", true);
	iSolidGroup = Find("SolidGroup", true);

  //	oLPCHPOW = Find("ПИТАНИЕ ПЧ МОД.3", false);
	oLSCANPOW = Find("ПИТАНИЕ СУ МОД.3", false);
	//oLSOLPOW = Find("НАМАГНИЧИВАНИЕ 3", false);
	oCSOLPOW = Find("НАМАГНИЧИВАНИЕ", false);
	oCWORK = Find("РАБОТА 2", false);
	oLWORK = Find("РАБОТА 1", false);
	oCMEAS = Find("ИЗМЕРЕНИЕ 2", false);
	oLMEAS = Find("ИЗМЕРЕНИЕ 1", false);
	oSHIFT = Find("ПЕРЕКЛАДКА", false);
	oCSTROBE = Find("СТРОБ ЗОНЫ 2", false);
	oLSTROBE = Find("СТРОБ ЗОНЫ 1", false);
	oCRESULT = Find("РЕЗУЛЬТАТ 2", false);
	oLRESULT = Find("РЕЗУЛЬТАТ 1", false);
	
	oSTF = Find("STF", false);
	oRL  = Find("RL", false);
	oRM  = Find("RM", false);
    oRH  = Find("RH", false);
	
	CrossCycle = false;
	LinearCycle = false;
	InvA = false;
}

void SignalListDef::SetCrossCycle(bool _v)
{
	cs->Enter();
	{
		CrossCycle = _v;
	} cs->Leave();
}

void SignalListDef::SetLinearCycle(bool _v)
{
	cs->Enter();
	{
		LinearCycle = _v;
	} cs->Leave();
}

void SignalListDef::SetInvA(bool _v)
{
	cs->Enter();
	{
		InvA = _v;
	} cs->Leave();
}

bool SignalListDef::CheckAlarm(void)
{
	if (iCC->WasConst0(false, 50))
	{
		AlarmList->Add("Нет сигнала цепи управления");
		wasAlarm = true;
	}
	if (LinearCycle)
	{
		if (iLCYCLE->WasConst0(false, 50))
		{
			AlarmList->Add("Пропал сигнал Продольный Цикл");
			wasAlarm = true;
		}
	}
	if (CrossCycle)
	{
		if (iCCYCLE->WasConst0(false, 50))
		{
			AlarmList->Add("Пропал сигнал Поперечный цикл");
			wasAlarm = true;
		}
	}
	if (InvA)
	{
		if (iLPCHA->WasConst0(false, 50))
		{
			AlarmList->Add("Авария: пропал сигнал Прод ПЧ А");
			wasAlarm = true;
		}
	}
	if (wasAlarm)
		prAlarm();
	return (wasAlarm);
}

void SignalListDef::SetAlarm(bool _on)
{
	SetAlarm0(_on);
	cs->Enter();
	{
		InvA = false;
	} cs->Leave();
}

void SignalListDef::Drop(void)
{
	if (oCSTROBE->WasConst0(true, 100))
		oCSTROBE->Set0(false);
	if (oLSTROBE->WasConst0(true, 100))
		oLSTROBE->Set0(false);
	if (oCRESULT->WasConst0(true, 100))
		oCRESULT->Set0(false);
	if (oLRESULT->WasConst0(true, 100))
		oLRESULT->Set0(false);

}
