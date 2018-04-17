// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFViewZoneD.h"
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
__fastcall TFViewZoneD::TFViewZoneD(TComponent* Owner, TIniFile* _ini,
	AnsiString _title) : TForm(Owner)
{
	ini = _ini;
	title = _title;
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneD::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		((TForm*)Sender)->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneD::FormCreate(TObject *Sender)
{
	LoadFormPos(this, ini);
	SetMetric(0, 0);
	KeyUpFlag = true;
}

// ---------------------------------------------------------------------------
void TFViewZoneD::SetInner(bool _v)
{
	if(_v)
		Chart1->LeftAxis->Minimum = -100;
	else
		Chart1->LeftAxis->Minimum = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneD::FormDestroy(TObject *Sender)
{
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void TFViewZoneD::SetCaption(AnsiString _cap)
{
	Caption = title + _cap;
}

void TFViewZoneD::SetMetric(double _start, double _zone_size)
{
	start = _start;
	zone_size = _zone_size;
	if (start < 0)
		start = 0;
	if (zone_size < 0)
		zone_size = 0;
}

void __fastcall TFViewZoneD::Chart1ClickBackground(TCustomChart *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	double x, y;
	Chart1->Series[0]->GetCursorValues(x, y);
	if (x < 0)
		x = 0;
	int StartOfZone = (int)start;
	int CurrentZoneMeasures = Chart1->Series[0]->Count();
	int zone_length = Singleton->CrossResult->GetZoneLength();
	if ((x > DevidMeas1) && (DevidMeas1 > 0))
	{
		StartOfZone += zone_length;
		if ((x > DevidMeas2) && (DevidMeas2 > 0))
		{
			StartOfZone += zone_length;
			x -= DevidMeas2;
			CurrentZoneMeasures = CurrentZoneMeasures - DevidMeas2;
		}
		else
		{
			x -= DevidMeas1;
			if (DevidMeas2 > DevidMeas1)
				CurrentZoneMeasures = DevidMeas2 - DevidMeas1;
			else CurrentZoneMeasures = CurrentZoneMeasures - DevidMeas1;
		}
	}
	else
	{
		CurrentZoneMeasures = DevidMeas1;
	}
	double Range = StartOfZone + x * zone_length / CurrentZoneMeasures;
	//Chart1->Series[0]->Count();
	//double Range = start + x * zone_size / Chart1->Series[0]->Count();
	AnsiString a = "Расстояние: ~";
	a += (int)Range;
	a += " мм";
	StatusBar1->Panels->Items[0]->Text = a;
}

// ---------------------------------------------------------------------------
void TFViewZoneD::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
	for (vector<TLineSeries*>::iterator iter = DefectsSeries.begin();
		iter != DefectsSeries.end(); iter++)
	{
		delete *iter;
	}
	DefectsSeries.clear();

};
// ---------------------------------------------------------------------------
void TFViewZoneD::SetBorder(TLineSeries* _series, double _level, TColor _color)
{
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
	_series->AddXY(BarOut->MaxXValue(), _level, "", _color);
}

void TFViewZoneD::SetCurrPoint(int _curr_zone, int _curr_sens, bool _IsLinear)
{
	curr_zone=_curr_zone;
	curr_sens=_curr_sens;
	IsLinear=_IsLinear;
}
//---------------------------------------------------------------------------

void __fastcall TFViewZoneD::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

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
			curr_zone--;
			if (curr_zone >= 0)
			{
				if (IsLinear) Singleton->LinearResult->PutDataOnChartX(this, curr_zone, curr_sens);
				else Singleton->CrossResult->PutDataOnChartX(this, curr_zone, curr_sens);
			}
			else curr_zone=0;
			break;
		}
	case 39:
		{
			curr_zone++;
			if (IsLinear && curr_zone < Singleton->LinearResult->zones)
				Singleton->LinearResult->PutDataOnChartX(this, curr_zone, curr_sens);
			else if (!IsLinear && curr_zone < Singleton->CrossResult->zones)
				Singleton->CrossResult->PutDataOnChartX(this, curr_zone, curr_sens);
			else curr_zone--;
			break;
		}

	case 38:
		{
			curr_sens++;
			if (IsLinear && curr_sens < Singleton->LinearResult->sensors_a)
				Singleton->LinearResult->PutDataOnChartX(this, curr_zone, curr_sens);
			else if (!IsLinear && curr_sens < Singleton->CrossResult->sensors_a)
				Singleton->CrossResult->PutDataOnChartX(this, curr_zone, curr_sens);
			else curr_sens--;
			break;
		}
	case 40:
		{
			curr_sens--;
			if (curr_sens >= 0)
			{
				if (IsLinear) Singleton->LinearResult->PutDataOnChartX(this, curr_zone, curr_sens);
				else Singleton->CrossResult->PutDataOnChartX(this, curr_zone, curr_sens);
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
void TFViewZoneD::ViewEtalonCheck(int _startZone, int _finalZone)
{
	//TPr::pr("ViewEtalonCheck");
	AnsiString Str = "";
	int IDTube = ini->ReadInteger("OtherSettings","SOPCheckID",0);
	if(IDTube)
	{
		//собираем данные по дефектам эталона
		QEtalons->Connection = DBS->Connection();
		QEtalons->Close();
		QEtalons->Active = false;
		QEtalons->SQL->Clear();
		AnsiString SQL;
		SQL = "SELECT * FROM EtalonTubeDefects ";
		AnsiString WhereStr = "where EtalonTubeDefects.IDTube='" + IntToStr(IDTube) + "'";
		SQL += WhereStr;
		SQL += " order by EtalonTubeDefects.ID";
		QEtalons->SQL->Add(SQL);
		QEtalons->ExecSQL();
		QEtalons->Active = true;

		QEtalons->First();
		int countDefects = 0;
		int numOfDefects = QEtalons->RecordCount;
		int selectedDefect = cbEtalonTubeDefects->ItemIndex;
		cbEtalonTubeDefects->Clear();
		cbEtalonTubeDefects->Visible = true;
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
		while (!QEtalons->Eof)
		{
			int Module = QEtalons->FieldByName("Module")->AsInteger;
			//добавляем запись в КомбоБокс
			countDefects++;
			Str = IntToStr(countDefects) + "/" + IntToStr(numOfDefects);
			if (Module == 1)
				Str += " поперечный ";
			else if (Module == 2)
				Str += " продольный ";
			else if (Module == 3)
				Str += " толщиномер ";
			Str += IntToStr(QEtalons->FieldByName("DefPoint")->AsInteger
				- QEtalons->FieldByName("DefRadius")->AsInteger) + "-";
            Str += IntToStr(QEtalons->FieldByName("DefPoint")->AsInteger
				+ QEtalons->FieldByName("DefRadius")->AsInteger);
			cbEtalonTubeDefects->AddItem(Str, NULL);
			//проверяем наш ли это модуль
			if((!IsLinear && (Module == 1)) || (IsLinear && (Module == 2)))
			{
				//если участок поиска пересекается с отображаемым участком,
				//то выясняем его точки начала и конца
				startX = QEtalons->FieldByName("DefPoint")->AsInteger
					- QEtalons->FieldByName("DefRadius")->AsInteger;
				finalX = QEtalons->FieldByName("DefPoint")->AsInteger
					+ QEtalons->FieldByName("DefRadius")->AsInteger;
				bool Include = true;
				if(startX < (start + zone_size))
				{
					if(startX > start)
					{
						startMeas = CalcMeasFromRange((int)(startX - start));
					}
					else
						startMeas = 0;
				}
				else
					Include = false;
				if(Include && (finalX > start))
				{
					if(finalX < (start + zone_size))
					{
						finalMeas = CalcMeasFromRange((int)(finalX - start));
					}
					else
						finalMeas = Chart1->Series[0]->Count();
				}
				else
					Include = false;
				//рисуем отрезки поиска
				if (Include)
				{
					//Рисуем верхнюю границу
					if(QEtalons->FieldByName("TopBorderPercent")->AsInteger)
					{
						if(IsLinear)
							param = "Linear";
						else
							param = "Cross";
						if(QEtalons->FieldByName("isTopBrackBorder")->AsBoolean)
							param += "Border1";
						else
							param += "Border2";
						TopBorder = ini->ReadFloat(sect,param,0)
							* QEtalons->FieldByName("TopBorderPercent")->AsInteger / 100;
						SetBrackBorder(startMeas,finalMeas,TopBorder,clFuchsia);
						//Если включен внутренний фильтр продольника
						if(IsLinear && SystemConst::isLinearDigitalFilterIn)
						{
							if(QEtalons->FieldByName("isTopBrackBorder")->AsBoolean)
								param = "LinearBorder1Inner";
							else
								param = "LinearBorder2Inner";
							TopBorder = ini->ReadFloat(sect,param,0)
								* QEtalons->FieldByName("TopBorderPercent")->AsInteger / -100;
							SetBrackBorder(startMeas,finalMeas,TopBorder,clFuchsia);
						}
					}
					//Рисуем нижнюю границу
					if(QEtalons->FieldByName("BottomBorderPercent")->AsInteger)
					{
						if(IsLinear)
							param = "Linear";
						else
							param = "Cross";
						if(QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean)
							param += "Border1";
						else
							param += "Border2";
						BottomBorder = ini->ReadFloat(sect,param,0)
							* QEtalons->FieldByName("BottomBorderPercent")->AsInteger / 100;
						SetBrackBorder(startMeas,finalMeas,BottomBorder,clBlue);
						//Если включен внутренний фильтр продольника
						if(IsLinear && SystemConst::isLinearDigitalFilterIn)
						{
							if(QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean)
								param = "LinearBorder1Inner";
							else
								param = "LinearBorder2Inner";
							BottomBorder = ini->ReadFloat(sect,param,0)
								* QEtalons->FieldByName("BottomBorderPercent")->AsInteger / -100;
							SetBrackBorder(startMeas,finalMeas,BottomBorder,clBlue);
						}
					}
				}
				startMeas = 0;
				finalMeas = 0;
				TopBorder = 0;
				BottomBorder = 0;
			}
			QEtalons->Next();
		}
		cbEtalonTubeDefects->ItemIndex = selectedDefect;
	}
}
//---------------------------------------------------------------------------
int TFViewZoneD::CalcMeasFromRange(int _x)
{
	double Measure;
	int CurrentZoneMeasures;
	int StartOfZoneMeas = 0;
	int LastMeas = Chart1->Series[0]->Count();
	int zone_length = Singleton->CrossResult->GetZoneLength();
	//Определяем конкретную зону (из 3)
	if ((_x > zone_length) && (DevidMeas1 > 0))
	{
		if ((_x > (2*zone_length)) && (DevidMeas2 > 0))
		{
			StartOfZoneMeas = DevidMeas2;
			_x -= (2*zone_length);
			CurrentZoneMeasures = LastMeas - DevidMeas2;
		}
		else
		{
			StartOfZoneMeas = DevidMeas1;
			_x -= zone_length;
			if (DevidMeas2 > DevidMeas1)
				CurrentZoneMeasures = DevidMeas2 - DevidMeas1;
			else CurrentZoneMeasures = LastMeas - DevidMeas1;
		}
	}
	else
	{
		CurrentZoneMeasures = DevidMeas1;
	}
	//окончательный расчет нужного измерения
	double x = _x;
	Measure = StartOfZoneMeas + x / zone_length * CurrentZoneMeasures;
	return (int)Measure;
}
//---------------------------------------------------------------------------
void TFViewZoneD::SetBrackBorder(int _startMeas,int _finalMeas,double _Border,TColor _color)
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

void __fastcall TFViewZoneD::cbEtalonTubeDefectsKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if (Key != 27)
	{
		cbEtalonTubeDefects->ItemIndex = curr_defect;
		KeyUpFlag = true;
	}
}
//---------------------------------------------------------------------------

