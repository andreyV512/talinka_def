// ---------------------------------------------------------------------------

#include <vcl.h>
#include "Global.h"
#pragma hdrstop

#include "uTFRThick.h"
#include "math.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "uTFRBorders"
#pragma resource "*.dfm"
TFRThick *FRThick;

// ---------------------------------------------------------------------------
__fastcall TFRThick::TFRThick(TComponent* Owner) : TFrame(Owner)
{
	OnChange = NULL;
	Chart1->BottomAxis->Maximum = Globals::max_zones;
	Chart1->AllowPanning = TPanningMode::pmNone;
	FRBordersThick->OnChange = OnChangeChild;
	SetDefects(0);
}

// ---------------------------------------------------------------------------
void TFRThick::OnChangeChild(void)
{
	if (OnChange != NULL)
		OnChange();
}

// ---------------------------------------------------------------------------
void TFRThick::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
	SetDefects(0);
	SetMinThick(0.0);
}

// ---------------------------------------------------------------------------
void TFRThick::Init(TIniFile* _ini)
{
	FRBordersThick->Init("Thick", _ini);
	ini = _ini;
	LoadSettings0();
	Chart1->LeftAxis->Minimum = 0;
	Chart1->LeftAxis->Maximum = 10;
}

// ---------------------------------------------------------------------------
void TFRThick::LoadSettings0(void)
{
}

// ---------------------------------------------------------------------------
void TFRThick::LoadSettings(void)
{
	LoadSettings0();
	FRBordersThick->LoadSettings();
}

// ---------------------------------------------------------------------------
void TFRThick::SetEnabledR(bool _v)
{
	FRBordersThick->Enabled = _v;
}

// ---------------------------------------------------------------------------
void __fastcall TFRThick::FrameResize(TObject *Sender)
{
	int space = 4;
	Chart1->Left = 0;
	Chart1->Width = ClientWidth;
	Chart1->Top = FRBordersThick->Top + FRBordersThick->Height + space;
	Chart1->Height = ClientHeight - Chart1->Top;
}

// ---------------------------------------------------------------------------
void TFRThick::SetDefects(int _val)
{
	if (_val == 0)
		Panel1->Caption = "";
	else
		Panel1->Caption = _val;
}
// ---------------------------------------------------------------------------
void TFRThick::SetMinThick(double _val)
{
	if (_val == 0.0)
		Panel2->Caption = "";
	else
	{
		AnsiString ss;
		double val = floor(_val * 100 + 0.5) / 100;
		ss.printf("%0.2f", val);
		Panel2->Caption = ss;
	}
}
// ---------------------------------------------------------------------------
