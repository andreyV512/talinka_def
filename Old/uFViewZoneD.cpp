// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFViewZoneD.h"
#include "uFunctions.h"
#include <Series.hpp>
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
	Chart1->Series[0]->Count();
	double Range = start + x * zone_size / Chart1->Series[0]->Count();
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
};
// ---------------------------------------------------------------------------
void TFViewZoneD::SetBorder(TLineSeries* _series, double _level, TColor _color)
{
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
	_series->AddXY(BarOut->MaxXValue(), _level, "", _color);
}

