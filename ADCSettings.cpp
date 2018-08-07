// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Filters.h"
#include "ADCSettings.h"
#include "LCardData.h"
#include "Singleton.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "uTFilterSettings"
#pragma link "uTFRSensor"
#pragma resource "*.dfm"
TADCSettForm *ADCSettForm;

// ---------------------------------------------------------------------------
__fastcall TADCSettForm::TADCSettForm(TComponent* Owner) : TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	if (!ini->ValueExists(sect, "CrossSensors"))
		ini->WriteString(sect, "CrossSensors", "12");
	if (!ini->ValueExists(sect, "LineSensors"))
		ini->WriteString(sect, "LineSensors", "4");

	LSensors = ini->ReadInteger(sect, "LineSensors", 4);
	CSensors = 12;//ini->ReadInteger(sect, "CrossSensors", 12);//todo

	int vmar = 40, hmar = 55;
	int height = 15, width = 60;

	int hspace = 3;
	int vspace = 5;

	int ltop;
	ltop = Label15->Top + Label15->Height + vspace;
	for (int i = 0; i < CSensors; i++)
	{
		TFRSensor* p = new TFRSensor(gbPP, ini, "Cross", i);
		AnsiString a = "CS_";
		a += i;
		p->Name = a;
		p->Parent = gbPP;
		p->Left = hspace;
		p->Top = ltop + (p->Height + vspace) * i;
		p->CBRange->PopupMenu = CopyValueCrossChannel;
		p->CBRange->OnContextPopup = TContextPopupEvent(&ContextPopup);
		CS.Add(p);
	}
	ltop = Label16->Top + Label16->Height + vspace;
	for (int i = 0; i < LSensors; i++)
	{
		TFRSensor* p = new TFRSensor(gbPR, ini, "Line", i);
		AnsiString a = "LS_";
		a += i;
		p->Name = a;
		p->Parent = gbPR;
		p->Left = hspace;
		p->Top = 100;
		p->Top = ltop + (p->Height + vspace) * i;
		LS.Add(p);
	}

	FillAllControls();
}

// ---------------------------------------------------------------------------
void TADCSettForm::FillAllControls()
{
	// читаем все настройки по датчикам из ini файла
	// String gain_str="Gain "+IntToStr(Globals::current_diameter);
	ppKadr->Text = FloatToStr(ini->ReadFloat("LCard", "Kadr", 0));
	ppRate->Text = FloatToStr(ini->ReadFloat("LCard", "Rate", 1));
	eFrequency->Text = FloatToStr(ini->ReadFloat("LCard", "Frequency", 1.0));

	// датчики группы прочности
	SpinSG->Value = ini->ReadInteger("OtherSettings", "SensorSG", 16);
	SpinCurr->Value = ini->ReadInteger("OtherSettings", "SensorCurr", 17);

	cbMedianFilter->Checked = ini->ReadBool("OtherSettings",
		"isMedianFilter", 0);
	ComboBoxWidthMF->ItemIndex = ComboBoxWidthMF->Items->IndexOf
		(ini->ReadInteger("OtherSettings", "WidthMedianFilter", 3));

	// заполняем настройки соленоидов для БУРАН 5000
	spIsSolenoidsON->Value = ini->ReadInteger("PP", "spIsSolenoidsON", 27);
  //	spCrossAmperage->Value = ini->ReadInteger("PP", "amperageChannel", 3);
	spSolenoid1->Value = ini->ReadInteger("PP", "spSolenoid1", 28);
	 //	spCrossVoltage->Value = ini->ReadInteger("PP", "voltageChannel", 4);
	spSolenoid2->Value = ini->ReadInteger("PP", "spSolenoid2", 29);

	edSolenoidTresholdU->Text = FloatToStr(ini->ReadFloat("PP", "solenoidTresholdU", 9.1));

  //	spLinearAmperage->Value = ini->ReadInteger("PR", "amperageChannel", 5);
  //	spLinearVoltage->Value = ini->ReadInteger("PR", "voltageChannel", 6);

	// Настройки фильтров продольного

	FSLineOut->Load(ini, "PrOut");
	FSLineIn->Load(ini, "PrIn");
	FSCross->Load(ini, "Pp");

}
// ---------------------------------------------------------------------------

void TADCSettForm::block()
{
	gbCross->Enabled = fl;
 //	gbLinear->Enabled = fl;
	ComboBoxWidthMF->Enabled = fl;
	ppKadr->Enabled = fl;
	ppRate->Enabled = fl;
	SpinSG->Enabled = fl;
	SpinCurr->Enabled = fl;
	cbMedianFilter->Enabled = fl;
	for (int i = 0; i < CSensors; i++)
		CS[i]->SetEnabledR(fl);
	for (int i = 0; i < LSensors; i++)
		LS[i]->SetEnabledR(fl);

	eFrequency->Enabled = fl;

	FSLineOut->SetEnable(fl);
	FSLineIn->SetEnable(fl);
	FSCross->SetEnable(fl);

}

// ------------------------------------------------------------------------------
void __fastcall TADCSettForm::ApplicationEventsMessage(tagMSG &Msg,
	bool &Handled)
{
	// Проверяем на нажатие сочетания клавиш Ctrl+Alt+Enter
	if (Msg.message == WM_HOTKEY) // сообщение наше
	{
		if (Msg.wParam == 0x00F) // идентификатор наш
		{
			Handled = true;
			if (fl)
				fl = false;
			else
				fl = true;
			block();
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TADCSettForm::FormActivate(TObject *Sender)
{
	bool RHKret = RegisterHotKey(ADCSettForm->Handle,
		// Handle окна, которому отправлять сообщения WM_HOTKEY
		0x00F, // УСЛОВНЫЙ идентификатор горячего ключа
		MOD_ALT + MOD_CONTROL, // модификаторы
		VK_RETURN // код клавиши
		);
	FillAllControls();
	for (int i = 0; i < CSensors; i++)
		CS[i]->LoadSettings();
	for (int i = 0; i < LSensors; i++)
		LS[i]->LoadSettings();
	fl = false;
	block();
	need_redraw = false;
}

// ------------------------------------------------------------------------------
void __fastcall TADCSettForm::bSaveClick(TObject *Sender)
{
	wchar_t *s = edSolenoidTresholdU->Text.c_str();
	for(int i = 0, len = wcslen(s); i < len; ++i)
	{
		if(',' == s[i]){s[i] = '.'; break;}
    }
	double tmp = _wtof(s);
	if(0.0 >= tmp)
	{
		Application->MessageBoxW(L"Порог перегрева соленоида введён некорректно",L"Ошибка",MB_OK|MB_ICONERROR);
		return;
	}
	ini->WriteFloat("PP", "solenoidTresholdU", tmp);
	// Сохраняем данные по датчикам
	for (int i = 0; i < CSensors; i++)
		CS[i]->SaveSettings();
	for (int i = 0; i < LSensors; i++)
		LS[i]->SaveSettings();

	// сохраняем прочие настройки

	ini->WriteInteger("OtherSettings", "SensorSG", SpinSG->Value);
	ini->WriteInteger("OtherSettings", "SensorCurr", SpinCurr->Value);

	ini->WriteBool("OtherSettings", "isMedianFilter", cbMedianFilter->Checked);
	ini->WriteInteger("OtherSettings", "WidthMedianFilter",
		ComboBoxWidthMF->Text.ToInt());
	// Данные по соленоидам

	ini->WriteInteger("PP", "spIsSolenoidsON", spIsSolenoidsON->Value);
//	ini->WriteInteger("PP", "amperageChannel", spCrossAmperage->Value);
ini->WriteInteger("PP", "spSolenoid1", spSolenoid1->Value);
//	ini->WriteInteger("PP", "voltageChannel", spCrossVoltage->Value);
ini->WriteInteger("PP", "spSolenoid2", spSolenoid2->Value);

 //	ini->WriteInteger("PR", "amperageChannel", spLinearAmperage->Value);
  //	ini->WriteInteger("PR", "voltageChannel", spLinearVoltage->Value);

	ini->WriteFloat("LCard", "Kadr", (double)StrToFloat(this->ppKadr->Text));
	ini->WriteFloat("LCard", "Rate", (double)StrToFloat(this->ppRate->Text));
	ini->WriteFloat("LCard", "Frequency",
		(double)StrToFloat(this->eFrequency->Text));
	ini->WriteInteger("LCard", "SyncMode", 0);
	ini->WriteInteger("LCard", "SyncStartMode", 0);

	lcard->LoadSettings();

	FSLineOut->Save(ini, "PrOut");
	FSLineIn->Save(ini, "PrIn");
	FSCross->Save(ini, "Pp");

	SystemConst::isCrossDigitalFilter = FSCross->IsUsed();
	SystemConst::isLinearDigitalFilterOut = FSLineOut->IsUsed();
	SystemConst::isLinearDigitalFilterIn = FSLineIn->IsUsed();
	ini->WriteBool("Filters", "isFilterPp", FSCross->IsUsed());
	ini->WriteBool("Filters", "isFilterPrOut", FSLineOut->IsUsed());
	ini->WriteBool("Filters", "isFilterPrIn", FSLineIn->IsUsed());
	CrossFilter->LoadSettings();//setSettingsFromIniFile();
	LinearFilterIn->LoadSettings();//setSettingsFromIniFile();
	LinearFilterOut->LoadSettings();//setSettingsFromIniFile();
	need_redraw = true;
}

// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::bExitClick(TObject *Sender)
{
	ADCSettForm->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::eThresholdResistKeyPress(TObject *Sender,
	wchar_t &Key)
{
	if (!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',')))
		Key = 0x00;
}
// ---------------------------------------------------------------------------

void __fastcall TADCSettForm::CopyAllSensorClick(TObject *Sender)
{
	for (int i = 0; i < CSensors; i++)
		CS[i]->CBRange->ItemIndex = CrossItemIndex;
}

// ---------------------------------------------------------------------------
void __fastcall TADCSettForm::ContextPopup(TObject *Sender, TPoint &MousePos,
	bool &Handled)
{
	if (((TComboBox*)Sender)->Tag == 0)
		CrossItemIndex = ((TComboBox*)Sender)->ItemIndex;
	else
		LinearItemIndex = ((TComboBox*)Sender)->ItemIndex;
}
// ---------------------------------------------------------------------------

void __fastcall TADCSettForm::CopyAllSensorLinearClick(TObject *Sender)
{
	for (int i = 0; i < LSensors; i++)
		LS[i]->CBRange->ItemIndex = CrossItemIndex;
}
//----------------------------------------------------------------------------
//Альметьевск
//void TADCSettForm::SaveTubeSettingsToFile(UnicodeString FileName)
void TADCSettForm::SaveTubeSettingsToFile(FILE *file)
{

		// Формат файла .dkb для дефектоскопа

//	FILE *file;
//	file = fopen(AnsiString(FileName).c_str(), "a");
//	if (file == NULL)
//		Application->MessageBoxW(L"Не удалось открыть файл для записи", L"Ошибка",
//		MB_ICONERROR | MB_OK);

//	int intPar = 0;
//	AnsiString strPar = "";
//	double doubPar = 0.0;
//	bool boolPar = false;

//	fprintf(file, "\n");
	AnsiString TypeSize = ini->ReadString("Default","TypeSize","73");
	AnsiString Section = "Type_" + TypeSize;
//	int size = TypeSize.Length();
//	fwrite(&size, sizeof(size), 1, file);
//	fwrite(TypeSize.c_str(), size, 1, file);
	fprintf(file, "%s", TypeSize); // Типоразмер
	fprintf(file, "\n");
	//fprintf(file, "%d %d ", zones, sensors); // кол-во зон,датчиков
	int CrossSensors = Singleton->CrossResult->sensors_a;//CSensors;
	int LinearSensors = Singleton->LinearResult->sensors_a;//LSensors;
//	fwrite(&CrossSensors,sizeof(CrossSensors),1,file);
//	fwrite(&LinearSensors,sizeof(LinearSensors),1,file);
	fprintf(file, "%d %d ", CrossSensors, LinearSensors);

//		CrossBorder1=25
//		CrossBorder2=8
//		LinearBorder1=60
//		LinearBorder2=25
//		LinearBorder1Inner=65
//		LinearBorder2Inner=25
//		ThicknessBorder1=3,8
//		ThicknessBorder2=0

	fprintf(file, "%f ", ini->ReadFloat(Section, "CrossBorder1", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "CrossBorder2", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "LinearBorder1", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "LinearBorder2", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "LinearBorder1Inner", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "LinearBorder2Inner", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "ThicknessBorder1", 0.0));
	fprintf(file, "%f ", ini->ReadFloat(Section, "ThicknessBorder2", 0.0));
//	doubPar = ini->ReadFloat(Section, "CrossBorder1", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "CrossBorder2", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearBorder1", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearBorder2", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearBorder1Inner", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearBorder2Inner", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "ThicknessBorder1", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "ThicknessBorder2", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);

//		Min_Good_Length=36
//		CrossDeadZoneStart=0
//		CrossDeadZoneFinish=0
//		LinearDeadZoneStart=0
//		LinearDeadZoneFinish=0
//		ThicknessDeadZoneStart=100
//		ThicknessDeadZoneFinish=100

	fprintf(file, "%d ", ini->ReadInteger(Section, "Min_Good_Length", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "CrossDeadZoneStart", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "CrossDeadZoneFinish", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "LinearDeadZoneStart", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "LinearDeadZoneFinish", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "ThicknessDeadZoneStart", 0));
	fprintf(file, "%d ", ini->ReadInteger(Section, "ThicknessDeadZoneFinish", 0));
//	doubPar = ini->ReadFloat(Section, "Min_Good_Length", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "CrossDeadZoneStart", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "CrossDeadZoneFinish", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearDeadZoneStart", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "LinearDeadZoneFinish", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "ThicknessDeadZoneStart", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);
//	doubPar = ini->ReadFloat(Section, "ThicknessDeadZoneFinish", 0.0);
//	fwrite(&doubPar,sizeof(double),1,file);

//		[Gain 48]
//		Gain0=1
//		Gain7=1
	fprintf(file, "\n");
	int diameter = ini->ReadInteger(Section, "Diameter", 73);
	Section = "Gain "+IntToStr(diameter);
	for(int i = 0; i < CrossSensors; i++)
//	{
//		doubPar = ini->ReadFloat(Section, ("Gain"+IntToStr(i)), 0.0);
//		fwrite(&doubPar,sizeof(double),1,file);
//	}
		fprintf(file, "%f ", ini->ReadFloat(Section, ("Gain"+IntToStr(i)), 0.0));
//		[Range_PP_48]
//		Range0=1
//		Range7=1
	fprintf(file, "\n");
	Section = "Range_PP_"+TypeSize;
	for(int i = 0; i < CrossSensors; i++)
//	{
//		intPar = ini->ReadInteger(Section, ("Range"+IntToStr(i)), 0.0);
//		fwrite(&intPar,sizeof(int),1,file);
//	}
		fprintf(file, "%d ", ini->ReadInteger(Section, ("Range"+IntToStr(i)), 0));
//		[CollectedMode_PP_48]
//		Mode0=0
//		Mode7=0
	fprintf(file, "\n");
	Section = "CollectedMode_PP_"+TypeSize;
	for(int i = 0; i < CrossSensors; i++)
//	{
//		intPar = ini->ReadInteger(Section, ("Mode"+IntToStr(i)), 0.0);
//		fwrite(&intPar,sizeof(int),1,file);
//	}
		fprintf(file, "%d ", ini->ReadInteger(Section, ("Mode"+IntToStr(i)), 0));
//		[PR]
//		Gain0=2
//		Gain3=1
	fprintf(file, "\n");
	Section = "PR";
	for(int i = 0; i < LinearSensors; i++)
//	{
//		doubPar = ini->ReadFloat(Section, ("Gain"+IntToStr(i)), 0.0);
//		fwrite(&doubPar,sizeof(double),1,file);
//	}
		fprintf(file, "%f ", ini->ReadFloat(Section, ("Gain"+IntToStr(i)), 0.0));
//		[Range_PR_48]
//		Range0=1
//		Range3=1
	fprintf(file, "\n");
	Section = "Range_PR_"+TypeSize;
	for(int i = 0; i < LinearSensors; i++)
//	{
//		intPar = ini->ReadInteger(Section, ("Range"+IntToStr(i)), 0.0);
//		fwrite(&intPar,sizeof(int),1,file);
//	}
		fprintf(file, "%d ", ini->ReadInteger(Section, ("Range"+IntToStr(i)), 0));
//		[CollectedMode_PR_48]
//		Mode0=0
//		Mode3=0
	fprintf(file, "\n");
	Section = "CollectedMode_PR_"+TypeSize;
	for(int i = 0; i < LinearSensors; i++)
//	{
//		intPar = ini->ReadInteger(Section, ("Mode"+IntToStr(i)), 0.0);
//		fwrite(&intPar,sizeof(int),1,file);
//	}
		fprintf(file, "%d ", ini->ReadInteger(Section, ("Mode"+IntToStr(i)), 0));
//-----------------
	CrossFilter->Save(file);
	LinearFilterIn->Save(file);
	LinearFilterOut->Save(file);

//	intPar = ini->ReadInteger("OtherSettings", "WidthMedianFilter", 5);
//	fwrite(&intPar,sizeof(int),1,file);
//	boolPar = ini->ReadBool("OtherSettings","isMedianFilter",true);
//	fwrite(&boolPar,sizeof(bool),1,file);
	fprintf(file, "%d ", ini->ReadInteger("OtherSettings", "WidthMedianFilter", 5));
	fprintf(file, "%d ", ini->ReadBool("OtherSettings", "isMedianFilter", 5));
//---------------------
//  	Имя оператора
	fprintf(file, "\n");
	AnsiString oper = ini->ReadString("Default","LastAuthorisation","");
	for (int i = 1; i < oper.Length(); i++)
	{
		if(oper[i]==' ') oper[i]='_';
	}
	fprintf(file, "%s ", oper);
//	fprintf(file, "\n");
//	size = oper.Length();
//	fwrite(&size, sizeof(size), 1, file);
//	fwrite(oper.c_str(), size, 1, file);
//	fclose(file);
}



