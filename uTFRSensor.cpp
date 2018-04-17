// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFRSensor.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFRSensor *FRSensor;

// ---------------------------------------------------------------------------
__fastcall TFRSensor::TFRSensor(TComponent* Owner, TIniFile* _ini, AnsiString _tp,
	int _nn) : TFrame(Owner)
{
	ini = _ini;
	tp = _tp;
	LoadSettings();
	nn = _nn;
	LNN->Caption = nn;
	SetEnabledR(false);
	LoadSettings();
}

// ---------------------------------------------------------------------------
void TFRSensor::SetEnabledR(bool _v)
{
	SEChannel->Enabled = _v;
	CBRange->Enabled = _v;
	CBMode->Enabled = _v;
}

// ---------------------------------------------------------------------------
void TFRSensor::LoadSettings(void)
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	AnsiString diameter = ini->ReadInteger(sect, "Diameter", 1);
	AnsiString postfix;
	if (tp == "Cross")
		postfix = "PP";
	else
		postfix = "PR";
	AnsiString section;
	AnsiString name;

	section = "Range";
	section += "_";
	section += postfix;
	section += "_";
	section += diameter;
	name = "Range";
	name += nn;
	CBRange->ItemIndex = ini->ReadInteger(section, name, 1);

	section = "CollectedMode";
	section += "_";
	section += postfix;
	section += "_";
	section += diameter;
	name = "Mode";
	name += nn;
	CBMode->ItemIndex = ini->ReadInteger(section, name, 1);

	section = postfix;
	name = "Number";
	name += nn;
	SEChannel->Value = ini->ReadInteger(postfix, name, 1);

}
void TFRSensor::SaveSettings(void)
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	AnsiString diameter = ini->ReadInteger(sect, "Diameter", 1);
	AnsiString postfix;
	if (tp == "Cross")
		postfix = "PP";
	else
		postfix = "PR";
	AnsiString section;
	AnsiString name;

	section = "Range";
	section += "_";
	section += postfix;
	section += "_";
	section += diameter;
	name = "Range";
	name += nn;
	ini->WriteInteger(section, name, CBRange->ItemIndex);

	section = "CollectedMode";
	section += "_";
	section += postfix;
	section += "_";
	section += diameter;
	name = "Mode";
	name += nn;
	ini->WriteInteger(section, name, CBMode->ItemIndex);

	section = postfix;
	name = "Number";
	name += nn;
	ini->WriteInteger(postfix, name, SEChannel->Value);
}
int TFRSensor::GetRange(void)
{
	return(CBRange->ItemIndex);
}
int TFRSensor::GetChannel(void)
{
	return(SEChannel->Value);
}
int TFRSensor::GetMode(void)
{
	return(CBMode->ItemIndex);
}



