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

// ----конструктор - переносим внешние переменные на внутренние---------------
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

// -----запуск потока работы--------------------------------------------------
void __fastcall ThreadOnLine::Execute()
{
//.вставить ini->ReadInteger("PP", "spIsSolenoidsON", 27);
	NameThreadForDebugging("WorkOnlineThread");
	SetStext1("Режим \"Работа (Д)\"");
	SetStext2("Готовим к исходному положению");
	TPr::pr("-----------");
	TPr::pr(stext1);
	Post(UPDATE_STATUS);

	UnicodeString prepare_result = PrepareForWork();
	if (prepare_result != "ok")
	{
		SetStext1("Режим \"Работа (Д)\" не завершен!");
		SetStext2(prepare_result);
		TPr::pr("Работа (Д): " + stext2);
		Post(UPDATE_STATUS);
		Finally();
		Post(COMPLETE, 0);
		return;
	}
	TPr::pr("Работа (Д): Подготовка прошла успешно");
//	//Проверяем не СОП ли это
//	Singleton->isSOP = SLD->iSOP->WasConst(true,50);
//	if(Singleton->isSOP) pr("THIS IS SOP!!!"); //todo убрать после отладки
	SLD->BlockDrop(false);
	bool ret = OnlineCycle();
	Post(COMPUTE);
	TPr::pr("Ждем результатов от Main");
	calc_event->WaitFor(INFINITE);
	calc_event->ResetEvent();
	if (Terminated)
	{
		Post(COMPLETE, 0);
		return;
	}
	TPr::pr("Получили результаты");
	if (!ret)
	{
		SetStext1("Режим \"Работа (Д)\" не завершен!");
		TPr::pr("Режим \"Работа (Д)\" не завершен!");
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
		FATAL("ThreadOnLine::Post: не могу послать сообщение");
	AnsiString a = "ThreadOnLine::Post: Однако послали ";
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
// -----подготовка к работе от самого начала до движения трубы----------------
UnicodeString ThreadOnLine::PrepareForWork()
{
	double t = 0;
	if(!CrossSolenoid->Solenoid1U(t))
	{
		char *a = "Перегрев поперечного соленоида 1";
		TPr::pr(a);
		char buf[128];
		 sprintf(buf, "%s %.1f", a, t);
		 updateStatusBarBottomParam = buf;
		 Synchronize(&UpdateStatusBarBottom);
		 return a;
	}
	if(!CrossSolenoid->Solenoid2U(t))
	{
		char *a = "Перегрев поперечного соленоида 2";
		TPr::pr(a);
		char buf[128];
		sprintf(buf, "%s %.1f", a, t);
		updateStatusBarBottomParam = buf;
		 Synchronize(&UpdateStatusBarBottom);
		 return a;
	}
   //	SLD->oLPCHPOW->Set(true);
	// ждем цех цикл
	SetStext2("Ждем сигнала \"Попер Цикл\"");
	Post(UPDATE_STATUS);

	if (!SLD->iCCYCLE->Wait(true, INFINITE))
		return "Не дождались сигнала Попер Цикл!";
	SLD->SetAlarm(true);
	SLD->SetCrossCycle(true);
	if (Linear)
	{
		SetStext2("Ждем сигнала \"Прод Цикл\"");
		Post(UPDATE_STATUS);

		if (!SLD->iLCYCLE->Wait(true, INFINITE))
			return "Не дождались сигнала Прод Цикл!";
		SLD->SetLinearCycle(true);
	}
	SetStext2("Ждем Готовность");
	Post(UPDATE_STATUS);

	SLD->oSHIFT->Set(true);
	TPr::pr("Установили oSHIFT");
	TPr::pr("Ждем сигнал  Готовность бесконечно");
	if (!SLD->iREADY->Wait(true, INFINITE))
		return ("Не дождались сигнала Готовность!");
	TPr::pr("Получили Готовность");
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
	 AnsiString a = "Соленоид поперечный: ";
	bool solinoidOn = CrossSolenoid->SolenoidOn();
	if(solinoidOn)
	{
		a += "включён";
	}
	else
	{
		a += "отключён";
	}
	TPr::pr(a);
	if(!solinoidOn)
	{
		char *a = "Нет поля поперечного соленоида";
		TPr::pr(a);
		 updateStatusBarBottomParam = a;
		 Synchronize(&UpdateStatusBarBottom);
		return a;
	}
   /*
	AnsiString a = "Соленоид поперечный: ";
	a += CrossSolenoid->GetUIR();
	TPr::pr(a);
	if (!CrossSolenoid->OkU())
	{
		SLD->oCSOLPOW->Set(false);
		return "Напряжение поперечного соленоида вне диапозона";
	}
	if (!CrossSolenoid->OkResist())
	{
		SLD->oCSOLPOW->Set(false);
		return "Сопротивление поперечного соленоида превысило норму";
	}
	*/
	/*
	if (Linear)
	{
		a = "Соленоид продольный: ";
		a += LinearSolenoid->GetUIR();
		TPr::pr(a);
		if (!LinearSolenoid->OkU())
		{
			SLD->oLSOLPOW->Set(false);
			return "Напряжение продольного соленоида вне диапозона";
		}
		if (!LinearSolenoid->OkResist())
		{
			SLD->oLSOLPOW->Set(false);
			return "Сопротивление продольного соленоида превысило норму";
		}
	}
	*/
	// крутим продольный
	if (Linear)
	{
		SLD->oLSCANPOW->Set(true);
		// Выставим скорость работы на входе в модуль
		TPr::pr(AnsiString("Скорость входящая InSpeed ") + AnsiString(InSpeed));
		if (!frConverter->setParameterSpeed(Globals::defaultRotParameter,
			InSpeed))
			return "Не смогли задать входную скорость вращения продольного модуля";

		if (!frConverter->startRotation())
			return "Не удалось включить вращение продольного модуля";
		if (!SLD->iLPCHRUN->Wait(false, 5000))
			return "Не достигнута скорость вращения продольного модуля!";
	  //	SLD->SetInvA(true);
	}
	lcard->LoadSettings();
	return "ok";
}

// -------------------------------------------------------------------------------
// ----онлайн цикл, крутящийся бесконечно и проверяющий все события---------------
bool ThreadOnLine::OnlineCycle()
{
	TPr::pr("Работа (Д): Режим работа");
	// Выставляем сигналы РАБОТА по модулям
	SLD->oCWORK->Set(true);
	if (Linear)
		SLD->oLWORK->Set(true);

	SetStext2("Ждем трубу в модулях");
	TPr::pr("Работа (Д): " + stext2);
	Post(UPDATE_STATUS);

	// флаги событий во время кругового цикла
	bool InSG = false; // труба зашла в размагничивание
	bool result = true; // результат всего цикла

	bool ppStarted = false;
	bool ppSignaled = false;
	bool ppIsStoped = false;

	bool prStarted = false;
	bool prSignaled = false;
	bool prIsStoped = false;
	bool ToFinish = false;

	bool TubeQuit = false;
	// Переменные для выставления сигнала на 50мс
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
	bool SLD_iCSTROBE_Get = false;  // заглушка на отсутствие лир
	bool SLD_iLSTROBE_Get = false;

	int crossZoneCounter = 0;

	while (Collect)
	{
		Sleep(delay);

		// заглушка на отсутствие лир
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
			// заглушка на отсутствие лир конец

		if (Cross && !ppIsStoped)
		{
			// -----------------------------------------------------------------------------------------------------------------
			// Начало поперечного сбора
			if (!ppStarted && SLD->iCCONTROL->Get())
			{
				AnsiString a;
				a = "iCCONTROL ";
				a += GetTickCount();
				TPr::pr(a);
				ppStarted = true;
				lcard->StartCross();
				SLD->oCMEAS->Set(true);
				SetStext2("Начали сбор поперечного");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);
				//Проверяем не СОП ли это
		   //		Singleton->isSOP = SLD->iSOP->WasConst(true,50);
		   //		if(Singleton->isSOP) pr("THIS IS SOP!!!"); //todo убрать после отладки

				delayCrossTimeControl = GetTickCount();
				speedTube = (double)unitBaseMMCross / (delayCrossTimeControl - SQ1TimeControl);
				dprint("cross Speed %f\n", speedTube);
			}
			// Обсчитываем поперечный модуль
			if (!ppSignaled && ppStarted && SLD_iCSTROBE_Get)//SLD->iCSTROBE->Get())  // заглушка на отсутствие лир
			{
				CrossStrobes++;
				TPr::pr(AnsiString("Поперечных стробов ") + CrossStrobes);
				ppSignaled = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("Авария: ") + lcard->LastError,
					&Collect, &result);
				else
				{
					 ++crossZoneCounter;
					BankCross->AddZone(lcard->GetPointCross());
					lcard->ClearCross();
					Post(REDRAW, REDRAW_CROSS);
				}
			}
			if (ppSignaled && !SLD_iCSTROBE_Get)//SLD->iCSTROBE->Get())  // заглушка на отсутствие лир
				ppSignaled = false;
			if (ppStarted && !SLD->iCCONTROL->Get())
			{
				ppIsStoped = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("Авария: ") + lcard->LastError,
					&Collect, &result);
				else
				{
					BankCross->AddZone(lcard->GetPointCross());
					lcard->ClearCross();
					Post(REDRAW, REDRAW_CROSS);
				}
				SLD->SetCrossCycle(false);
				lcard->StopCross();
				SetStext2("Остановили сбор с поперечного");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);
				if (!Linear)
				{
					SLD->oCSOLPOW->Set(false);
					pr("Сняли oCSOLPOW");
				}
			}
		} // -----------------------------------------------------------------------------------------------------------------
		if (Linear && !prIsStoped)
		{
			CycleTick = GetTickCount();
			// Начало продольного сбора
			if (!prStarted && SLD->iLCONTROL->Get())
			{
				prStarted = true;
				FirstLinerStrobeTick = CycleTick;
				SLD->oLMEAS->Set(true);
				lcard->StartLine();
				SetStext2("Начали сбор с продольного");
				TPr::pr(stext2);
				Post(UPDATE_STATUS);

				delayLongTimeControl = GetTickCount();
				speedTube = (double)unitBaseMMLong / (delayLongTimeControl - SQ1TimeControl);
				dprint("long Speed %f\n", speedTube);

			}
			if (prStarted && SLD_iLSTROBE_Get)//SLD->iLSTROBE->Get()) // заглушка на отсутствие лир
			{
				if (!prSignaled && prStarted)
				{
					LineStrobes++;
					TPr::pr(AnsiString("Продольных стробов ") + LineStrobes);
					prSignaled = true;
					if (!lcard->Read())
						ErrFinally(AnsiString("Авария: ") + lcard->LastError,
						&Collect, &result);
					else
					{
						BankLine->AddZone(lcard->GetPointLine());
						lcard->ClearLine();
						Post(REDRAW, REDRAW_LINE);
					}
				}
			}
			if (prSignaled && !SLD_iLSTROBE_Get)//SLD->iLSTROBE->Get())   // заглушка на отсутствие лир
				prSignaled = false;
			if (!isTwoSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseStop))
			{
				isTwoSpeed = true;
				TPr::pr(String("Скорость торможения OutSpeed ") +
					String(OutSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, OutSpeed))
					ErrFinally("Авария: Не удалось включить торможение",
					&Collect, &result);
			}

			if (!isFreeSpeed && prStarted && ((CycleTick - FirstLinerStrobeTick)
				> (DWORD)pauseWorkSpeed))
			{
				isFreeSpeed = true;
				TPr::pr(String("Скорость в работе WorkSpeed ") +
					String(WorkSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, WorkSpeed))
					ErrFinally("Авария: Не удалось включить рабочую скорость",
					&Collect, &result);
			}
			if (!isOutSpeed && prStarted && !SLD->iLCONTROL->Get())
			{
				longTimeControl = 0;
				delayLongTimeControl = 0;
				isOutSpeed = true;
				ControlOffTime = CycleTick;
				TPr::pr("Скорость разгона в конце струбы InSpeed " +
					String(InSpeed));
				if (!frConverter->setParameterSpeed
					(Globals::defaultRotParameter, InSpeed));
				//	ErrFinally("Авария: Не удалось включить конечную скорость",
				 //	&Collect, &result);
			}
			/*
			if (isOutSpeed && ((CycleTick - ControlOffTime) > 1000))
			{
				prIsStoped = true;
				if (!lcard->Read())
					ErrFinally(AnsiString("Авария: ") + lcard->LastError,
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
					ErrFinally("Авария: Не удалось выключить вращение",
					&Collect, &result);
				else
				{
					SetStext2("Остановили сбор с продольного");
					TPr::pr(stext2);
					Post(UPDATE_STATUS);
				}
			 //	SLD->oLSOLPOW->Set(false);
			//	pr("Сняли oLSOLPOW");
				SLD->oCSOLPOW->Set(false);
				pr("Сняли oCSOLPOW");
				SLD->oLSCANPOW->Set(false);
				pr("Сняли oLSCANPOW");
			}
			*/
		}
		// -----------------------------------------------------------------------------------------------------------------
		// смотрим, что труба вышла из установки
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
			SetStext2("Труба вышла из установки");
			Post(UPDATE_STATUS);
			SLD->oCSOLPOW->Set(false);
		  //	SLD->oLSOLPOW->Set(false);
			SLD->oLSCANPOW->Set(false);
			ToFinish = true;
			FinishTick = GetTickCount();
			// Finally();
			// Collect = false;
			TPr::pr(stext2);
			pr("Задержка по выходу");
		}
		if (ToFinish)
		{
			if (GetTickCount() - FinishTick > 1000)
			{
				Finally();
		   //		if (SLD->iLPCHRUN->Get())
			  //		SLD->oLPCHPOW->Set(false);
				Collect = false;
				pr("Задержка по выходу завершена");
			}
		}
		// -----------------------------------------------------------------------------------------------------------------
		// смотрим, не было ли сброса
		if (Terminated)
		{
			Collect = false;
			result = false;
			TPr::pr("Работа (Д): Зашли в Terminated");
			Finally();
		}
		// смотрим, не было ли аварии
		if (SLD->WasAlarm())
		{
			ErrFinally("Работа (Д): Выход по аварии", &Collect, &result);
		}
	}
	TPr::pr("OnlineCycle завершен");
	Post(SGDRAW);
	return result;
}

void ThreadOnLine::Finally()
{
	frConverter->stopRotation();
	TPr::pr("Finally() снятие сигналов");
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
