// ---------------------------------------------------------------------------
#include <vcl.h>
#include <dir.h>
#pragma hdrstop
#include "Main.h"
#include "ColorSettings.h"
#include "SMS.h"
#include "registry.hpp"
#include "Solenoid.h"
#include "uFSignalsState.h"

#include "uFunctions.h"
#include "global.h"
#include "Classes.hpp"
#include "Singleton.h"
#include "ADCSettings.h"
#include "View.h"
#include "Settings.h"
#include "Manage.h"
#include "ABOUT_NTC_NK_URAN.h"
#include "Inverter.h"
#include "SignalListDef.h"
#include "ThreadOnLine.h"
#include "uCDBS.h"
#include "uCExecute.h"
#include "Defectoscopers.h"
#include "EtalonDefectNew.h"
#include "EtalonCheckJournal.h"
#include "EtalonDefect.h"
#include "LCardData.h"
#include "Authorisation.h"
#include "Password.h"
#include "Save.h"
#include "TSChangeWarning.h"

#include "tools_debug/DebugMess.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "TeeGDIPlus"
// #pragma link "CommPort"
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "Series"
#pragma link "Chart"
#pragma link "uTFRBorders"
#pragma link "uTFRCross"
#pragma link "uTFRLine"
#pragma link "uTFRThick"
#pragma link "uTFRSum"
#pragma link "TeCanvas"
#pragma link "uFRStatist"
#pragma link "uTFRSOP"
#pragma resource "*.dfm"

using System::TThreadFunc;

TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner)
{
	ini = new TIniFile(Globals::IniFileName);

	Tag = 0; // запуск произведен 1й раз очистка буфера не требуется
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
	if (ParamCount() > 0)
		ReadFile(ParamStr(1));
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
dprint("APP RUN\n");
hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	DBS = new CDBS();
	LoadFormPos(this, ini);
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	if (!ini->ValueExists(sect, "CrossSensors"))
		ini->WriteString(sect, "CrossSensors", "12");
	if (!ini->ValueExists(sect, "LineSensors"))
		ini->WriteString(sect, "LineSensors", "4");

	LSensors = ini->ReadInteger(sect, "LineSensors", 4);
	CSensors = ini->ReadInteger(sect, "CrossSensors", 12);
	BankLine = NULL;
	BankCross = NULL;
	ComWithASU = ini->ReadBool("OtherSettings", "ComWithASU", 0);
	SLD = new SignalListDef(ini);
	FSignalsState = new TFSignalsState(this, ini, SLD);
	if (ini->ReadBool("OtherSettings", "SignalsVisible", false))
		FSignalsState->Show();
	DragAcceptFiles(Handle, true); // Разрешаем перетаскивание файлов

	bool RHKret = RegisterHotKey(Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00E, // УСЛОВНЫЙ идентификатор горячего ключа
		0, // модификатор
		VK_F1); // код клавиши

	bool RHKret1 = RegisterHotKey(Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00A, // УСЛОВНЫЙ идентификатор горячего ключа
		MOD_ALT + MOD_CONTROL, // модификатор
		VK_ADD); // код клавиши

	MainFormInit(Sender);
	KeyPreview = true;
#ifndef NOL502
	lcard = new LCardData(ini);
#endif
	CrossSolenoid = new Solenoid();//"PP");
   //	LinearSolenoid = new Solenoid("PR");

	TPr::pr("ага");
	Singleton = new CSingleton(ini, this);
	Singleton->CrossResult->Bind(FRCross1);
	Singleton->LinearResult->Bind(FRLine1);
	Singleton->ThResult->Bind(FRThick1);
	Singleton->SumResult->Bind(FRSum1);
	UpdateComboBox();

	FRCross1->Init(ini);
	FRLine1->Init(ini);
	FRThick1->Init(ini);
	FRSum1->Init(ini);
	FRStatist1->Init(ini);

	FRCross1->OnChange = Post;
	FRLine1->OnChange = Post;
	FRThick1->OnChange = Post;
	FRSum1->OnChange = Post;

	FRSOP1->Init(ini);

	LoadSettings();

	// frHistory->Clear();

	frConverter = new Inverter();

	CrossFilter = new Filters(ini,"Pp");
	LinearFilterIn = new Filters(ini,"PrIn");
	LinearFilterOut = new Filters(ini,"PrOut");

	CrossFilter->LoadSettings();//setSettingsFromIniFile();
	LinearFilterIn->LoadSettings();//setSettingsFromIniFile();
	LinearFilterOut->LoadSettings();//setSettingsFromIniFile();

	// смотрим, какие модули работают по умолчанию (в последний раз)
	cbInterruptView->Checked =
		ini->ReadBool("Default", "IsInterruptView", false);

	SystemConst::SetSystemConstants();
	SetProjectSpecialty();

	TThread::CurrentThread->NameThreadForDebugging("Main ");

	set_msg = RegisterWindowMessage(L"Settings");
	if (set_msg == 0)
		FATAL("TMainForm::FormCreate: не могу зарегистрировать сообщение");
	thread_msg = RegisterWindowMessage(L"thread");
	if (thread_msg == 0)
		FATAL("TMainForm::FormCreate: не могу зарегистрировать сообщение");
	sms_msg = RegisterWindowMessage(L"sms");
	if (sms_msg == 0)
		FATAL("TMainForm::FormCreate: не могу зарегистрировать сообщение");
	spectro_msg = RegisterWindowMessage(L"spectro");
	if (spectro_msg == 0)
		FATAL("TMainForm::FormCreate: не могу зарегистрировать сообщение");
	// инициализируем передачу данных между модулями по TCP/IP
	sms = new SMS(this, Handle, sms_msg);
	sms->StartServer();

 //	SLD->oLPCHPOW->Set(true);
	Sleep(1000);
	if (!frConverter->stateRead())
	{
		StatusBarBottom->Panels->Items[0]->Text =
			"Не удалось прочитать состояние ПЧ";
		StatusBarBottom->Refresh();
	}
	else
	{
		StatusBarBottom->Panels->Items[0]->Text = "ПЧ подключен";
		StatusBarBottom->Refresh();
	}
	bCancelWork->Enabled = false;
	isInteruptSG = false;
	ClearCharts();

	MuchWinWorkTime = ini->ReadInteger("Default","MuchWinWorkTime",10*60*60*1000);
	WinWorkTimer->Interval = ini->ReadInteger("Default","MuchWinWorkTimeInterval",15*60*1000);
}

// ---------------------------------------------------------------------------
void TMainForm::Post(void)
{
	if (PostMessage(Handle, set_msg, 0, 0) == 0)
		throw(Exception("TMainForm::Post: не могу послать сообщение"));
	TPr::pr("Однако послали");
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	frConverter->stopRotation();
	DragAcceptFiles(Handle, false); // Запрещаем перетаскивание файлов

   //	SLD->oLSOLPOW->Set(false);
	SLD->oCSOLPOW->Set(false);
  //	SLD->oLPCHPOW->Set(false);

	// запись дефолтного всего
	ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
	ini->WriteBool("Default", "IsInterruptView", cbInterruptView->Checked);
	delete CrossSolenoid;
  //	delete LinearSolenoid;

	UnregisterHotKey(Handle, // Handle окна
		0x00E); // наш идентификатор горячего ключа
	UnregisterHotKey(Handle, // Handle окна
		0x00A); // наш идентификатор горячего ключа

	delete Singleton;
	ini->WriteBool("OtherSettings", "SignalsVisible", FSignalsState->Visible);
	delete FSignalsState;
	delete SLD;
	SaveFormPos(this, ini);
	TPr::Dispose();
	if(BankCross!=NULL)
		delete BankCross;
	if(BankLine!=NULL)
		delete BankLine;
	delete ini;
	delete DBS;
	CloseHandle(hEvent);
}

// ---------------------------------------------------------------------------
void TMainForm::SetProjectSpecialty()
{
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::MainFormInit(TObject *Sender)
{
	int space = 3;
	int bigspace = 30;
	// ActionMainMenuBar1->Height=100;
	// StatusBarTop->Top=ActionMainMenuBar1->Top+ActionMainMenuBar1->Height;
	FRCross1->Left = space;
	FRCross1->Width = ClientWidth - FRCross1->Left - space;
	FRLine1->Left = FRCross1->Left;
	FRLine1->Width = FRCross1->Width;
	FRThick1->Left = FRCross1->Left;
	FRThick1->Width = FRCross1->Width;
	FRSum1->Left = FRCross1->Left;
	FRSum1->Width = FRCross1->Width;

	FRThick1->Top = cbInterruptView->Top + cbInterruptView->Height;
	int hhh = (StatusBarBottom->Top - FRThick1->Top) / 5;
	FRThick1->Height = hhh * 2;

	FRCross1->Top = FRThick1->Top + FRThick1->Height;
	FRCross1->Height = hhh;

	FRLine1->Top = FRCross1->Top + FRCross1->Height;
	FRLine1->Height = hhh;

	FRSum1->Top = FRLine1->Top + FRLine1->Height;
	FRSum1->Height = hhh;

	StatusBarBottom->Panels->Items[0]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[1]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[2]->Width = ClientWidth / 4;
	StatusBarBottom->Panels->Items[3]->Width = ClientWidth / 4;

	StatusBarTop->Panels->Items[0]->Width = ClientWidth / 4;
	StatusBarTop->Panels->Items[1]->Width =
		(ClientWidth - StatusBarTop->Panels->Items[0]->Width) / 2;
	StatusBarTop->Panels->Items[2]->Width =
		StatusBarTop->Panels->Items[1]->Width;
	FRStatist1->Left = ClientWidth - FRStatist1->Width - space;
}

// ---------------------------------------------------------------------------
void TMainForm::ClearCharts(void)
{
	TPr::pr("ClearCharts()");
	Singleton->FromFile = false;
	Singleton->CrossResult->DeleteData();
	Singleton->LinearResult->DeleteData();
	Singleton->ThResult->DeleteData();
	Singleton->SumResult->DeleteData();

	FRCross1->Clear();
	FRLine1->Clear();
	FRThick1->Clear();
	FRSum1->Clear();

	Refresh();
	MainFormInit(this);
	isView = false;
}

void TMainForm::LoadSettings(void)
{
	pr("LoadSettings");
	String sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	// прочитаем используемый диаметр (типоразмер) и прогрузим эталоны по группе прочности
	LSensors = ini->ReadInteger(sect, "LineSensors", 4);
	CSensors = ini->ReadInteger(sect, "CrossSensors", 12);
	if(BankCross!=NULL)
		delete BankCross;
	if(BankLine!=NULL)
		delete BankLine;
	BankLine = new CBank(Globals::max_zones, LSensors);
	BankCross = new CBank(Globals::max_zones, CSensors);

	Globals::current_diameter = ini->ReadInteger(sect, "Diameter", 1);

	Singleton->SumResult->min_good_length =
		(short)ini->ReadInteger(sect, "Min_Good_Length", 0);
	Singleton->SumResult->end_cut = (short)ini->ReadInteger(sect, "EndCut", 0);
	FRCross1->LoadSettings();
	FRLine1->LoadSettings();
	FRThick1->LoadSettings();
	FRSum1->LoadSettings();
	Singleton->LoadSettings();
	ReCalcSG();
}
// ---------------------------------------------------------------------------

void TMainForm::UpdateComboBox()
{
	cbTypeSize->Items->Clear();
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf
		(ini->ReadString("Default", "TypeSize", "1"));
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW
			(L"Ни одного типоразмера не найдено! Создайте их, нажав клавишу F4",
		L"Предупреждение!", MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::cbTypeSizeSelect(TObject *Sender)
{
	// Предупреждение
	AnsiString Last = ini->ReadString("Default", "TypeSize", "");
	if (Last != cbTypeSize->Text)
	{
		TTSChangeWarningForm * TSWar = new TTSChangeWarningForm(this);
		if(TSWar->ShowModal()==mrOk)
		{
			ini->WriteString("Default", "TypeSize", cbTypeSize->Text);
			LoadSettings();
			TPr::pr("cbTypeSizeSelect");
		}
		else
			cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(Last);
		delete TSWar;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::eSettingsChange(TObject *Sender)
{
	ini->WriteString("Type_" + cbTypeSize->Text,
		((TEdit *) Sender)->Name.SubString(2, 29), ((TEdit *) Sender)->Text);
	LoadSettings();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bTestClick(TObject *Sender)
{
	SLD->oLSCANPOW->Set(true);
	SLD->oCSOLPOW->Set(true);
	Sleep(1000);
   //	SLD->oLSOLPOW->Set(true);

	ClearCharts();
	isView = false;

	StatusBarTop->Panels->Items[1]->Text = L"Идет сбор данных с АЦП";
	StatusBarTop->Refresh();
	SetAbleButtons(false); // Блокируем главную форму от кривых рук

	lcard->LoadSettings();
	lcard->StartCross();
	lcard->StartLine();
	for (DWORD tt = GetTickCount(); GetTickCount() - tt < 5000; Sleep(10))
	{
		// Application->ProcessMessages();
	}
	bool ret = lcard->Read();
	SLD->oCSOLPOW->Set(false);
   //	SLD->oLSOLPOW->Set(false);
	SLD->oLSCANPOW->Set(false);
	lcard->StopCross();
	lcard->StopLine();
	SetAbleButtons(true); // Разблокировка формы
	if (!ret)
	{
		AnsiString a = "Ошибка сбора данных: ";
		a += lcard->LastError;
		StatusBarTop->Panels->Items[1]->Text = a;
		StatusBarTop->Refresh();
	}
	else
	{
		StatusBarTop->Panels->Items[1]->Text = L"Сбор данных с АЦП завершен";
		StatusBarTop->Refresh();
		// ------------------Тестовое заполнение результата и вывод на экран------------
		Singleton->CrossResult->zones = 30;
		Singleton->CrossResult->FillDataR(lcard->GetPointCross());
		Singleton->LinearResult->zones = 30;
		Singleton->LinearResult->FillDataR(lcard->GetPointLine());
		isView = true;
		ReDraw();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bViewClick(TObject *Sender)
{
	if (!isView)
		return;
	ViewForm->ShowModal();
	if (ViewForm->need_repaint)
	{
		LoadSettings();
		ReDraw();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::bWorkClick(TObject *Sender)
{
	if (workonline == NULL)
	{
		//Вот здесь нужно вызвать окно запроса оператора.
   //		TAuthorisationForm* AuthorisationForm = new TAuthorisationForm(this,ini);
	 //	AuthorisationForm->ShowModal();
	 //	delete AuthorisationForm;
	 //	if (ini->ReadBool("Default", "IsAuthorisationOk", false))
	  //	{
	   //		AnsiString Defectoscoper = ini->ReadString("Default","LastAuthorisation","");
		//	if(Defectoscoper != "")
		 //	{
		  //		AnsiString NewCaption = "БУРАН-5000 №1627. Магнитоиндукционная дефектоскопия НКТ. ";
		   //		NewCaption	+= "Оператор " + Defectoscoper;
			//	MainForm->Caption = NewCaption;
		   //	}
			Start();
	   //	}
	}
	else
		ReStart();

}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bCancelWorkClick(TObject *Sender)
{
	Stop();
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TMainForm::bManageClick(TObject *Sender)
{
	ManageForm->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 112)
		WinExec("hh Help.chm", SW_RESTORE);
	if (Key == '`' || Key == L'ё')
	{
		cbTypeSize->ItemIndex =
			(cbTypeSize->ItemIndex < cbTypeSize->Items->Count - 1) ?
			(cbTypeSize->ItemIndex + 1) : (0);
		cbTypeSizeSelect(Sender);
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuTypeSizeClick(TObject *Sender)
{
	// показать окно с настройками типоразмера
	SettingsForm = new TSettingsForm(this, ini);
	SettingsForm->ShowModal();
	bool need_loadsettings = SettingsForm->need_loadsettings;
	delete SettingsForm;
	if (need_loadsettings)
	{
		LoadSettings();
		ReDraw();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuMeasuresClick(TObject *Sender)
{
	// показать окно с настройками измерений
	ADCSettForm->ShowModal();
	if (ADCSettForm->need_redraw)
	{
		LoadSettings();
		ReDraw();
	}
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSignalsStateClick(TObject *Sender)
{
	FSignalsState->Show();
}

// -----------------------------------------------------------------------------
void __fastcall TMainForm::menuSaveTubeClick(TObject *Sender)
{
//	TPasswordForm* PasswordForm = new TPasswordForm(this);
//	PasswordForm->SetIni(ini);
//	PasswordForm->SetUserName("Master");
//	PasswordForm->ShowModal();
//	delete PasswordForm;
//	if (ini->ReadBool("Default", "IsPasswordOk", false))
//	{
	TAuthorisationForm* AuthorisationForm = new TAuthorisationForm(this,ini);
	AuthorisationForm->ShowModal();
	delete AuthorisationForm;
	if (ini->ReadBool("Default", "IsAuthorisationOk", false))
	{
		if (SaveToFileDialog->Execute())
		{
			// Создаем пустой файл, чтобы записать в него собранные данные
			try
			{
				FILE *file;
				file = fopen(AnsiString(SaveToFileDialog->FileName).c_str(), "wb");
				// fprintf(file,"%s \t %s\n","Zones","Sensors");
//				fclose(file);
//
//				Singleton->CrossResult->SaveTubeToFile(SaveToFileDialog->FileName);
//				// Поперечные данные
//
//				Singleton->LinearResult->SaveTubeToFile(SaveToFileDialog->FileName);
//				// Продольные данные
//
//				Singleton->ThResult->SaveTubeToFile(SaveToFileDialog->FileName);
//				// Толщинометрия (только zone_data)
//
//				ADCSettForm->SaveTubeSettingsToFile(SaveToFileDialog->FileName);
//				//Сохраняет пороги и усиления для Альметьевска

				Singleton->CrossResult->SaveTubeToFile(file);
				// Поперечные данные

				Singleton->LinearResult->SaveTubeToFile(file);
				// Продольные данные

				Singleton->ThResult->SaveTubeToFile(file);
				// Толщинометрия (только zone_data)

				ADCSettForm->SaveTubeSettingsToFile(file);
				//Сохраняет пороги и усиления для Альметьевска
				fclose(file);
			}
			catch (...)
			{
				Application->MessageBoxW(L"Не удалось сохранить файл", L"Ошибка",
					MB_ICONERROR | MB_OK);
			}
		}
	}
}

// ------------------------------------------------------------------------------
void __fastcall TMainForm::menuLoadTubeClick(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		if (OpenDialogFromFile->Execute())
		{
			ClearCharts();
			ReadFromFile((OpenDialogFromFile->FileName).c_str());
		}
	}
}

// ---------------------------------------------------------------------------
void TMainForm::ReadFromFile(UnicodeString path)
{
	FILE *file;
	file = fopen(AnsiString(path).c_str(), "r+");

	if (file == NULL)
		Application->MessageBoxW(L"Не удалось открыть файл для чтения",
		L"Ошибка", MB_ICONERROR | MB_OK);
	Singleton->CrossResult->LoadTubeFromFile(file);
	Singleton->LinearResult->LoadTubeFromFile(file);
	Singleton->ThResult->LoadTubeFromFile(file);
	Singleton->FromFile = true;
	fclose(file); // закрыли
	isView = true;
	ReDraw();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuTestAdvantechClick(TObject *Sender)
{
	// закрывает прогу и вызывает прогу с управлением Выходами платы Advantech
	ShellExecute(0, L"open", L"..\\..\\Settings\\IO_Management.exe", 0, 0,
		SW_SHOWNORMAL);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuF1Click(TObject *Sender)
{
	WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuColorsClick(TObject *Sender)
{
	FormColor->Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::menuProtocolClick(TObject *Sender)
{
	TPr::Show();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY)
	{ // сообщение наше
		if (Msg.wParam == 0x00E) // идентификатор наш
		{
			WinExec("hh ..\\..\\help\\Help.chm", SW_RESTORE);
			Handled = true;
		}
		else if (Msg.wParam == 0x00A) // идентификатор наш
		{
			Handled = true;
		}
	}
	else if (Msg.message == set_msg)
	{
		Handled = true;
		TPr::pr("Однако LoadSettings по сообщению");
		LoadSettings();
		ReDraw();
	}
	else if (Msg.message == thread_msg)
	{
		Handled = true;
		if (workonline == NULL)
			return;
		switch (Msg.wParam)
		{
		case ThreadOnLine::REDRAW:
			TPr::pr("Сообщение: перерисовать");
			if (Msg.lParam == ThreadOnLine::REDRAW_CROSS)
			{
				if (Singleton->CrossResult->AddBankZone(BankCross, false))
				{
					SLD->oCRESULT->Set(true);
					TPr::pr("oCRESULT");
				}
				SLD->oCSTROBE->Set(true);
				Singleton->ComputeZonesData();
				Singleton->CrossResult->PutResultOnChart();
				Singleton->SumResult->PutResultOnChart();
				AnsiString a = "Перерисовали cross, зон ";
				a += Singleton->CrossResult->zones;
				a += " ";
				a += GetTickCount();
				TPr::pr(a);
			}
			else if (Msg.lParam == ThreadOnLine::REDRAW_LINE)
			{
				if (Singleton->LinearResult->AddBankZone(BankLine, true))
				{
					SLD->oLRESULT->Set(true);
					TPr::pr("oLRESULT");
				}
				SLD->oLSTROBE->Set(true);
				Singleton->ComputeZonesData();
				Singleton->LinearResult->PutResultOnChart();
				Singleton->SumResult->PutResultOnChart();
				AnsiString a = "Перерисовали line, зон ";
				a += Singleton->LinearResult->zones;
				a += " ";
				a += GetTickCount();
				TPr::pr(a);
			}
			else
			{
				TPr::pr("Неизвестный параметр");
			}
			break;
		case ThreadOnLine::COMPUTE:
			{
				AnsiString a = "Сообщение: расчет треда, код: ";
				a += Msg.lParam;
				TPr::pr(a);
			} isView = true;
			ReDraw();
			StatusBarTop->Refresh();
			StatusBarTop->Panels->Items[2]->Text = " ";
			{
				if (cbInterruptView->Checked
				 || cbBrak->Checked && Singleton->SumResult->decision == "Брак")
					InteruptView();
				else
					workonline->SetCalc(Singleton->SumResult->decision == "Брак");
			} break;
		case ThreadOnLine::COMPLETE:
			{
				AnsiString a = "Сообщение: завершение треда, код: ";
				a += Msg.lParam;
				TPr::pr(a);
			} workonline->Terminate();
			SLD->LatchesTerminate();
			SLD->SetAlarm(false);
			workonline->WaitFor();
			delete workonline;
			workonline = NULL;
			SetAbleButtons(true);
			if (Msg.lParam == 1)
			{
				//--- Сохранение в БД и в файл
				if(Singleton->isSOP == false)
				{
					SaveTubeToDB(ini);
					if (ini->ReadString("OtherSettings", "TubePath","") != "")
						SaveTubeToFile(ini);
				//---
				FRStatist1->Add(Singleton->SumResult->decision == "Брак");
				}
				Start();
			}
			else
			{
				TPr::pr("_return_code==false");
				bWork->Caption = "Работа";
				bCancelWork->Enabled = false;
			}
			break;
		case ThreadOnLine::UPDATE_STATUS:
			UpdateStatus(workonline->GetStext1(), workonline->GetStext2());
			break;
		case ThreadOnLine::NEXT_TUBE:
			{
				ClearCharts();
				FRSum1->pNN->Caption = "";
			} break;
		case ThreadOnLine::SGDRAW:
			pr("Main: получили SGDRAW");
			// workonline->SetCalc();
		}
	}
	else if (Msg.message == sms_msg)
	{
	if(Singleton->CrossResult->zones > Singleton->ThResult->zones)
	{
		TPr::pr("Перерисовываем Thick");
		AnsiString a;
		a = "MSG Thick: [";
		a += Singleton->ThResult->zones;
		a += "]=";
		a += sms->cur_thick;
		TPr::pr(a);
		// Singleton->ThResult->zone_data[Singleton->ThResult->zones] =
		// ((double)Msg.lParam) / 10000;

		Singleton->ThResult->zone_data[Singleton->ThResult->zones] =
			sms->cur_thick;
		Singleton->ThResult->zones++;
		Singleton->ThResult->PutResultOnChart();
		Singleton->ComputeZonesData();
		Singleton->SumResult->PutResultOnChart();
	}
	}
}

// ----------------------------------------------------------------------------
void __fastcall TMainForm::bManualSGClick(TObject *Sender)
{
	SLD->oCWORK->Set(true);
	SLD->oSHIFT->Set(true);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::WmDropFiles(TWMDropFiles& Message)
{
	HDROP drop_handle = (HDROP)Message.Drop;
	wchar_t fName[MAXPATH];
	int filenum = DragQueryFile(drop_handle, -1, NULL, NULL);
	for (int i = 0; i < filenum; i++)
	{
		DragQueryFile(drop_handle, i, fName, MAXPATH);
		ReadFile(fName);
	}
	DragFinish(drop_handle);
}

// ---------------------------------------------------------------------------------

// После перетаскивания, вычисляем расширения и открываем файл -----
void __fastcall TMainForm::ReadFile(AnsiString FileName)
{
	AnsiString str = ExtractFileExt(FileName);
	if (str == ".sg")
	{
	}
	else if (str == ".dkb")
		ReadFromFile(FileName);
}
// ---------------------------------------------------------------------------------

void __fastcall TMainForm::N1Click(TObject *Sender)
{
	// about
	AboutBox1->Show();
}

// ---------------------------------------------------------------------------

void TMainForm::UpdateStatus(AnsiString _text1, AnsiString _text2)
{
	StatusBarTop->Panels->Items[1]->Text = _text1;
	StatusBarTop->Panels->Items[2]->Text = _text2;
	StatusBarTop->Refresh();
}

// --------------------------------------------------------------------------------

void TMainForm::ReDraw(void)
{
	if (!isView)
		return;

	Singleton->CrossResult->ComputeZoneData();
	Singleton->CrossResult->PutResultOnChart();

	Singleton->LinearResult->ComputeZoneData();
	Singleton->LinearResult->PutResultOnChart();

	Singleton->ThResult->PutResultOnChart();

	Singleton->ComputeZonesData();

	Singleton->SumResult->PutResultOnChart();

	Singleton->SumResult->MakeDecision();

	FRThick1->SetMinThick(Singleton->ThResult->MinThick
		(Singleton->SumResult->cut1, Singleton->SumResult->cut2));

	TColor lcolor;
	if (Singleton->SumResult->decision == "Брак")
		lcolor = clRed;
	else
		lcolor = clGreen;
	FRSum1->SetDecision(Singleton->SumResult->decision, lcolor,
		Singleton->SumResult->cut1, Singleton->SumResult->cut2);
}

// ---------------------------------------------------------------------------
void TMainForm::pr(AnsiString _msg)
{
	TPr::pr(_msg);
}

// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void TMainForm::ReCalcSG(void)
{
}

// ---------------------------------------------------------------------------
void TMainForm::Start(void)
{
	if (workonline != NULL)
	{
		pr("TMainForm::Start: работа уже запущена");
		return;
	}
	SetAbleButtons(false);
	bCancelWork->Enabled = true;
	TPr::Clear();
	bWork->Caption = "";
	if (cbInterruptView->Checked)
		bWork->Caption = "Продолжить";
	else
		bWork->Caption = "Работа";
	BankCross->Clear();
	BankLine->Clear();
	workonline = new ThreadOnLine(false, FRLine1->IsLinear(), BankCross,
		BankLine, Handle, thread_msg, ini, &SGbuffer);
	SLD->SetAlarm(true);
}

// ---------------------------------------------------------------------------
void TMainForm::InteruptView(void)
{
	if (workonline == NULL)
	{
		pr("TMainForm::Interupt: работа не запущена");
		return;
	}

	StatusBarTop->Panels->Items[2]->Text = L"Прерывание просмотр";
	StatusBarTop->Refresh();
	bWork->Enabled = true;
	bWork->Caption = "Продолжить";
	bView->Enabled = true;

	ActionManager1->FindItemByCaption("Настройка")->Visible = true;
	ActionManager1->FindItemByCaption("Труба")->Visible = true;
	ActionManager1->FindItemByCaption("Диагностика")->Visible = true;
	ActionManager1->FindItemByCaption("Помощь")->Visible = true;
   //	ActionManager1->FindItemByCaption("Контроль СОП")->Visible = true;
}

void TMainForm::InteruptSG(void)
{
	if (workonline == NULL)
	{
		pr("TMainForm::Interupt: работа не запущена");
		return;
	}
	isInteruptSG = true;
	StatusBarTop->Panels->Items[2]->Text = L"Прерывание СОП ГП";
	StatusBarTop->Refresh();
	bWork->Enabled = true;
	bWork->Caption = "Продолжить";
	bView->Enabled = true;
	ActionManager1->FindItemByCaption("Группа прочности")->Visible = true;
}

// ---------------------------------------------------------------------------
void TMainForm::ReStart(void)
{
	if (workonline == NULL)
	{
		pr("TMainForm::ReStart: работа не запущена");
		return;
	}
	SetAbleButtons(false);
	bCancelWork->Enabled = true;
	bWork->Caption = "Продолжить";
	isInteruptSG = false;
	workonline->SetCalc(Singleton->SumResult->decision == "Брак");
}

// ---------------------------------------------------------------------------
void TMainForm::Stop(void)
{
	if (workonline == NULL)
	{
		pr("TMainForm::Stop: работа не запущена");
		return;
	}
	workonline->Terminate();
	SLD->LatchesTerminate();
	workonline->SetCalc(true);
	TPr::pr("User: Сбросили Онлайн работу");
}

// ---------------------------------------------------------------------------
// блокировка/разблокировка клавиш
void TMainForm::SetAbleButtons(bool state)
{
	FRLine1->SetEnabledR(state);
	cbTypeSize->Enabled = state;
	bTest->Enabled = state;
	bView->Enabled = state;
	bWork->Enabled = state;
	bManage->Enabled = state;
	bCancelWork->Enabled = state;
	ExitTube->Enabled = state;

	FRCross1->SetEnabledR(state);
	FRThick1->SetEnabledR(state);
	// eMin_Good_Length->Enabled=state;

	ActionManager1->FindItemByCaption("Настройка")->Visible = state;
	ActionManager1->FindItemByCaption("Труба")->Visible = state;
	ActionManager1->FindItemByCaption("Диагностика")->Visible = state;
//	ActionManager1->FindItemByCaption("Группа прочности")->Visible = state;
	ActionManager1->FindItemByCaption("Помощь")->Visible = state;
	//ActionManager1->FindItemByCaption("Выгон трубы")->Visible = state;
 //	ActionManager1->FindItemByCaption("Контроль СОП")->Visible = state;
  //  SetEvent(hEvent);
	// menuTest->Enabled = state;
	//
	// menuTestAdvantech->Enabled = state;
	// menuSGTest->Enabled = state;
	// menuSolidGroup->Enabled=state;
	// menuViewEtalons->Enabled=state;
	// menuSGTest->Enabled=state;
	// menuGraphicsSG->Enabled=state;
}

// ---------------------------------------------------------------------------

void __fastcall TMainForm::Action22Execute(TObject *Sender)
{
	TDefectoscopersForm* defectoscopersForm = new TDefectoscopersForm(this);
	defectoscopersForm->SetIni(ini);
	defectoscopersForm->ShowModal();
	delete defectoscopersForm;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Action23Execute(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		TEtalonDefectNewForm* EtalonDefectNewForm = new TEtalonDefectNewForm(this);
		EtalonDefectNewForm->SetIni(ini);
		EtalonDefectNewForm->ShowModal();
		delete EtalonDefectNewForm;
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::Action24Execute(TObject *Sender)
{
	TEtalonCheckJournalForm* EtalonCheckJournalForm =
		new TEtalonCheckJournalForm(this);
	EtalonCheckJournalForm->SetIni(ini);
	EtalonCheckJournalForm->OnLoadTube = ReadFromFile;
	EtalonCheckJournalForm->ShowModal();
	delete EtalonCheckJournalForm;
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::Action25Execute(TObject *Sender)
{
//	if(Singleton->isSOP)
//	{
		TEtalonDefectForm* EtalonDefectForm = new TEtalonDefectForm(this);
		EtalonDefectForm->SetIni(ini);
		EtalonDefectForm->ShowModal();
		delete EtalonDefectForm;
//	}
//	else
//		Application->MessageBoxW(L"Не включен Режим СОП! Сверка запрещена!",
//			L"Ошибка", MB_ICONERROR | MB_OK);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FRSOP1cbEtalonViewClick(TObject *Sender)
{
	FRSOP1->cbEtalonViewClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WinWorkTimerTimer(TObject *Sender)
{
	if(GetTickCount() > MuchWinWorkTime)
	{
		WinWorkPanel->Left = FRThick1->Left+50;
		WinWorkPanel->Top = FRThick1->Top+50;
		WinWorkPanel->Width = FRThick1->Width-100;
		WinWorkPanel->Height = FRSum1->Top - WinWorkPanel->Top;
		lWinWorkMessage->Left = (WinWorkPanel->Width - lWinWorkMessage->Width)/2;
		lWinWorkMessage->Top = WinWorkPanel->Height/2 - lWinWorkMessage->Height;
		bWinWorkOk->Left = (WinWorkPanel->Width - bWinWorkOk->Width)/2;
		bWinWorkOk->Top = WinWorkPanel->Height/2 + 50;
		//WinWorkPanel->BringToFront();//todo
		//WinWorkPanel->Visible = true;//todo
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::bWinWorkOkClick(TObject *Sender)
{
	WinWorkPanel->SendToBack();
	WinWorkPanel->Visible=false;
}
//---------------------------------------------------------------------------
DWORD WINAPI TestInputBitCycle3(PVOID p)
{
	while(true)
	{
		DWORD res = WaitForSingleObject(((TMainForm *)p)->hEvent, 500);
		switch(res)
		{
			case WAIT_TIMEOUT:
			{
			   if(!SLD->iLCONTROL->Get())
			   {
				SLD->oLWORK->Set(false);
				((TMainForm *)p)->ExitTube->Caption = "Выгон трубы";
				frConverter->stopRotation();
				((TMainForm *)p)->ExitTube->Tag = 0;
				((TMainForm *)p)->SetAbleButtons(true);
				return 0;
               }
			}
			break;
			case WAIT_OBJECT_0:
				SLD->oLWORK->Set(false);
				((TMainForm *)p)->ExitTube->Caption = "Выгон трубы";
				frConverter->stopRotation();
				((TMainForm *)p)->ExitTube->Tag = 0;
				((TMainForm *)p)->SetAbleButtons(true);
				return 0;

		}
	}
}

void __fastcall TMainForm::ExitTubeClick(TObject *Sender)
{
if(0 == ExitTube->Tag)
{
		bool bLongControl = SLD->iLCONTROL->Get();
		if(!bLongControl)return;
		if(!SLD->iCC->Get())
		{
		StatusBarBottom->Panels->Items[2]->Text = "Включите цепи управления";
		StatusBarBottom->Refresh();
			  return;
		}
		else
		{
		StatusBarBottom->Panels->Items[2]->Text = "";
		StatusBarBottom->Refresh();
		}

			AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
		int speed = ini->ReadInteger(sect, "WorkSpeed", 4);
		if (frConverter->setParameterSpeed(Globals::defaultRotParameter, speed))
		{
			ExitTube->Caption = "СТОП вращения";
			ExitTube->Tag = 1;
			frConverter->startRotation();
			Sleep(1000);
			SLD->oLWORK->Set(true);
			Sleep(1000);
			SetAbleButtons(false);
			ExitTube->Enabled = true;
			CloseHandle(CreateThread(NULL, 0, TestInputBitCycle3, this, 0, NULL));
		}
}else
{
	SetEvent(hEvent);
	dprint("Event\n");
}
}





