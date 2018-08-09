// ---------------------------------------------------------------------------

#pragma hdrstop

#include "ThreadOnLine.h"
#include "Solenoid.h"
#include "SignalListDef.h"
#include "Inverter.h"
#include "Global.h"
#include <IOUtils.hpp>
#include "uFunctions.h"
#include "LCardData.h"
#include "Singleton.h"
#include "DebugMess.h"
#include "Main.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

// ----����������� - ��������� ������� ���������� �� ����������---------------
__fastcall ThreadOnLine::ThreadOnLine(bool CreateSuspended, bool _Linear,
	CBank* _BankCross, CBank* _BankLine, HWND _main_handle, UINT _thread_msg,
	TIniFile* _ini, vector<double> *_SGbuffer)
{
	Collect = true;
	Cross = true;
	Linear = _Linear;
	ini = _ini;
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	InSpeed = ini->ReadInteger(sect, "InSpeed", 4);
	WorkSpeed = ini->ReadInteger(sect, "WorkSpeed", 4);
	OutSpeed = ini->ReadInteger(sect, "OutSpeed", 4);
	delay = ini->ReadInteger("OtherSettings", "OnLineCycleDelay", 10);
	pauseWorkSpeed = ini->ReadInteger(sect, "PauseWorkSpeed", 1050);
	pauseStop = ini->ReadInteger(sect, "PauseStop", 500);
	BankCross = _BankCross;
	BankLine = _BankLine;
	main_handle = _main_handle;
	thread_msg = _thread_msg;
	cs = new TCriticalSection();
	calc_event = new TEvent(NULL, false, false, "calc_event", true);
	SGbuffer = _SGbuffer;
	unitBaseMMCross = ini->ReadInteger("unitBase", "unitBaseMMCross", 1830);
	unitBaseMMLong = ini->ReadInteger("unitBase", "unitBaseMMLong", 500);
}

// ---------------------------------------------------------------------------
__fastcall ThreadOnLine::~ThreadOnLine(void)
{
	delete calc_event;
	delete cs;
}

// -----������ ������ ������--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
//.�������� ini->ReadInteger("PP", "spIsSolenoidsON", 27);
	NameThreadForDebugging("WorkOnlineThread");
	SetStext1("����� \"������ (�)\"");
	SetStext2("������� � ��������� ���������");
	TPr::pr("-----------");
	TPr::pr(stext1);
	Post(UPDATE_STATUS);

	UnicodeString prepare_result = PrepareForWork();
	if (prepare_result != "ok")
	{
		SetStext1("����� \"������ (�)\" �� ��������!");
		SetStext2(prepare_result);
		TPr::pr("������ (�): " + stext2);
		Post(UPDATE_STATUS);
		Finally();
		Post(COMPLETE, 0);
		return;
	}
	TPr::pr("������ (�): ���������� ������ �������");
//	//��������� �� ��� �� ���
//	Singleton->isSOP = SLD->iSOP->WasConst(true,50);
//	if(Singleton->isSOP) pr("THIS IS SOP!!!"); //todo ������ ����� �������
	SLD->BlockDrop(false);
	bool ret = OnlineCycle();
	Post(COMPUTE);
	TPr::pr("���� ����������� �� Main");
	calc_event->WaitFor(INFINITE);
	calc_event->ResetEvent();
	if (Terminated)
	{
		Post(COMPLETE, 0);
		return;
	}
	TPr::pr("�������� ����������");
	if (!ret)
	{
		SetStext1("����� \"������ (�)\" �� ��������!");
		TPr::pr("����� \"������ (�)\" �� ��������!");
		Post(UPDATE_STATUS);
		Finally();
		Post(COMPLETE, 0);
		return;
	}
	SLD->BlockDrop(true);
	if (IsBrak)
	{
		SLD->oCSTROBE->Set(false);
		SLD->oLSTROBE->Set(false);
	}
	else
	{
		SLD->oCSTROBE->Set(false);
		SLD->oLSTROBE->Set(true);
	}
	Sleep(500);

	SLD->oSHIFT->Set(true);
	Post(COMPLETE, (LPARAM)1);
}

void ThreadOnLine::Post(WPARAM _w, LPARAM _l)
{
	if (PostMessage(main_handle, thread_msg, _w, _l) == 0)
		FATAL("ThreadOnLine::Post: �� ���� ������� ���������");
	AnsiString a = "ThreadOnLine::Post: ������ ������� ";
	a += _w;
	a += " ";
	a += _l;
	TPr::pr(a);
}

// ---------------------------------------------------------------------------
void __fastcall ThreadOnLine::UpdateStatusBarBottom()
{

	 MainForm->StatusBarBottom->Panels->Items[2]->Text = updateStatusBarBottomParam;
	 MainForm->StatusBarBottom->Refresh();
}
// -----���������� � ������ �� ������ ������ �� �������� �����----------------
UnicodeString ThreadOnLine::PrepareForWork()
{
	double t = 0;
	if(!CrossSolenoid->Solenoid1U(t))
	{
		char *a = "�������� ����������� ��������� 1";
		TPr::pr(a);
		char buf[128];
		 sprintf(buf, "%s %.1f", a, t);
		 updateStatusBarBottomParam = buf;
		 Synchronize(&UpdateStatusBarBottom);
		 return a;
	}
	if(!CrossSolenoid->Solenoid2U(t))
	{
		char *a = "�������� ����������� ��������� 2";
		TPr::pr(a);
		char buf[128];
		sprintf(buf, "%s %.1f", a, t);
		updateStatusBarBottomParam = buf;
		 Synchronize(&UpdateStatusBarBottom);
		 return a;
	}
   //	SLD->oLPCHPOW->Set(true);
	// ���� ��� ����
	SetStext2("���� ������� \"����� ����\"");
	Post(UPDATE_STATUS);

	if (!SLD->iCCYCLE->Wait(true, INFINITE))
		return "�� ��������� ������� ����� ����!";
	SLD->SetAlarm(true);
	SLD->SetCrossCycle(true);
	if (Linear)
	{
		SetStext2("���� ������� \"���� ����\"");
		Post(UPDATE_STATUS);

		if (!SLD->iLCYCLE->Wait(true, INFINITE))
			return "�� ��������� ������� ���� ����!";
		SLD->SetLinearCycle(true);
	}
	SetStext2("���� ����������");
	Post(UPDATE_STATUS);

	SLD->oSHIFT->Set(true);
	TPr::pr("���������� oSHIFT");
	TPr::pr("���� ������  ���������� ����������");
	if (!SLD->iREADY->Wait(true, INFINITE))
		return ("�� ��������� ������� ����������!");
	TPr::pr("�������� ����������");
	Post(NEXT_TUBE);

	SLD->oSHIFT->Set(false);
	SLD->oCSTROBE->Set(false);
	SLD->oLSTROBE->Set(false);
	SLD->oLRESULT->Set(false);
	SLD->oCRESULT->Set(false);

	SLD->oCSOLPOW->Set(false);
	/*
	if (Linear)
	{
		Sleep(600);
		SLD->oLSOLPOW->Set(true);
	}
	*/
	//Sleep(500);


	SLD->oCSOLPOW->Set(true);
	Sleep(500);
	 AnsiString a = "�������� ����������: ";
	bool solinoidOn = CrossSolenoid->SolenoidOn();
	if(solinoidOn)
	{
		a += "�������";
	}
	else
	{
		a += "��������";
	}
	TPr::pr(a);
	if(!solinoidOn)
	{
		char *a = "��� ���� ����������� ���������";
		TPr::pr(a);
		 updateStatusBarBottomParam = a;
		 Synchronize(&UpdateStatusBarBottom);
		return a;
	}
   /*
	AnsiString a = "�������� ����������: ";
	a += CrossSolenoid->GetUIR();
	TPr::pr(a);
	if (!CrossSolenoid->OkU())
	{
		SLD->oCSOLPOW->Set(false);
		return "���������� ����������� ��������� ��� ���������";
	}
	if (!CrossSolenoid->OkResist())
	{
		SLD->oCSOLPOW->Set(false);
		return "������������� ����������� ��������� ��������� �����";
	}
	*/
	/*
	if (Linear)
	{
		a = "�������� ����������: ";
		a += LinearSolenoid->GetUIR();
		TPr::pr(a);
		if (!LinearSolenoid->OkU())
		{
			SLD->oLSOLPOW->Set(false);
			return "���������� ����������� ��������� ��� ���������";
		}
		if (!LinearSolenoid->OkResist())
		{
			SLD->oLSOLPOW->Set(false);
			return "������������� ����������� ��������� ��������� �����";
		}
	}
	*/
	// ������ ����������
	if (Linear)
	{
		SLD->oLSCANPOW->Set(true);
		// �������� �������� ������ �� ����� � ������
		TPr::pr(AnsiString("�������� �������� InSpeed ") + AnsiString(InSpeed));
		if (!frConverter->setParameterSpeed(Globals::defaultRotParameter,
			InSpeed))
			return "�� ������ ������ ������� �������� �������� ����������� ������";

		if (!frConverter->startRotation())
			return "�� ������� �������� �������� ����������� ������";
		if (!SLD->iLPCHRUN->Wait(false, 5000))
			return "�� ���������� �������� �������� ����������� ������!";
	  //	SLD->SetInvA(true);
	}
	lcard->LoadSettings();
	return "ok";
}

// -------------------------------------------------------------------------------
// ----������ ����, ���������� ���������� � ����������� ��� �������---------------
bool ThreadOnLine::OnlineCycle()
{
	TPr::pr("������ (�): ����� ������");
	// ���������� ������� ������ �� �������
	SLD->oCWORK->Set(true);
	if (Linear)
		SLD->oLWORK->Set(true);

	SetStext2("���� ����� � �������");
	TPr::pr("������ (�): " + stext2);
	Post(UPDATE_STATUS);

	// ����� ������� �� ����� ��������� �����
	bool InSG = false; // ����� ����� � ���������������
	bool result = true; // ��������� ����� �����

	bool ppStarted = false;
	bool ppSignaled = false;
	bool ppIsStoped = false;

	bool prStarted = false;
	bool prSignaled = false;
	bool prIsStoped = false;
	bool ToFinish = false;

	bool TubeQuit = false;
	// ���������� ��� ����������� ������� �� 50��
	// DWORD LinearResult;
	// DWORD CrossResult;
	DWORD FirstLinerStrobeTick = 0;
	DWORD CycleTick = 0;
	DWORD FinishTick = 0;
	bool isTwoSpeed = false;
	bool isFreeSpeed = false;
	bool isOutSpeed = false;
	DWORD ControlOffTime = 0;
	int CrossStrobes = 0;
	int LineStrobes = 0;
	unsigned crossTimeControl = 0;
	unsigned longTimeControl = 0;

	unsigned delayCrossTimeControl = 0;
	unsigned delayLongTimeControl = 0;

	unsigned SQ1TimeControl = 0;
	double speedTube = 0.4;
	bool SLD_iCSTROBE_Get = false;  // �������� �� ���������� ���
	bool SLD_iLSTROBE_Get = false;

	int crossZoneCounter = 0;

	while (Collect)
	{
		Sleep(delay);

		// �������� �� ���������� ���
		unsigned tick = GetTickCount();
		if(!SQ1TimeControl && SLD->iSQ1->Get())
		{
			SQ1TimeControl = tick;
			dprint("SQ1TimeControl %d\n", SQ1TimeControl);
		}

		if(crossTimeControl > 0)
		{
		double p = speedTube * (int)(tick - crossTimeControl);
		//dprint("crossTimeControl %f\n", p);
			if(p > 200)
			{
				crossTimeControl += int((400.0 - p) / speedTube);

		   //		dprint("SLD_iCSTROBE_Get ON\n");
				SLD_iCSTROBE_Get = true;
			}
			else if(p > 100)
			{
			//	dprint("SLD_iCSTROBE_Get OFF\n");
				SLD_iCSTROBE_Get = false;
			}
		}
		else  if(delayCrossTimeControl > 0)
		{
				double p = speedTube * (int)(tick - delayCrossTimeControl);

				if(p > 350)
				{
					crossTimeControl = tick;
					dprint("delayCrossTimeControl %f\n", p);
				}
		}

		//p = speedTube * (int)(tick - longTimeControl);
		if(longTimeControl > 0)
		{
			double p = speedTube * (int)(tick - longTimeControl);
			if(p > 200)
			{
			  longTimeControl += int((400.0 - p) / speedTube);
			 //  dprint("SLD_iCSTROBE_Get ON\n");
			 SLD_iLSTROBE_Get = true;
			}
			else if(p > 100)
			{
	   //		dprint("SLD_iCSTROBE_Get OFF\n");
				SLD_iLSTROBE_Get = false;
			}
		}
		else  if(delayLongTimeControl > 0)
		{
				double p = speedTube * (int)(tick - delayLongTimeControl);

				if(p > 400)
				{
					longTimeControl = tick;
					dprint("delayLongTimeControl %f\n", p);
				}
		}
			// �������� �� ���������� ��� �����

		if (Cross && !ppIsStoped)
		{
			// -----------------------------------------------------------------------------------------------------------------
			// ������ ����������� �����
			if (!ppStarted && SLD->iCCONTROL->Get())
			{
				AnsiString a;
				a = "iCCONTROL ";
				a += GetTickCount();
				TPr::pr(a);
				ppStarted = true;
				lcard->StartCross();
				SLD->oCMEAS->Set(true);
				SetStext2("������ ���� �����������");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);
				//��������� �� ��� �� ���
		   //		Singleton->isSOP = SLD->iSOP->WasConst(true,50);
		   //		if(Singleton->isSOP) pr("THIS IS SOP!!!"); //todo ������ ����� �������

				delayCrossTimeControl = GetTickCount();
				speedTube = (double)unitBaseMMCross / (delayCrossTimeControl - SQ1TimeControl);
				dprint("cross Speed %f\n", speedTube);
			}
			// ����������� ���������� ������
			if (!ppSignaled && ppStarted && SLD_iCSTROBE_Get)//SLD->iCSTROBE->Get())  // �������� �� ���������� ���
			{
				CrossStrobes++;
				TPr::pr(AnsiString("���������� ������� ") + CrossStrobes);
				ppSignaled = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("������: ") + lcard->LastError,
					&Collect, &result);
				else
				{
					 ++crossZoneCounter;
					BankCross->AddZone(lcard->GetPointCross());
					lcard->ClearCross();
					Post(REDRAW, REDRAW_CROSS);
				}
			}
			if (ppSignaled && !SLD_iCSTROBE_Get)//SLD->iCSTROBE->Get())  // �������� �� ���������� ���
				ppSignaled = false;
			if (ppStarted && !SLD->iCCONTROL->Get())
			{
				ppIsStoped = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("������: ") + lcard->LastError,
					&Collect, &result);
				else
				{
					BankCross->AddZone(lcard->GetPointCross());
					lcard->ClearCross();
					Post(REDRAW, REDRAW_CROSS);
				}
				SLD->SetCrossCycle(false);
				lcard->StopCross();
				SetStext2("���������� ���� � �����������");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);
				if (!Linear)
				{
					SLD->oCSOLPOW->Set(false);
					pr("����� oCSOLPOW");
				}
			}
		} // -----------------------------------------------------------------------------------------------------------------
		if (Linear && !prIsStoped)
		{
			CycleTick = GetTickCount();
			// ������ ����������� �����
			if (!prStarted && SLD->iLCONTROL->Get())
			{
				prStarted = true;
				FirstLinerStrobeTick = CycleTick;
				SLD->oLMEAS->Set(true);
				lcard->StartLine();
				SetStext2("������ ���� � �����������");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);

				delayLongTimeControl = GetTickCount();
				speedTube = (double)unitBaseMMLong / (delayLongTimeControl - SQ1TimeControl);
				dprint("long Speed %f\n", speedTube);

			}
			if (prStarted && SLD_iLSTROBE_Get)//SLD->iLSTROBE->Get()) // �������� �� ���������� ���
			{
				if (!prSignaled && prStarted)
				{
					LineStrobes++;
					TPr::pr(AnsiString("���������� ������� ") + LineStrobes);
					prSignaled = true;
					if (!lcard->Read())
						ErrFinally(AnsiString("������: ") + lcard->LastError,
						&Collect, &result);
					else
					{
						BankLine->AddZone(lcard->GetPointLine());
						lcard->ClearLine();
						Post(REDRAW, REDRAW_LINE);
					}
				}
			}
			if (prSignaled && !SLD_iLSTROBE_Get)//SLD->iLSTROBE->Get())   // �������� �� ���������� ���
				prSignaled = false;
			if (!isTwoSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseStop))
			{
				isTwoSpeed = true;
				TPr::pr(String("�������� ���������� OutSpeed ") +
					String(OutSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, OutSpeed))
					ErrFinally("������: �� ������� �������� ����������",
					&Collect, &result);
			}

			if (!isFreeSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseWorkSpeed))
			{
				isFreeSpeed = true;
				TPr::pr(String("�������� � ������ WorkSpeed ") +
					String(WorkSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, WorkSpeed))
					ErrFinally("������: �� ������� �������� ������� ��������",
					&Collect, &result);
			}
			if (!isOutSpeed && prStarted && !SLD->iLCONTROL->Get())
			{
				longTimeControl = 0;
				delayLongTimeControl = 0;
				isOutSpeed = true;
				ControlOffTime = CycleTick;
				TPr::pr("�������� ������� � ����� ������ InSpeed " +
					String(InSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, InSpeed));
				//	ErrFinally("������: �� ������� �������� �������� ��������",
				 //	&Collect, &result);
			}
			/*
			if (isOutSpeed && ((CycleTick - ControlOffTime) > 1000))
			{
				prIsStoped = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("������: ") + lcard->LastError,
					&Collect, &result);
				else
				{
					BankLine->AddZone(lcard->GetPointLine());
					lcard->ClearLine();
					lcard->StopLine();
					Post(REDRAW, REDRAW_LINE);
				}
				SLD->SetLinearCycle(false);
			//	SLD->SetInvA(false);
				if (!frConverter->stopRotation())
					ErrFinally("������: �� ������� ��������� ��������",
					&Collect, &result);
				else
				{
					SetStext2("���������� ���� � �����������");
					TPr::pr(stext2);
					Post(UPDATE_STATUS);
				}
			 //	SLD->oLSOLPOW->Set(false);
			//	pr("����� oLSOLPOW");
				SLD->oCSOLPOW->Set(false);
				pr("����� oCSOLPOW");
				SLD->oLSCANPOW->Set(false);
				pr("����� oLSCANPOW");
			}
			*/
		}
		// -----------------------------------------------------------------------------------------------------------------
		// �������, ��� ����� ����� �� ���������
		if (ppStarted && !SLD->iCCONTROL->Get() && (!Linear ||
			!SLD->iLCONTROL->Get()) && !ToFinish)
		{
		SLD_iLSTROBE_Get = false;
		SLD_iCSTROBE_Get = false;
		if(crossZoneCounter > 3)
		{
			lcard->StopCross();
			lcard->StopLine();
		  //	/*
			BankLine->zones = crossZoneCounter - 3;
			BankCross->zones = crossZoneCounter - 3;
			BankCross->last = BankCross->zones;
			BankLine->last = BankLine->zones;
		//	*/
			lcard->GetPointCross()->resize(crossZoneCounter - 3);
			lcard->GetPointLine()->resize(crossZoneCounter - 3);
			BankLine->Source.resize(crossZoneCounter - 3);
			BankCross->Source.resize(crossZoneCounter - 3);
			Singleton->LinearResult->zones = crossZoneCounter - 3;
			Singleton->CrossResult->zones = crossZoneCounter - 3;
			lcard->ClearLine();
			lcard->ClearCross();
			Post(REDRAW, REDRAW_LINE);
			Post(REDRAW, REDRAW_CROSS);
        }
			crossTimeControl = 0;
			delayCrossTimeControl = 0;
			SetStext2("����� ����� �� ���������");
			Post(UPDATE_STATUS);
			SLD->oCSOLPOW->Set(false);
		  //	SLD->oLSOLPOW->Set(false);
			SLD->oLSCANPOW->Set(false);
			ToFinish = true;
			FinishTick = GetTickCount();
			// Finally();
			// Collect = false;
			TPr::pr(stext2);
			pr("�������� �� ������");
		}
		if (ToFinish)
		{
			if (GetTickCount() - FinishTick > 1000)
			{
				Finally();
		   //		if (SLD->iLPCHRUN->Get())
			  //		SLD->oLPCHPOW->Set(false);
				Collect = false;
				pr("�������� �� ������ ���������");
			}
		}
		// -----------------------------------------------------------------------------------------------------------------
		// �������, �� ���� �� ������
		if (Terminated)
		{
			Collect = false;
			result = false;
			TPr::pr("������ (�): ����� � Terminated");
			Finally();
		}
		// �������, �� ���� �� ������
		if (SLD->WasAlarm())
		{
			ErrFinally("������ (�): ����� �� ������", &Collect, &result);
		}
	}
	TPr::pr("OnlineCycle ��������");
	Post(SGDRAW);
	return result;
}

void ThreadOnLine::Finally()
{
	frConverter->stopRotation();
	TPr::pr("Finally() ������ ��������");
	lcard->StopCross();
	lcard->StopLine();
	SLD->SetCrossCycle(false);
	SLD->SetLinearCycle(false);
	SLD->oLWORK->Set(false);
	SLD->oLMEAS->Set(false);
	SLD->oCWORK->Set(false);
	SLD->oCMEAS->Set(false);
  //	SLD->oLSOLPOW->Set(false);
	SLD->oCSOLPOW->Set(false);
	SLD->oLSCANPOW->Set(false);
	SLD->oSHIFT->Set(false);
}

void ThreadOnLine::ErrFinally(AnsiString _msg, bool* _Collect, bool* _result)
{
	frConverter->stopRotation();
 //	SLD->oLPCHPOW->Set(false);
	Finally();
	*_Collect = false;
	*_result = false;
	TPr::pr(_msg);
	SetStext2(_msg);
	Post(UPDATE_STATUS);
}

// ---------------------------------------------------------------------------
AnsiString ThreadOnLine::GetStext1(void)
{
	AnsiString ret;
	cs->Enter();
	{
		ret = stext1;
	} cs->Leave();
	return (ret);
}

// ---------------------------------------------------------------------------
AnsiString ThreadOnLine::GetStext2(void)
{
	AnsiString ret;
	cs->Enter();
	{
		ret = stext2;
	} cs->Leave();
	return (ret);
}

// ---------------------------------------------------------------------------
void ThreadOnLine::SetStext1(AnsiString _s)
{
	cs->Enter();
	{
		stext1 = _s;
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void ThreadOnLine::SetStext2(AnsiString _s)
{
	cs->Enter();
	{
		stext2 = _s;
	} cs->Leave();
}

// ---------------------------------------------------------------------------
void ThreadOnLine::SetCalc(bool _IsBrak)
{
	IsBrak = _IsBrak;
	calc_event->SetEvent();
}

// ---------------------------------------------------------------------------
void ThreadOnLine::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}
// ----------------------------------------------------------------------------
