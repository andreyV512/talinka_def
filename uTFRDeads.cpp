// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFRDeads.h"
#include "uFunctions.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRDeads *FRDeads;

// ---------------------------------------------------------------------------
__fastcall TFRDeads::TFRDeads(TComponent* Owner) : TFrame(Owner)
{
	OnChange = NULL;
	tpp="";
}

// ---------------------------------------------------------------------------
void __fastcall TFRDeads::EStartExit(TObject *Sender)
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
void __fastcall TFRDeads::EStartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
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
void TFRDeads::Init(AnsiString _tp, TIniFile* _ini)
{
	tpp = _tp;
	ini = _ini;
	LoadSettings();
}

// ---------------------------------------------------------------------------
void TFRDeads::LoadSettings(void)
{
	section = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	if (tpp.Length() == 0)
		return;
	if (tpp == "Cross")
	{
		EStart->Text = IntToStr(ini->ReadInteger(section, "CrossDeadZoneStart", 0));
		EFinish->Text = IntToStr(ini->ReadInteger(section, "CrossDeadZoneFinish", 0));
	}
	else if (tpp == "Line")
	{
		EStart->Text = IntToStr(ini->ReadInteger(section, "LinearDeadZoneStart", 0));
		EFinish->Text =
			IntToStr(ini->ReadInteger(section, "LinearDeadZoneFinish", 0));
	}
	else
	{
		AnsiString a = "TFRDeads::Init: Не верный тип: ";
		a += tpp;
		FATAL(a);
	}
}

// ---------------------------------------------------------------------------
void TFRDeads::Save(void)
{
	if (tpp == "Cross")
	{
		ini->WriteString(section, "CrossDeadZoneStart", EStart->Text);
		ini->WriteString(section, "CrossDeadZoneFinish", EFinish->Text);
	}
	else if (tpp == "Line")
	{
		ini->WriteString(section, "LinearDeadZoneStart", EStart->Text);
		ini->WriteString(section, "LinearDeadZoneFinish", EFinish->Text);
	}
	if (OnChange != NULL)
		OnChange();
}
