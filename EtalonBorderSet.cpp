// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonBorderSet.h"
#include "uFunctions.h"
#include <Series.hpp>
#include "Singleton.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Global.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "Series"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TEtalonBorderSet::TEtalonBorderSet(TEtalonNewDefectForm* Owner, TIniFile* _ini,
	AnsiString _title) : TForm(Owner)
{
	EtalonNewDefectForm = Owner;
	ini = _ini;
	title = _title;
	borderOut1 = 0.0;
	borderOut2 = 0.0;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		((TForm*)Sender)->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormCreate(TObject *Sender)
{
	LoadFormPos(this, ini);
	SetMetric(0, 0);
	KeyUpFlag = true;
}

// ---------------------------------------------------------------------------
void TEtalonBorderSet::SetInner(bool _v)
{
	if(_v)
		Chart1->LeftAxis->Minimum = -100;
	else
		Chart1->LeftAxis->Minimum = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormDestroy(TObject *Sender)
{
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void TEtalonBorderSet::SetCaption(AnsiString _cap)
{
	Caption = title + _cap;
}

void TEtalonBorderSet::SetMetric(double _start, double _zone_size)
{
	start = _start;
	zone_size = _zone_size;
	if (start < 0)
		start = 0;
	if (zone_size < 0)
		zone_size = 0;
}

void __fastcall TEtalonBorderSet::Chart1ClickBackground(TCustomChart *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	double x, y;
	Chart1->Series[0]->GetCursorValues(x, y);
	if (x < 0)
		x = 0;
	int StartOfZone = (int)start;
	int CurrentZoneMeasures = Chart1->Series[0]->Count();
	int zone_length = Singleton->CrossResult->GetZoneLength();
	for (vector<int>::iterator iter = DevidersMeasures.begin();
		iter != DevidersMeasures.end(); iter++)
	{
		if(x < *iter)
		{
			CurrentZoneMeasures = *iter;
			if(iter != DevidersMeasures.begin())
			{
				iter--;
				CurrentZoneMeasures -= *iter;
				x -= *iter;
			}
			break;
		}
		else StartOfZone += zone_length;
	}

	double Range = StartOfZone + x * zone_length / CurrentZoneMeasures;
	//Chart1->Series[0]->Count();
	//double Range = start + x * zone_size / Chart1->Series[0]->Count();
	AnsiString a = "Расстояние: ~";
	a += (int)Range;
	a += " мм";
	StatusBar1->Panels->Items[0]->Text = a;

	//выставляем границы дефекта эталона
	if(Button == mbLeft)
		SetEBorder(0, y); //верхняя граница
	if(Button == mbRight)
		SetEBorder(1, y); //нижняя граница
}

// ---------------------------------------------------------------------------
void TEtalonBorderSet::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
	for (vector<TLineSeries*>::iterator iter = DefectsSeries.begin();
		iter != DefectsSeries.end(); iter++)
	{
		delete *iter;
	}
	DefectsSeries.clear();
	for (vector<TLineSeries*>::iterator iter = DevidersSeries.begin();
		iter != DevidersSeries.end(); iter++)
	{
		delete *iter;
	}
	DevidersSeries.clear();
	DevidersMeasures.clear();

};
// ---------------------------------------------------------------------------
void TEtalonBorderSet::SetBorder(TLineSeries* _series, double _level, TColor _color)
{
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
	_series->AddXY(BarOut->MaxXValue(), _level, "", _color);
}
//----------------------------------------------------------------------------
void TEtalonBorderSet::SetCurrPoint(int _curr_start_zone, int _curr_final_zone
	, int _curr_sens, bool _IsLinear)
{
	curr_start_zone=_curr_start_zone;
	curr_final_zone=_curr_final_zone;
	//curr_zone=_curr_zone;
	curr_sens=_curr_sens;
	IsLinear=_IsLinear;
}
//---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(KeyUpFlag)
	{
		curr_defect = cbEtalonTubeDefects->ItemIndex;
		KeyUpFlag = false;
	}
	switch(Key)
	{
	case 37:
		{
			//curr_zone--;
			if (curr_start_zone > 0)
			{
				curr_start_zone--;
				curr_final_zone--;
				if (IsLinear) Singleton->LinearResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
				else Singleton->CrossResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			}
			//else curr_zone=0;
			break;
		}
	case 39:
		{
			//curr_zone++;
			if (IsLinear && curr_final_zone < Singleton->LinearResult->zones)
			{
				curr_start_zone++;
				curr_final_zone++;
				Singleton->LinearResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			}
			else if (!IsLinear && curr_final_zone < Singleton->CrossResult->zones)
			{
                curr_start_zone++;
				curr_final_zone++;
				Singleton->CrossResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			}
			//else curr_zone--;
			break;
		}

	case 38:
		{
			curr_sens++;
			if (IsLinear && curr_sens < Singleton->LinearResult->sensors_a)
				Singleton->LinearResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			else if (!IsLinear && curr_sens < Singleton->CrossResult->sensors_a)
				Singleton->CrossResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			else curr_sens--;
			break;
		}
	case 40:
		{
			curr_sens--;
			if (curr_sens >= 0)
			{
				if (IsLinear) Singleton->LinearResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
				else Singleton->CrossResult->PutDataOnChartEtalonBorder(this
					, curr_start_zone, curr_final_zone, curr_sens);
			}
			else curr_sens=0;
			break;
		}
	case 27:
		{
			Close();
			break;
		}
	}
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::ViewEtalonCheck(int _startZone, int _finalZone)
{
	//TPr::pr("ViewEtalonCheck");
	AnsiString Str = "";
	int zone_length = Singleton->CrossResult->GetZoneLength();
	int startX;
	int finalX;
	int startMeas;
	int finalMeas;
	double TopBorder;
	double BottomBorder;
	Chart1->SeriesList->AddGroup("Defects");
	String sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	String param = "";

	int Module = 0;
	if(EtalonNewDefectForm->cbModule->Text == "Поперечный")
		Module = 1;
	if(EtalonNewDefectForm->cbModule->Text == "Продольный")
		Module = 2;
	//проверяем наш ли это модуль
	if((!IsLinear && (Module == 1)) || (IsLinear && (Module == 2)))
	{
		//если участок поиска пересекается с отображаемым участком,
		//то выясняем его точки начала и конца
		startX = EtalonNewDefectForm->eDefPoint->Text.ToInt()
			- EtalonNewDefectForm->eDefRadius->Text.ToInt();
		finalX = EtalonNewDefectForm->eDefPoint->Text.ToInt()
			+ EtalonNewDefectForm->eDefRadius->Text.ToInt();
		bool Include = true;
		if(startX < (start + zone_size))
		{
			if(startX > start)
				startMeas = CalcMeasFromRange((int)(startX - start));
			else startMeas = 0;
		}
		else
			Include = false;
		if(Include && (finalX > start))
		{
			if(finalX < (start + zone_size))
				finalMeas = CalcMeasFromRange((int)(finalX - start));
			else finalMeas = Chart1->Series[0]->Count();
		}
		else
			Include = false;
		//рисуем отрезки поиска
		if (Include)
		{
			//Стираем старые границы
			for (vector<TLineSeries*>::iterator iter = DefectsSeries.begin();
				iter != DefectsSeries.end(); iter++)
			{
				delete *iter;
			}
			DefectsSeries.clear();
			//Рисуем верхнюю границу
			int TopBorderPercent = EtalonNewDefectForm->eTopPercentage->Text.ToInt();
			if(TopBorderPercent != 0)
			{
				if(IsLinear)
					param = "Linear";
				else
					param = "Cross";
				if(EtalonNewDefectForm->cbTopBorder->Text == "Брак")
					param += "Border1";
				else
					param += "Border2";
				TopBorder = ini->ReadFloat(sect,param,0) * TopBorderPercent / 100;
				SetBrackBorder(startMeas,finalMeas,TopBorder,clFuchsia);
				//Если включен внутренний фильтр продольника
				if(IsLinear && SystemConst::isLinearDigitalFilterIn)
				{
					if(EtalonNewDefectForm->cbTopBorder->Text == "Брак")
						param = "LinearBorder1Inner";
					else
						param = "LinearBorder2Inner";
					TopBorder = ini->ReadFloat(sect,param,0) * TopBorderPercent / -100;
					SetBrackBorder(startMeas,finalMeas,TopBorder,clFuchsia);
				}
			}
			//Рисуем нижнюю границу
			int BottomBorderPercent = EtalonNewDefectForm->eBottomPercentage->Text.ToInt();
			if(BottomBorderPercent != 0)
			{
				if(IsLinear)
					param = "Linear";
				else
					param = "Cross";
				if(EtalonNewDefectForm->cbBottomBorder->Text == "Брак")
					param += "Border1";
				else
					param += "Border2";
				BottomBorder = ini->ReadFloat(sect,param,0) * BottomBorderPercent / 100;
				SetBrackBorder(startMeas,finalMeas,BottomBorder,clFuchsia);
				//Если включен внутренний фильтр продольника
				if(IsLinear && SystemConst::isLinearDigitalFilterIn)
				{
					if(EtalonNewDefectForm->cbBottomBorder->Text == "Брак")
						param = "LinearBorder1Inner";
					else
						param = "LinearBorder2Inner";
					BottomBorder = ini->ReadFloat(sect,param,0) * BottomBorderPercent / -100;
					SetBrackBorder(startMeas,finalMeas,BottomBorder,clFuchsia);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
int TEtalonBorderSet::CalcMeasFromRange(int _x)
{
	double Measure;
	int CurrentZoneMeasures = 0;
	int StartOfZoneMeas = 0;
	int LastMeas = Chart1->Series[0]->Count();
	int zone_length = Singleton->CrossResult->GetZoneLength();
	int CountZone = 0;
	//Определяем конкретную зону (из нескольких)
	for (vector<int>::iterator iter = DevidersMeasures.begin();
		iter != DevidersMeasures.end(); iter++)
	{
		if(_x < (CountZone + 1) * zone_length)
		{
			StartOfZoneMeas = 0;
			_x -= (CountZone * zone_length);
			CurrentZoneMeasures = *iter;
			if(iter != DevidersMeasures.begin())
			{
				iter--;
				CurrentZoneMeasures -= *iter;
				//_x -= *iter;
				StartOfZoneMeas = *iter;
			}
			break;
		}
		CountZone++;
	}
	//окончательный расчет нужного измерения
	double x = _x;
	Measure = StartOfZoneMeas + x / zone_length * CurrentZoneMeasures;
	return (int)Measure;
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::SetBrackBorder(int _startMeas,int _finalMeas
	,double _Border,TColor _color)
{
	TLineSeries *s_new = new TLineSeries(this);
	s_new->Name = "DefectSeries"+IntToStr((int)DefectsSeries.size());
	s_new->ParentChart = Chart1;
	s_new->LinePen->Width = 2;
	s_new->AddXY(_startMeas,_Border,"",_color);
	s_new->AddXY(_finalMeas,_Border,"",_color);
	DefectsSeries.push_back(s_new);
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::AddDevider(int _meas,TColor _color)
{
	TLineSeries *s_new = new TLineSeries(this);
	s_new->Name = "DeviderSeries"+IntToStr((int)DevidersSeries.size());
	s_new->ParentChart = Chart1;
	s_new->LinePen->Width = 2;
	s_new->AddXY(_meas,0,"",_color);
	s_new->AddXY(_meas,100,"",_color);
	DevidersSeries.push_back(s_new);
	DevidersMeasures.push_back(_meas);
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::AddDeviderIn(int _meas,TColor _color)
{
	TLineSeries *s_new = new TLineSeries(this);
	s_new->Name = "DeviderSeries"+IntToStr((int)DevidersSeries.size());
	s_new->ParentChart = Chart1;
	s_new->LinePen->Width = 2;
	s_new->AddXY(_meas,-100,"",_color);
	s_new->AddXY(_meas,100,"",_color);
	DevidersSeries.push_back(s_new);
	DevidersMeasures.push_back(_meas);
}
//---------------------------------------------------------------------------

void __fastcall TEtalonBorderSet::cbEtalonTubeDefectsKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if (Key != 27)
	{
		cbEtalonTubeDefects->ItemIndex = curr_defect;
		KeyUpFlag = true;
	}
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::SetEBorder(int _border,int _Y_coord)
{
	double newBorder = 0.0;
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	AnsiString param = "";
	if(_border == 0) //верхняя граница
	{
		if(EtalonNewDefectForm->cbTopBorder->Text == "Брак")
			newBorder = _Y_coord * 100.0 / borderOut1;
		else if(EtalonNewDefectForm->cbTopBorder->Text == "Класс2")
			newBorder = _Y_coord * 100.0 / borderOut2;
		EtalonNewDefectForm->eTopPercentage->Text = IntToStr((int)newBorder);
		//проверим не надо ли сдвинуть вторую границу
		int BottomBorderPercent = EtalonNewDefectForm->eBottomPercentage->Text.ToInt();
		if(BottomBorderPercent != 0)
		{
			if(IsLinear)
				param = "Linear";
			else
				param = "Cross";
			if(EtalonNewDefectForm->cbBottomBorder->Text == "Брак")
				param += "Border1";
			else
				param += "Border2";
			int BottomBorder = ini->ReadFloat(sect,param,0) * BottomBorderPercent / 100;
			if(_Y_coord < BottomBorder)
				SetEBorder(1,_Y_coord);
		}
	}
	else if(_border == 1) //нижняя граница
	{
		if(EtalonNewDefectForm->cbBottomBorder->Text == "Брак")
			newBorder = _Y_coord * 100.0 / borderOut1;
		else if(EtalonNewDefectForm->cbBottomBorder->Text == "Класс2")
			newBorder = _Y_coord * 100.0 / borderOut2;
		EtalonNewDefectForm->eBottomPercentage->Text = IntToStr((int)newBorder);
		//проверим не надо ли сдвинуть вторую границу
		int TopBorderPercent = EtalonNewDefectForm->eTopPercentage->Text.ToInt();
		if(TopBorderPercent != 0)
		{
			if(IsLinear)
				param = "Linear";
			else
				param = "Cross";
			if(EtalonNewDefectForm->cbTopBorder->Text == "Брак")
				param += "Border1";
			else
				param += "Border2";
			int TopBorder = ini->ReadFloat(sect,param,0) * TopBorderPercent / 100;
			if(_Y_coord > TopBorder)
				SetEBorder(0,_Y_coord);
		}
	}
	else return; //какая-то фигня
	//todo если верхняя стала ниже нижней или наоборот - сдвинем и ее

	//перерисовать
	ViewEtalonCheck(curr_start_zone, curr_final_zone);
}
//---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormShow(TObject *Sender)
{
	SaveOldBorders();
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::SaveOldBorders()
{
	oldTopBorderPercent = EtalonNewDefectForm->eTopPercentage->Text.ToInt();
	oldBottomBorderPercent = EtalonNewDefectForm->eBottomPercentage->Text.ToInt();
	oldTopBorderSource = EtalonNewDefectForm->cbTopBorder->Text;
	oldBottomBorderSource = EtalonNewDefectForm->cbBottomBorder->Text;
}
//---------------------------------------------------------------------------
void TEtalonBorderSet::LoadOldBorders()
{
	EtalonNewDefectForm->eTopPercentage->Text = IntToStr(oldTopBorderPercent);
	EtalonNewDefectForm->eBottomPercentage->Text = IntToStr(oldBottomBorderPercent);
	EtalonNewDefectForm->cbTopBorder->Text = oldTopBorderSource;
	EtalonNewDefectForm->cbBottomBorder->Text = oldBottomBorderSource;
}
//---------------------------------------------------------------------------
void __fastcall TEtalonBorderSet::FormClose(TObject *Sender, TCloseAction &Action)
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
			LoadOldBorders();
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
//---------------------------------------------------------------------------

