// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uFRViewZone.h"
#include <SysUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma link "Series"
#pragma link "ArrowCha"
#pragma link "cspin"
#pragma link "ArrowCha"
#pragma link "cspin"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TFRViewZone::TFRViewZone(TComponent* Owner, int _sensor,
	bool _gain_visible,bool _check_visible) : TFrame(Owner), rCursor(Cursor)
{
	AnsiString a = "Датчик ";
	a += _sensor + 1;
	Chart1->Title->Caption = a;
	// Edit1->Tag=1;
	Edit1->Visible = _gain_visible;
	Edit1->Tag = _sensor;
	OnChangeGain = NULL;
	OnCalibrate = NULL;
	sensor = _sensor;
	rCursor.SetMax(Chart1->LeftAxis->Maximum);
	step=1;
	CheckBox1->Visible=_check_visible;
}

// ---------------------------------------------------------------------------
void __fastcall TFRViewZone::FrameResize(TObject *Sender)
{
	Edit1->Top = 3;
	CSpinButton1->Top = 3;
	CSpinButton1->Left = ClientWidth - CSpinButton1->Width - 1;
	Edit1->Left = CSpinButton1->Left - Edit1->Width;
	CheckBox1->Left=Edit1->Left-CheckBox1->Width;
}

// ---------------------------------------------------------------------------
void TFRViewZone::Clear(void)
{
	for (int i = 0; i < Chart1->SeriesCount(); i++)
		Chart1->Series[i]->Clear();
}

void TFRViewZone::SetBorder(TLineSeries* _series, double _level, TColor _color)
{
	_series->Clear();
	_series->AddXY(0, _level, "", _color);
	_series->AddXY(BarOut->MaxXValue(), _level, "", _color);
}

void TFRViewZone::SetGain(double _gain)
{
	Edit1->Text = FloatToStr(_gain);
}

void TFRViewZone::TrigerVisible(void)
{
	Edit1->Visible = !Edit1->Visible;
}

void TFRViewZone::SetInner(bool _v)
{
	if (_v)
		Chart1->LeftAxis->Minimum = -100;
	else
		Chart1->LeftAxis->Minimum = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TFRViewZone::Edit1KeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	TEdit* p = (TEdit*)Sender;
	if (Key == VK_RETURN)
	{
		double ret = (double)StrToFloatDef(p->Text, -1);
		if (ret < 0)
		{
			p->Font->Color = clHotLight;
			p->Font->Style = TFontStyles() << fsBold;
			return;
		}
		p->Font->Color = clWindowText;
		p->Font->Style = TFontStyles();
		if (OnChangeGain != NULL)
			OnChangeGain(sensor, (double)StrToFloatDef(p->Text, -1));
	}
	else
	{
		p->Font->Color = clHotLight;
		p->Font->Style = TFontStyles() << fsBold;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::Edit1Exit(TObject *Sender)
{
	TEdit* p = (TEdit*)Sender;
	double ret = (double)StrToFloatDef(p->Text, -1);
	if (ret < 0)
	{
		p->SetFocus();
		return;
	}
	p->Font->Color = clWindowText;
	p->Font->Style = TFontStyles();
	if (OnChangeGain != NULL)
		OnChangeGain(sensor, (double)StrToFloatDef(p->Text, -1));
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::Chart1ClickSeries(TCustomChart *Sender,
	TChartSeries *Series, int ValueIndex, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	// if (Button == mbLeft)
	// rCursor.Set(BarOut->XScreenToValue(X));
	// else
	if (Button == mbRight)
	{
		if (OnCalibrate == NULL)
			return;
		N1->Caption = AnsiString("Калибровать д") + (sensor + 1);
		PopupMenu1->Popup(Left + X, Top + Y);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::Chart1ClickBackground(TCustomChart *Sender,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	// if (Button == mbLeft)
	// rCursor.Set(BarOut->XScreenToValue(X));
	// else
	if (Button == mbRight)
	{
		if (OnCalibrate == NULL)
			return;
		N1->Caption = AnsiString("Калибровать по датчику ") + (sensor + 1);
		PopupMenu1->Popup(Left + X, Top + Y);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::N1Click(TObject *Sender)
{
	if (OnCalibrate != NULL)
		OnCalibrate(this);
}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
TFRViewZone::RCursor::RCursor(TArrowSeries* _series)
{
	series = _series;
	max = 100;
}

// ---------------------------------------------------------------------------
void TFRViewZone::RCursor::SetMax(int _max)
{
	max = _max;
}

// ---------------------------------------------------------------------------
void TFRViewZone::RCursor::Set(int _val)
{
	series->Clear();
	series->AddArrow(_val, 0, _val, max);
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::CSpinButton1UpClick(TObject *Sender)
{
	TEdit* p=Edit1;
	double ret = (double)StrToFloatDef(p->Text, -1);
	if (ret < 0)
		return;
	ret+=step;
	p->Text=FloatToStr(ret);
	p->Font->Color = clWindowText;
	p->Font->Style = TFontStyles();
	if (OnChangeGain != NULL)
		OnChangeGain(sensor, ret);
}
// ---------------------------------------------------------------------------

void __fastcall TFRViewZone::CSpinButton1DownClick(TObject *Sender)
{
	TEdit* p=Edit1;
	double ret = (double)StrToFloatDef(p->Text, -1);
	if (ret < 0)
		return;
	ret-=step;
	if(ret<0)
		return;
	p->Text=FloatToStr(ret);
	p->Font->Color = clWindowText;
	p->Font->Style = TFontStyles();
	if (OnChangeGain != NULL)
		OnChangeGain(sensor, ret);
}
// ---------------------------------------------------------------------------
