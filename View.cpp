// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "View.h"
#include "Main.h"
#include "uFunctions.h"
#include "Singleton.h"
#include "Protocol.h"
#include <math.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TViewForm *ViewForm;

// ---------------------------------------------------------------------------
__fastcall TViewForm::TViewForm(TComponent* Owner) : TForm(Owner)
{
	int CrossPos = 0; // Начальная позиция Поперечного
	int LinearPos = 0; // Начальная позиция Продольного
	GainEnable = true; // Начальная позиция Навального
}

void __fastcall TViewForm::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);
	LoadFormPos(this, ini);
	inited = false;
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::FormDestroy(TObject *Sender)
{
	SaveFormPos(this, ini);
	delete ini;
}
// ---------------------------------------------------------------------------

void TViewForm::ViewFormInit()
{
	inited = true;
	ViewCrossChart->Title->Visible = false;
	ViewCrossChart->BottomAxis->Visible = true;
	ViewCrossChart->BottomAxis->Automatic = false;
	ViewCrossChart->BottomAxis->Minimum = 0;
	ViewCrossChart->BottomAxis->Maximum = Globals::max_zones;
	ViewCrossChart->LeftAxis->Visible = true;
	ViewCrossChart->LeftAxis->Automatic = false;
	ViewCrossChart->LeftAxis->Minimum = 0;
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	ViewCrossChart->BottomAxis->EndPosition = 99;
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		ViewCrossChart->Series[i]->ColorEachPoint = true;
		ViewCrossChart->Series[i]->Marks->Visible = false;
		((TBarSeries*) ViewCrossChart->Series[i])->BarWidthPercent = 100;
		((TBarSeries*) ViewCrossChart->Series[i])->MultiBar = mbStacked;
		((TBarSeries*) ViewCrossChart->Series[i])->SideMargins = false;
		((TBarSeries*) ViewCrossChart->Series[i])->OffsetPercent = 50;
		ViewCrossChart->Series[i]->Visible=i<Singleton->CrossResult->sensors_a;
	}

	ViewLinearChart->Title->Visible = false;
	ViewLinearChart->BottomAxis = ViewCrossChart->BottomAxis;
	ViewLinearChart->LeftAxis = ViewCrossChart->LeftAxis;
	ViewLinearChart->LeftAxis->Maximum = Singleton->LinearResult->sensors_a;
	for (int i = 0; i < Singleton->LinearResult->sensors_a; i++)
	{
		ViewLinearChart->Series[i]->ColorEachPoint = true;
		ViewLinearChart->Series[i]->Marks->Visible = false;
		((TBarSeries*) ViewLinearChart->Series[i])->BarWidthPercent = 100;
		((TBarSeries*) ViewLinearChart->Series[i])->MultiBar = mbStacked;
		((TBarSeries*) ViewLinearChart->Series[i])->SideMargins = false;
		((TBarSeries*) ViewLinearChart->Series[i])->OffsetPercent = 50;
	}

	int ws = Screen->Width;
	byte cols = 4; // кол-во столбцов, в которых расположены графики
	short vm = 10;
	short chh = 150, chw = 300; // высота чарта, ширина чарта
	short bvm = gbLinear->Top + gbLinear->Height + gbCross->Top;
	// большой отступ сверху (для 2 главных графиков)
	short hm = gbCross->Left; // верт отступ, гориз отступ

	// создаем графики для результатов
	AnsiString a;
	a = "TViewForm::ViewFormInit: ControlCount=";
	a += ControlCount;
	TPr::pr(a);
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		Carc[i] = new TFRViewZone(this, i, true,true);
		Carc[i]->Parent = this;
		AnsiString a = "FRViewZoneC_";
		a += i;
		Carc[i]->Name = a;
		Carc[i]->OnChangeGain = GainChangeC;
		Carc[i]->OnCalibrate = Calibrate;
		Carc[i]->Visible = false;
	}
	for (int i = 0; i < Singleton->LinearResult->sensors_a; i++)
	{
		Larc[i] = new TFRViewZone(this, i, true,false);
		Larc[i]->Parent = this;
		AnsiString a = "FRViewZoneL_";
		a += i;
		Larc[i]->Name = a;
		Larc[i]->OnChangeGain = GainChangeL;
		Larc[i]->Visible = false;
		Larc[i]->SetInner(Singleton->LinearResult->IsInner());
	}
	CBStep->ItemIndex = ini->ReadInteger("OtherSettings", "RangeStep", 0);
	if (CBStep->ItemIndex < 0)
		CBStep->ItemIndex = 0;
	double val = StrToFloatDef(CBStep->Text, 1);
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		Carc[i]->SetStep(val);
	for (int i = 0; i < Singleton->LinearResult->sensors_a; i++)
		Larc[i]->SetStep(val);
	isRevertCalibrateAccept=false;
	FormResize(this);
	Refresh();
	SetStartGain(true);
	SetStartGain(false);
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormShow(TObject *Sender)
{
	ViewFormInit();
	Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
	Singleton->LinearResult->PutResultOnChart(ViewLinearChart);
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//	if(isRevertCalibrateAccept)
//	{
//		int RetVal = Application->MessageBoxW(L"Принять автокалибровку?",
//			L"Внимание", MB_ICONERROR | MB_YESNO);
//		switch (RetVal)
//		{
//			case 6: //Yes
//			{
//				//да ничего не делаем - все норм!
//			}break;
//			case 7: //No
//			{
//				bRevertGainClick(this);
//			}break;
//			default:
//			{
//				Application->MessageBoxW(L"Ошибка формы запроса!",
//					L"Ошибка!", MB_ICONERROR | MB_OK);
//			}break;
//		}
//	}
	if(need_repaint)
	{
		int RetVal = Application->MessageBoxW(L"Принять изменения?",
			L"Внимание", MB_ICONERROR | MB_YESNOCANCEL);
		switch (RetVal)
		{
			case 6: //Yes
			{
				//да ничего не делаем - все норм!
			}break;
			case 7: //No
			{
				RevertStartGain();
			}break;
			case 2: //Cancel
			{
				Action = caNone;
				return;
			}break;
			default:
			{
				Application->MessageBoxW(L"Ошибка формы запроса!",
					L"Ошибка!", MB_ICONERROR | MB_OK);
			}break;
		}
	}

	if (SystemConst::HideInGain)
		UnregisterHotKey(MainForm->Handle, // Handle окна
		0x01F); // наш идентификатор горячего ключа

	isRevertCalibrateAccept=false;
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		delete Carc[i];
	gainRevertCalibrate.clear();
	for (int i = 0; i < Singleton->LinearResult->sensors; i++)
		delete Larc[i];
	gainStartCross.clear();
	gainStartLinear.clear();
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::ViewCrossChartClickSeries(TCustomChart *Sender,
	TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	// вывод измерений по всем датчикам в конкретной зоне (поперечный)
	CrossPos = ValueIndex;
	Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
	TColor zoneColor;
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		Carc[i]->Visible = true;
		zoneColor = ViewCrossChart->Series[i]->ValueColor[CrossPos];
		ViewCrossChart->Series[i]->ValueColor[CrossPos] = SelectedColor;
		Singleton->CrossResult->PutDataOnFrame(Carc[i], ValueIndex, i, zoneColor);
	}
	for (int i = 0; i < Singleton->LinearResult->sensors; i++)
		Larc[i]->Visible = false;
	// выводим номер зоны в соответствующую панель
	gbCross->Caption = "Поперечный контроль, зона: " + IntToStr(ValueIndex + 1);
	gbLinear->Caption = "Продольный контроль";
	ViewCrossChart->SetFocus();
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	bRevertGain->Visible=true;
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::ViewLinearChartClickSeries(TCustomChart *Sender,
	TChartSeries *Series, int ValueIndex, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	// вывод измерений по всем датчикам в конкретной зоне (продольный)
	LinearPos = ValueIndex;
	Singleton->LinearResult->PutResultOnChart(ViewLinearChart);
	TColor zoneColor;
	for (int i = 0; i < Singleton->LinearResult->sensors; i++)
	{
		Larc[i]->Visible = true;
		zoneColor = ViewLinearChart->Series[i]->ValueColor[LinearPos];
		ViewLinearChart->Series[i]->ValueColor[LinearPos] = SelectedColor;
		Singleton->LinearResult->PutDataOnFrame(Larc[i], ValueIndex, i, zoneColor);
	}
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		Carc[i]->Visible = false;
	gbCross->Caption = "Поперечный контроль";
	gbLinear->Caption = "Продольный контроль, зона: " + IntToStr(ValueIndex + 1);
	ViewLinearChart->SetFocus();
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	bRevertGain->Visible=false;
}

// ---------------------------------------------------------------------------

void __fastcall TViewForm::EditKeyPress(TObject *Sender, wchar_t &Key)
{
	if (!((Key >= '0' && Key <= '9') || (Key == VK_BACK) || (Key == ',')))
		Key = 0x00;
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormActivate(TObject *Sender)
{
	if (SystemConst::HideInGain)
	{
		bool RHKret = RegisterHotKey(MainForm->Handle,
			// Handle окна, которому отправлять сообщения WM_HOTKEY
			0x01F, // УСЛОВНЫЙ идентификатор горячего ключа
			MOD_ALT + MOD_CONTROL, // модификатор
			VK_ADD); // код клавиши +
	}

	SelectedColor = (TColor)ini->ReadInteger("Color", "SelectedZone", 16711680);
	ViewCrossChart->SetFocus();
	need_repaint=false;
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::ApplicationEventsMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_HOTKEY)
	{
		if (Msg.wParam == 0x01F)
		{
			if (SystemConst::HideInGain)
			{
				for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
					Carc[i]->TrigerVisible();
				for (int i = 0; i < Singleton->LinearResult->sensors; i++)
					Larc[i]->TrigerVisible();
			}
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::ViewCrossChartKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	if (Key == 27)
		Close();
	else
	{
		if (Key == 40)
		{
			if (Singleton->LinearResult->zones > 0)
				ViewLinearChart->SetFocus();
		}
		else if (Key == 39)
		{
			CrossPos++;
			if (CrossPos >= Singleton->CrossResult->zones)
				CrossPos = Singleton->CrossResult->zones - 1;
		}
		else if (Key == 37)
		{
			CrossPos--;
			if (CrossPos < 0)
				CrossPos = 0;
		}
		if ((Key == 37) || (Key == 39))
		{
			int i;
			Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
			String gain_str = "Gain " + IntToStr(Globals::current_diameter);
			TColor zoneColor;
			for (i = 0; i < Singleton->CrossResult->sensors_a; i++)
			{
				Carc[i]->Visible = true;
				zoneColor = ViewCrossChart->Series[i]->ValueColor[CrossPos];
				ViewCrossChart->Series[i]->ValueColor[CrossPos] = SelectedColor;
				Singleton->CrossResult->PutDataOnFrame(Carc[i], CrossPos, i, zoneColor);
			}
			// выводим номер зоны в соответствующую панель
			gbCross->Caption = "Поперечный контроль, зона: " + IntToStr(CrossPos + 1);
			gbLinear->Caption = "Продольный контроль";
			for (i = 0; i < Singleton->LinearResult->sensors_a; i++)
				Larc[i]->Visible = false;
			ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
		}
	}
	if (Key == 67)
	{
		if (Application->MessageBox(L"Произвести калибровку по текущей зоне",
			L"Внимание!", MB_YESNO) == IDYES)
		{
			Singleton->CrossResult->AutoCalibration(CrossPos);
			Singleton->CrossResult->ComputeZoneData();
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::ViewLinearChartKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	if (Key == 27)
		Close();
	else
	{
		if (Key == 38)
		{
			ViewCrossChart->SetFocus();
		}
		if (Key == 39)
		{
			LinearPos++;
			if (LinearPos >= Singleton->LinearResult->zones)
				LinearPos = Singleton->LinearResult->zones - 1;
		}
		else if (Key == 37)
		{
			LinearPos--;
			if (LinearPos < 0)
				LinearPos = 0;
		}
		if ((Key == 37) || (Key == 39))
		{
			int i;
			Singleton->LinearResult->PutResultOnChart(ViewLinearChart);
			TColor zoneColor;
			for (i = 0; i < Singleton->LinearResult->sensors; i++)
			{
				Larc[i]->Visible = true;
				zoneColor = ViewLinearChart->Series[i]->ValueColor[LinearPos];
				ViewLinearChart->Series[i]->ValueColor[LinearPos] = SelectedColor;
				Singleton->LinearResult->PutDataOnFrame(Larc[i], LinearPos, i, zoneColor);
			}
			gbCross->Caption = "Поперечный контроль";
			gbLinear->Caption = "Продольный контроль, зона: " +
				IntToStr(LinearPos + 1);
			for (i = 0; i < Singleton->CrossResult->sensors_a; i++)
				Carc[i]->Visible = false;
			ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TViewForm::FormResize(TObject *Sender)
{
	if (!inited)
		return;
	int spaceW = gbCross->Left;
	int spaceH = gbCross->Top;
	gbCross->Width = ClientWidth - spaceW * 2;
	gbLinear->Width = ClientWidth - spaceW * 2;
	int columns = 4;
	double x;
	x = Singleton->LinearResult->sensors_a;
	x /= columns;
	int Lrows = (int)ceil(x);
	x = Singleton->CrossResult->sensors_a;
	x /= columns;
	int Crows = (int)ceil(x);
	int first_top = CBStep->Top + CBStep->Height;

	int w = (ClientWidth - spaceW * 2) / columns;
	int Ch = (ClientHeight - first_top - spaceH * 2) / Crows;
	int Lh = Ch;
	if (Singleton->LinearResult->IsInner())
		Lh *= 2;
	int i;
	i = 0;
	for (int r = 0; ; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (i >= Singleton->LinearResult->sensors_a)
				goto M1;
			Larc[i]->Width = w;
			Larc[i]->Height = Lh;
			Larc[i]->Left = c * w + spaceW;
			Larc[i]->Top = r * Lh + spaceH + first_top;
			i++;
		}
	}
M1:
	i = 0;
	for (int r = 0; ; r++)
	{
		for (int c = 0; c < columns; c++)
		{
			if (i >= Singleton->CrossResult->sensors_a)
				goto M2;
			Carc[i]->Width = w;
			Carc[i]->Height = Ch;
			Carc[i]->Left = c * w + spaceW;
			Carc[i]->Top = r * Ch + spaceH + first_top;
			i++;
		}
	}
M2:
}

// ---------------------------------------------------------------------------
void TViewForm::GainChangeC(int _sensor, double _gain)
{
	String gain_str = "Gain " + IntToStr(Globals::current_diameter);
	AnsiString a="Gain";
	a+=_sensor;
	ini->WriteFloat(gain_str,a, _gain);
	Singleton->CrossResult->SetSensorGain(_gain, _sensor);
	Singleton->CrossResult->ComputeZoneData();
	Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
	TColor zoneColor = ViewCrossChart->Series[_sensor]->ValueColor[CrossPos];
	Singleton->CrossResult->PutDataOnFrame(Carc[_sensor],CrossPos,_sensor,zoneColor);
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	need_repaint=true;
	Singleton->FromFile=true; //чтобы нельзя было подстроить для сверки
}

void TViewForm::GainChangeL(int _sensor, double _gain)
{
	AnsiString a="Gain";
	a+=_sensor;
	ini->WriteFloat("PR", a, _gain);
	Singleton->LinearResult->SetSensorGain(_gain,_sensor);
	Singleton->LinearResult->ComputeZoneData();
	Singleton->LinearResult->PutResultOnChart(ViewLinearChart);
	TColor zoneColor = ViewLinearChart->Series[_sensor]->ValueColor[LinearPos];
	Singleton->LinearResult->PutDataOnFrame(Larc[_sensor],LinearPos,_sensor,zoneColor);
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	need_repaint=true;
	Singleton->FromFile = true; // чтобы нельзя было подстроить для сверки
}

// ---------------------------------------------------------------------------
void TViewForm::Calibrate(TFRViewZone* _arc)
{
	if(!isRevertCalibrateAccept)
	{
		SetRevertGain();
		isRevertCalibrateAccept=true;
	}
	bool* mb=new bool[Singleton->CrossResult->sensors_a];
	for (int s = 0; s < Singleton->CrossResult->sensors_a; s++)
		mb[s]=Carc[s]->NeedCalibrate();
	Singleton->CrossResult->Calibrate(CrossPos, _arc->GetSensor(),mb);
	delete mb;
	//Singleton->CrossResult->SaveSettingsGain();
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	int diameter = ini->ReadInteger(sect,"Diameter",1);
	sect = "Gain " + IntToStr(diameter);
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		AnsiString name = "Gain";
		name += IntToStr(i);
		double newGain = Singleton->CrossResult->GetSensorGain(i); //Carc[i]->Edit1->Text.ToDouble();
		ini->WriteFloat(sect, name, newGain);
	}
	Singleton->CrossResult->ComputeZoneData();
	Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
	for (int s = 0; s < Singleton->CrossResult->sensors_a; s++)
	{
		TColor zoneColor = ViewCrossChart->Series[s]->ValueColor[CrossPos];
		Singleton->CrossResult->PutDataOnFrame(Carc[s], CrossPos, s, zoneColor);
	}
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
	need_repaint = true;
	Singleton->FromFile = true; // чтобы нельзя было подстроить для сверки
}
// ---------------------------------------------------------------------------
void __fastcall TViewForm::CBStepChange(TObject *Sender)
{
	double val = StrToFloatDef(CBStep->Text, 1);
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		Carc[i]->SetStep(val);
	for (int i = 0; i < Singleton->LinearResult->sensors_a; i++)
		Larc[i]->SetStep(val);
}

// ---------------------------------------------------------------------------
void __fastcall TViewForm::bRevertGainClick(TObject *Sender)
{
	if(isRevertCalibrateAccept)
	{
		AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
		int diameter = ini->ReadInteger(sect,"Diameter",1);
		sect = "Gain " + IntToStr(diameter);
		AnsiString str = "";
		int i = 0;
		for (vector<double>::iterator iter = gainRevertCalibrate.begin();
			iter != gainRevertCalibrate.end(); iter++)
		//for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		{
			double gain = *iter;//gainRevertCalibrate[i];
			//todo убрать после отладки
			str = "bRevertGain " + IntToStr(i) + " " + FloatToStr(gain);
			TPr::pr(str);
			//---
			AnsiString name = "Gain";
			name += IntToStr(i);
			ini->WriteFloat(sect, name, gain);
			Carc[i]->Edit1->Text=FloatToStr(gain);
			Singleton->CrossResult->SetSensorGain(gain, i);
			i++;
		}
			Singleton->CrossResult->ComputeZoneData();
			Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
        for (unsigned int i = 0; i < Singleton->CrossResult->sensors_a; i++)
		{
			TColor zoneColor = ViewCrossChart->Series[i]->ValueColor[CrossPos];
			Singleton->CrossResult->PutDataOnFrame(Carc[i],CrossPos,i,zoneColor);
		}
		ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;
		need_repaint=true;
		isRevertCalibrateAccept=false;
		Singleton->FromFile=true; //чтобы нельзя было подстроить для сверки
	}
}
//---------------------------------------------------------------------------
void TViewForm::SetRevertGain()
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	int diameter = ini->ReadInteger(sect,"Diameter",1);
	sect = "Gain " + IntToStr(diameter);
	gainRevertCalibrate.clear();
	for (int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		AnsiString name = "Gain";
		name += IntToStr(i);
		double oldGain = ini->ReadFloat(sect, name, 0);
		gainRevertCalibrate.push_back(oldGain);
	}
}
//---------------------------------------------------------------------------
void TViewForm::SetStartGain(bool isLinear)
{
	AnsiString sect = "";
	int SensorsNum = 0;
	if(isLinear)
	{
		sect = "PR";
		gainStartLinear.clear();
		SensorsNum = Singleton->LinearResult->sensors_a;
	}
	else
	{
		sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
		int diameter = ini->ReadInteger(sect,"Diameter",1);
		sect = "Gain " + IntToStr(diameter);
		gainStartCross.clear();
		SensorsNum = Singleton->CrossResult->sensors_a;
	}
	for (int i = 0; i < SensorsNum; i++)
	{
		AnsiString name = "Gain";
		name += IntToStr(i);
		double oldGain = ini->ReadFloat(sect, name, 0);
		if(isLinear)
			gainStartLinear.push_back(oldGain);
		else gainStartCross.push_back(oldGain);
	}
}
//---------------------------------------------------------------------------
void TViewForm::RevertStartGain()
{
	AnsiString sect = "";
	int i = 0;
	sect = "PR";
	for (vector<double>::iterator iter = gainStartLinear.begin();
		iter != gainStartLinear.end(); iter++)
	{
		double gain = *iter;
		AnsiString name = "Gain";
		name += IntToStr(i);
		ini->WriteFloat(sect, name, gain);
		Larc[i]->Edit1->Text=FloatToStr(gain);
		Singleton->LinearResult->SetSensorGain(gain, i);
		i++;
	}
		Singleton->LinearResult->ComputeZoneData();
		Singleton->LinearResult->PutResultOnChart(ViewLinearChart);
	for (unsigned int i = 0; i < Singleton->LinearResult->sensors_a; i++)
	{
		TColor zoneColor = ViewLinearChart->Series[i]->ValueColor[LinearPos];
		Singleton->LinearResult->PutDataOnFrame(Larc[i],LinearPos,i,zoneColor);
	}

	sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	int diameter = ini->ReadInteger(sect,"Diameter",1);
	sect = "Gain " + IntToStr(diameter);
    i = 0;
	for (vector<double>::iterator iter = gainStartCross.begin();
		iter != gainStartCross.end(); iter++)
	{
		double gain = *iter;
		AnsiString name = "Gain";
		name += IntToStr(i);
		ini->WriteFloat(sect, name, gain);
		Carc[i]->Edit1->Text=FloatToStr(gain);
		Singleton->CrossResult->SetSensorGain(gain, i);
		i++;
	}
		Singleton->CrossResult->ComputeZoneData();
		Singleton->CrossResult->PutResultOnChart(ViewCrossChart);
	for (unsigned int i = 0; i < Singleton->CrossResult->sensors_a; i++)
	{
		TColor zoneColor = ViewCrossChart->Series[i]->ValueColor[CrossPos];
		Singleton->CrossResult->PutDataOnFrame(Carc[i],CrossPos,i,zoneColor);
	}
	ViewCrossChart->LeftAxis->Maximum = Singleton->CrossResult->sensors_a;

	need_repaint=true;
	isRevertCalibrateAccept=false;
	Singleton->FromFile=true; //чтобы нельзя было подстроить для сверки
}
//----------------------------------------------------------------------------
void __fastcall TViewForm::bRevertStartClick(TObject *Sender)
{
	//todo
	RevertStartGain();
}
//---------------------------------------------------------------------------

