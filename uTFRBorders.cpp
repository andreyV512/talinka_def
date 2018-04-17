// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFRBorders.h"
#include "Protocol.h"
#include "uFunctions.h"
#include "Singleton.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRBorders *FRBorders;

// ---------------------------------------------------------------------------
__fastcall TFRBorders::TFRBorders(TComponent* Owner) : TFrame(Owner)
{
	SetInner(false);
	inner = false;
	OnChange = NULL;
	tp="";
}

// ---------------------------------------------------------------------------
void TFRBorders::SetInner(bool _v)
{
	EBorder11->Visible = _v;
	EBorder21->Visible = _v;
	LBorder1->Visible = _v;
	LBorder2->Visible = _v;
}

void TFRBorders::Init(AnsiString _tp, TIniFile* _ini)
{
	tp = _tp;
	ini = _ini;
	LoadSettings();
}
void TFRBorders::LoadSettings(void)
{
	section = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	if(tp.Length()==0)
		return;
	inner = ini->ReadBool("Filters","isFilterPrIn",0);
	if (tp == "Cross")
	{
		SetInner(false);
		EBorder10->Text = ini->ReadFloat(section, "CrossBorder1", 0.0);
		EBorder20->Text = ini->ReadFloat(section, "CrossBorder2", 0.0);
	}
	else if (tp == "Thick")
	{
		SetInner(false);
		EBorder10->Text = ini->ReadFloat(section, "ThicknessBorder1", 0.0);
		EBorder20->Text = ini->ReadFloat(section, "ThicknessBorder2", 0.0);
	}
	else if (tp == "Line")
	{
		SetInner(inner);
		EBorder10->Text = ini->ReadFloat(section, "LinearBorder1", 0.0);
		EBorder20->Text = ini->ReadFloat(section, "LinearBorder2", 0.0);
		if (inner)
		{
			EBorder11->Text = ini->ReadFloat(section, "LinearBorder1Inner", 0.0);
			EBorder21->Text = ini->ReadFloat(section, "LinearBorder2Inner", 0.0);
		}
	}
	else
	{
		AnsiString a = "TFRBorders::Init: Не верный тип: ";
		a += tp;
		FATAL(a);
	}
}
void TFRBorders::Save(void)
{
	if (tp == "Cross")
	{
		ini->WriteString(section, "CrossBorder1", EBorder10->Text);
		ini->WriteString(section, "CrossBorder2", EBorder20->Text);
	}
	else if (tp == "Thick")
	{
		ini->WriteString(section, "ThicknessBorder1", EBorder10->Text);
		ini->WriteString(section, "ThicknessBorder2", EBorder20->Text);
	}
	else if (tp == "Line")
	{
		ini->WriteString(section, "LinearBorder1", EBorder10->Text);
		ini->WriteString(section, "LinearBorder2", EBorder20->Text);
		if (inner)
		{
			ini->WriteString(section, "LinearBorder1Inner", EBorder11->Text);
			ini->WriteString(section, "LinearBorder2Inner", EBorder21->Text);
		}
	}
	if (OnChange != NULL)
		OnChange();
	//wws: 20150820
	Singleton->FromFile=true; //чтобы нельзя было подстроить для сверки
}

void __fastcall TFRBorders::EBorder10Exit(TObject *Sender)
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
	Save();
}
// ---------------------------------------------------------------------------


void __fastcall TFRBorders::EBorder10KeyDown(TObject *Sender, WORD &Key,
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
		Save();
	}
	else
	{
		p->Font->Color = clHotLight;
		p->Font->Style = TFontStyles() << fsBold;
	}
}
// ---------------------------------------------------------------------------
