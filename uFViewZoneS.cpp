// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFViewZoneS.h"
#include "uFunctions.h"
#include <Series.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "Series"
#pragma link "ArrowCha"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFViewZoneS::TFViewZoneS(TComponent* Owner, TIniFile* _ini,
	AnsiString _title) : TForm(Owner)
{
	ini = _ini;
	title = _title;
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneS::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		((TForm*)Sender)->Close();
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneS::FormCreate(TObject *Sender)
{
	LoadFormPos(this, ini);
	SBSource->Down = ini->ReadBool("OtherSettings", "TFViewZoneSSource", true);
	SBMedian->Down = ini->ReadBool("OtherSettings", "TFViewZoneSMedian", true);
	SBFilterIn->Down = ini->ReadBool("OtherSettings",
		"TFViewZoneSFilterIn", true);
	SBFilterOut->Down = ini->ReadBool("OtherSettings",
		"TFViewZoneSFilterOut", true);
	Chart1->Series[0]->Visible = SBSource->Down;
	Chart1->Series[1]->Visible = SBMedian->Down;
	Chart1->Series[2]->Visible = SBFilterOut->Down;
	Chart1->Series[3]->Visible = SBFilterIn->Down;
	Out10->Visible=SBFilterOut->Down;
	Out11->Visible=SBFilterOut->Down;
	Out20->Visible=SBFilterOut->Down;
	Out21->Visible=SBFilterOut->Down;
	In10->Visible=SBFilterIn->Down;
	In11->Visible=SBFilterIn->Down;
	In20->Visible=SBFilterIn->Down;
	In21->Visible=SBFilterIn->Down;
	SetMetric(0, 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFViewZoneS::FormDestroy(TObject *Sender)
{
	ini->WriteBool("OtherSettings", "TFViewZoneSSource", SBSource->Down);
	ini->WriteBool("OtherSettings", "TFViewZoneSMedian", SBMedian->Down);
	ini->WriteBool("OtherSettings", "TFViewZoneSFilterOut", SBFilterOut->Down);
	ini->WriteBool("OtherSettings", "TFViewZoneSFilterIn", SBFilterIn->Down);
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void TFViewZoneS::SetCaption(AnsiString _cap)
{
	Caption = title + _cap;
}

void TFViewZoneS::SetBorder(TLineSeries* _series, double _level, TColor _color)
{
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
	_series->AddXY(Series1->MaxXValue(), _level, "", _color);
}

void TFViewZoneS::SetMetric(double _start, double _zone_size)
{
	start = _start;
	zone_size = _zone_size;
	if (start < 0)
		start = 0;
	if (zone_size < 0)
		zone_size = 0;
}

void __fastcall TFViewZoneS::Chart1ClickBackground(TCustomChart *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	double x, y;
	Chart1->Series[0]->GetCursorValues(x, y);
	if (x < 0)
		x = 0;
	Chart1->Series[0]->Count();
	double Range = start + x * zone_size / Chart1->Series[0]->Count();
	AnsiString a = "Расстояние: ~";
	a += (int)Range;
	a += " мм";
	StatusBar1->Panels->Items[0]->Text = a;
}

// ---------------------------------------------------------------------------
void TFViewZoneS::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
};

void __fastcall TFViewZoneS::SBSourceClick(TObject *Sender)
{
	Chart1->Series[0]->Visible = ((TSpeedButton*)Sender)->Down;
}
// ---------------------------------------------------------------------------

void __fastcall TFViewZoneS::SBMedianClick(TObject *Sender)
{
	Chart1->Series[1]->Visible = ((TSpeedButton*)Sender)->Down;
}
// ---------------------------------------------------------------------------

void __fastcall TFViewZoneS::SBFilterOutClick(TObject *Sender)
{
	bool v=((TSpeedButton*)Sender)->Down;
	Chart1->Series[2]->Visible = v;
	Out10->Visible=v;
	Out11->Visible=v;
	Out20->Visible=v;
	Out21->Visible=v;
}
// ---------------------------------------------------------------------------

void __fastcall TFViewZoneS::SBFilterInClick(TObject *Sender)
{
	bool v=((TSpeedButton*)Sender)->Down;
	Chart1->Series[3]->Visible = v;
	In10->Visible=v;
	In11->Visible=v;
	In20->Visible=v;
	In21->Visible=v;
}
// ---------------------------------------------------------------------------
void TFViewZoneS::BorderVisible(AnsiString _mark,bool _v)
{
}

