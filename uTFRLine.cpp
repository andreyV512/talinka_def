//---------------------------------------------------------------------------

#include <vcl.h>
#include "Global.h"
#pragma hdrstop

#include "uTFRLine.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "uTFRBorders"
#pragma link "uTFRDeads"
#pragma resource "*.dfm"
TFRLine *FRLine;
//---------------------------------------------------------------------------
__fastcall TFRLine::TFRLine(TComponent* Owner)
	: TFrame(Owner)
{
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
		Chart1->Series[i]->OnClick=SeriesClick;
		Chart1->Series[i]->OnDblClick=SeriesDblClick;
	}
	mark_zone=0;
	mark_sensor=0;
	mark_is=false;
	mark_color=clWhite;
	OnClick=NULL;
	OnDblClick=NULL;
	OnChange=NULL;
	FRBordersLine->OnChange=OnChangeChild;
	FRDeads1->OnChange=OnChangeChild;
	Chart1->LeftAxis->Maximum=Chart1->SeriesCount();
	SetDefects(0);
}
//---------------------------------------------------------------------------
void TFRLine::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
	MarkerOff();
	SetDefects(0);
}
void TFRLine::Init(TIniFile* _ini)
{
	FRBordersLine->Init("Line",_ini);
	FRDeads1->Init("Line",_ini);
	ini = _ini;
	LoadSettings0();
}
void TFRLine::LoadSettings(void)
{
	LoadSettings0();
	FRBordersLine->LoadSettings();
}
void TFRLine::LoadSettings0(void)
{
	cbLinear->Checked=ini->ReadBool("Default", "IsLinear", true);
}
void TFRLine::SetEnabledR(bool _v)
{
	FRBordersLine->Enabled=_v;
	FRDeads1->Enabled=_v;
}
void __fastcall TFRLine::cbLinearClick(TObject *Sender)
{
	ini->WriteBool("Default", "IsLinear", cbLinear->Checked);
	if(OnChange!=NULL)
		OnChange();
}
//---------------------------------------------------------------------------
void TFRLine::MarkerSet(int _zone,int _sensor)
{
	MarkerOff();
	if(_sensor<0||_sensor>=Chart1->SeriesCount())
		return;
	if(_zone<0||_zone>=Chart1->Series[_sensor]->Count())
		return;
	mark_sensor=_sensor;
	mark_zone=_zone;
	mark_color=Chart1->Series[_sensor]->ValueColor[_zone];
	mark_is=true;
	Chart1->Series[_sensor]->ValueColor[_zone]=clNavy;
}
void TFRLine::MarkerOff(void)
{
	if(!mark_is)
		return;
	mark_is=false;
	if(Chart1->Series[mark_sensor]->Count() > mark_zone)
		Chart1->Series[mark_sensor]->ValueColor[mark_zone]=mark_color;
}
//---------------------------------------------------------------------------
void __fastcall TFRLine::SeriesDblClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if(OnDblClick==NULL)
	{
		MarkerOff();
		return;
	}
	int zone=ValueIndex;
	int sensor;
	for(int i=0;i<Chart1->SeriesCount();i++)
	{
		if(Chart1->Series[i]==Sender)
		{
			sensor=i;
			break;
		}
	}
	bool IsCtrl=Shift.Contains(ssCtrl);
	MarkerSet(zone,sensor);
	OnDblClick(zone,sensor,IsCtrl);
}
//---------------------------------------------------------------------------

void __fastcall TFRLine::SeriesClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if(OnClick==NULL)
	{
		MarkerOff();
		return;
	}
	int zone=ValueIndex;
	int sensor;
	for(int i=0;i<Chart1->SeriesCount();i++)
	{
		if(Chart1->Series[i]==Sender)
		{
			sensor=i;
			break;
		}
	}
	bool IsCtrl=Shift.Contains(ssCtrl);
	MarkerSet(zone,sensor);
	OnClick(zone,sensor,IsCtrl);
}
//---------------------------------------------------------------------------
void TFRLine::OnChangeChild(void)
{
	if(OnChange!=NULL)
		OnChange();
}
void __fastcall TFRLine::Panel1Resize(TObject *Sender)
{
	FRDeads1->Left=Panel1->ClientWidth-FRDeads1->Width-4;
}
//---------------------------------------------------------------------------
void TFRLine::SetDefects(int _val)
{
	if(_val==0)
		Panel2->Caption="";
	else
		Panel2->Caption=_val;
}
//---------------------------------------------------------------------------

