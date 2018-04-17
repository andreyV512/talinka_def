#pragma hdrstop
#include "SignalListDef.h"
#pragma package(smart_init)
SignalListDef* SLD = NULL;

SignalListDef::SignalListDef(TIniFile* _ini) : SignalList(_ini)
{
	iCC = Find("���� ����������", true);
	iLPCHA = Find("���� �� �", true);
	iLPCHRUN = Find("���� �� RUN", true);
	iCSTROBE = Find("����� 2", true);
	iLSTROBE = Find("����� 3", true);
	iCCYCLE = Find("���� 2", true);
	iLCYCLE = Find("���� 3", true);
	iREADY = Find("����������", true);
	iCCONTROL = Find("�������� 2", true);
	iLCONTROL = Find("�������� 3", true);
	iSOP = Find("����� ���", true);

	oLPCHPOW = Find("������� �� ���.3", false);
	oLSCANPOW = Find("������� �� ���.3", false);
	oLSOLPOW = Find("�������������� 3", false);
	oCSOLPOW = Find("�������������� 2", false);
	oCWORK = Find("������ 2", false);
	oLWORK = Find("������ 3", false);
	oCMEAS = Find("��������� 2", false);
	oLMEAS = Find("��������� 3", false);
	oSHIFT = Find("����������", false);
	oCSTROBE = Find("����� ���� 2", false);
	oLSTROBE = Find("����� ���� 3", false);
	oCRESULT = Find("��������� 2", false);
	oLRESULT = Find("��������� 3", false);

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
		AlarmList->Add("��� ������� ���� ����������");
		wasAlarm = true;
	}
	if (LinearCycle)
	{
		if (iLCYCLE->WasConst0(false, 50))
		{
			AlarmList->Add("������ ������ ���������� ����");
			wasAlarm = true;
		}
	}
	if (CrossCycle)
	{
		if (iCCYCLE->WasConst0(false, 50))
		{
			AlarmList->Add("������ ������ ���������� ����");
			wasAlarm = true;
		}
	}
	if (InvA)
	{
		if (iLPCHA->WasConst0(false, 50))
		{
			AlarmList->Add("������: ������ ������ ���� �� �");
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
