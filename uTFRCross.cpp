// ---------------------------------------------------------------------------

#include <vcl.h>
#include "Global.h"
#pragma hdrstop

#include "uTFRCross.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "uTFRBorders"
#pragma link "uTFRDeads"
#pragma resource "*.dfm"
TFRCross *FRCross;

// ---------------------------------------------------------------------------
__fastcall TFRCross::TFRCross(TComponent* Owner) : TFrame(Owner)
{
	FRBordersCross->OnChange = OnChangeChild;
	FRDeads1->OnChange = OnChangeChild;
	Chart1->BottomAxis->Maximum = Globals::max_zones;
	Chart1->AllowPanning = TPanningMode::pmNone;
	for (int i = 0; i < Chart1->SeriesCount(); i++)
	{
		Chart1->Series[i]->ColorEachPoint = true;
		Chart1->Series[i]->Marks->Visible = false;
		((TBarSeries*) Chart1->Series[i])->BarWidthPercent = 100;
		((TBarSeries*) Chart1->Series[i])->MultiBar = mbStacked;
		((TBarSeries*) Chart1->Series[i])->SideMargins = false;
		((TBarSeries*) Chart1->Series[i])->OffsetPercent = 50;
		Chart1->Series[i]->OnClick = SeriesClick;
		Chart1->Series[i]->OnDblClick = SeriesDblClick;
	}
	mark_zone = 0;
	mark_sensor = 0;
	mark_is = false;
	mark_color = clWhite;
	OnClick = NULL;
	OnDblClick = NULL;
	OnChange = NULL;
	SetDefects(0);
}

// ---------------------------------------------------------------------------
void TFRCross::OnChangeChild(void)
{
	if (OnChange != NULL)
		OnChange();
}

void TFRCross::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
	MarkerOff();
	SetDefects(0);
}

void TFRCross::Init(TIniFile* _ini)
{
	FRBordersCross->Init("Cross", _ini);
	FRDeads1->Init("Cross", _ini);
	ini = _ini;
}

void TFRCross::LoadSettings(void)
{
	FRBordersCross->LoadSettings();
	FRDeads1->LoadSettings();
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	int lsensors_a = ini->ReadInteger(sect, "CrossSensors", 100);
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Visible = i < lsensors_a;
	Chart1->LeftAxis->Minimum = 0;
	Chart1->LeftAxis->Maximum = lsensors_a;
}

void TFRCross::SetEnabledR(bool _v)
{
	FRBordersCross->Enabled = _v;
	FRDeads1->Enabled = _v;
}

void TFRCross::MarkerSet(int _zone, int _sensor)
{
	MarkerOff();
	if (_sensor < 0 || _sensor >= Chart1->SeriesCount())
		return;
	if (_zone < 0 || _zone >= Chart1->Series[_sensor]->Count())
		return;
	mark_sensor = _sensor;
	mark_zone = _zone;
	mark_color = Chart1->Series[_sensor]->ValueColor[_zone];
	mark_is = true;
	Chart1->Series[_sensor]->ValueColor[_zone] = clNavy;
}

void TFRCross::MarkerOff(void)
{
	if (!mark_is)
		return;
	mark_is = false;
	if (Chart1->Series[mark_sensor]->Count() > mark_zone)
		Chart1->Series[mark_sensor]->ValueColor[mark_zone] = mark_color;
}

// ---------------------------------------------------------------------------
void __fastcall TFRCross::SeriesDblClick(TChartSeries *Sender, int ValueIndex,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (OnDblClick == NULL)
	{
		MarkerOff();
		return;
	}
	int zone = ValueIndex;
	int sensor;
	for (int i = 0; i < Chart1->SeriesCount(); i++)
	{
		if (Chart1->Series[i] == Sender)
		{
			sensor = i;
			break;
		}
	}
	bool IsCtrl = Shift.Contains(ssCtrl);
	MarkerSet(zone, sensor);
	OnDblClick(zone, sensor, IsCtrl);
}
// ---------------------------------------------------------------------------

void __fastcall TFRCross::SeriesClick(TChartSeries *Sender, int ValueIndex,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (OnClick == NULL)
	{
		MarkerOff();
		return;
	}
	int zone = ValueIndex;
	int sensor;
	for (int i = 0; i < Chart1->SeriesCount(); i++)
	{
		if (Chart1->Series[i] == Sender)
		{
			sensor = i;
			break;
		}
	}
	bool IsCtrl = Shift.Contains(ssCtrl);
	MarkerSet(zone, sensor);
	OnClick(zone, sensor, IsCtrl);
}
// ---------------------------------------------------------------------------

void __fastcall TFRCross::Panel1Resize(TObject *Sender)
{
	FRDeads1->Left = Panel1->ClientWidth - FRDeads1->Width - 4;
}

// ---------------------------------------------------------------------------
void TFRCross::SetDefects(int _val)
{
	if (_val == 0)
		Panel2->Caption = "";
	else
		Panel2->Caption = _val;
}
