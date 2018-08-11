//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>


//---------------------------------------------------------------------------
USEFORM("Settings.cpp", SettingsForm);
USEFORM("SolidGroupOptions.cpp", SolidGroupOptionsForm);
USEFORM("NewDefectoscoper.cpp", NewDefectoscoperForm);
USEFORM("Password.cpp", PasswordForm);
USEFORM("uTFRCross.cpp", FRCross); /* TFrame: File Type */
USEFORM("uTFRDeads.cpp", FRDeads); /* TFrame: File Type */
USEFORM("uTFRBorders.cpp", FRBorders); /* TFrame: File Type */
USEFORM("uTFNewTypeSize.cpp", FNewTypeSize);
USEFORM("uTFProtocol.cpp", FProtocol);
USEFORM("uTFRLine.cpp", FRLine); /* TFrame: File Type */
USEFORM("uTFRThick.cpp", FRThick); /* TFrame: File Type */
USEFORM("View.cpp", ViewForm);
USEFORM("uTFRSum.cpp", FRSum); /* TFrame: File Type */
USEFORM("uTFRSensor.cpp", FRSensor); /* TFrame: File Type */
USEFORM("uTFRSOP.cpp", FRSOP); /* TFrame: File Type */
USEFORM("uTFilterSettings.cpp", FilterSettings); /* TFrame: File Type */
USEFORM("TSChangeWarning.cpp", TSChangeWarningForm);
USEFORM("uFViewZoneD.cpp", FViewZoneD);
USEFORM("uFViewZoneS.cpp", FViewZoneS);
USEFORM("uFRStatist.cpp", FRStatist); /* TFrame: File Type */
USEFORM("uFRViewZone.cpp", FRViewZone); /* TFrame: File Type */
USEFORM("Manage.cpp", ManageForm);
USEFORM("ColorSettings.cpp", FormColor);
USEFORM("ChangePassword.cpp", ChangePasswordForm);
USEFORM("Authorisation.cpp", AuthorisationForm);
USEFORM("ADCSettings.cpp", ADCSettForm);
USEFORM("A1730\uFSignalsState.cpp", FSignalsState);
USEFORM("ABOUT_NTC_NK_URAN.cpp", AboutBox1);
USEFORM("A1730\uFRSigPanel.cpp", FRSigPanel); /* TFrame: File Type */
USEFORM("Main.cpp", MainForm);
USEFORM("EtalonTubeDefectsNew.cpp", EtalonTubeDefectsNewForm);
USEFORM("Defectoscopers.cpp", DefectoscopersForm);
USEFORM("EtalonBorderSet.cpp", EtalonBorderSet);
USEFORM("EtalonCheckJournal.cpp", EtalonCheckJournalForm);
USEFORM("EtalonNewDefect.cpp", EtalonNewDefectForm);
USEFORM("EtalonTubeDefects.cpp", EtalonTubeDefectsForm);
USEFORM("EtalonDefectsCheck.cpp", EtalonDefectsCheckForm);
USEFORM("EtalonDefect.cpp", EtalonDefectForm);
USEFORM("EtalonDefectNew.cpp", EtalonDefectNewForm);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{

	try
	{
        // Пытаемся открыть мьютекс.
		HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, L"MyApp1.0");
        if(!hMutex)
          // Мьютекса не существует. То есть,
		  // это первый экземпляр,
          // создаем мьютекс.
		  hMutex = CreateMutex(0, 0, L"MyApp1.0");
		else
		{
		  // Мьютекс существует , то есть , запущен
		  // второй экземпляр, говорим юзеру что он олень и закрываем прогу.
		  Application->MessageBoxW(L"Приложение уже запущено!!!",L"Ошибка",MB_OK|MB_ICONERROR);
		  return 0;
		}

		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		Application->CreateForm(__classid(TMainForm), &MainForm);
		Application->CreateForm(__classid(TViewForm), &ViewForm);
		Application->CreateForm(__classid(TManageForm), &ManageForm);
		Application->CreateForm(__classid(TFormColor), &FormColor);
		Application->CreateForm(__classid(TADCSettForm), &ADCSettForm);
		Application->CreateForm(__classid(TAboutBox1), &AboutBox1);
		Application->CreateForm(__classid(TSolidGroupOptionsForm), &SolidGroupOptionsForm);
		Application->Run();

		// Приложение закрывается ,
		// освобождаем мьютекс.
		ReleaseMutex(hMutex);
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
