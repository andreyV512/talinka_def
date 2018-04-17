// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ColorSettings.h"
#include "Global.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormColor *FormColor;

// ---------------------------------------------------------------------------
__fastcall TFormColor::TFormColor(TComponent* Owner) : TForm(Owner)
{
}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::FormCreate(TObject *Sender)
{
	ini = new TIniFile(Globals::IniFileName);
	PanelBrack->Color = (TColor)ini->ReadInteger(L"Color", L"Brack", 255);
	PanelSecondClass->Color = (TColor)ini->ReadInteger(L"Color", L"SecondClass",16711680);
	PanelValid->Color = (TColor)ini->ReadInteger(L"Color", L"Valid", 41984);

	ActiveIn->Color = (TColor)ini->ReadInteger(L"Color", L"ActiveIn", 1503543);
	ActiveOut->Color = (TColor)ini->ReadInteger(L"Color", L"ActiveOut", 465649);

	NotActiveOut->Color = (TColor)ini->ReadInteger(L"Color", L"NotActiveOut",	8421504);
	NotInActive->Color = (TColor)ini->ReadInteger(L"Color", L"NotActiveIn", 12425481);

	pSelectZone->Color=(TColor)ini->ReadInteger(L"Color", L"SelectedZone",0xFF0000 );
	pDeadZone->Color=(TColor)ini->ReadInteger(L"Color", L"DeadZone", 0x999999 );
	sgD->Color = SolidGroup_D =  (TColor)ini->ReadInteger(L"Color", L"sgD",65535);
	sgK->Color = SolidGroup_K = (TColor)ini->ReadInteger(L"Color", L"sgK",65280);
	sgE->Color = SolidGroup_E =  (TColor)ini->ReadInteger(L"Color", L"sgE",16711680);

}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::PanelValidClick(TObject *Sender)
{
	TPanel* Pan = dynamic_cast<TPanel*>(Sender);
	if (ColorDialog->Execute())
		Pan->Color = ColorDialog->Color;
}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::BDefaultClick(TObject *Sender)
{
	PanelBrack->Color = (TColor)255;
	PanelSecondClass->Color = (TColor)65535;
	PanelValid->Color = (TColor)41984;

	ActiveIn->Color = (TColor)1503543;
	ActiveOut->Color = (TColor)465649;
	NotActiveOut->Color = (TColor)8421504;
	NotInActive->Color = (TColor)12425481;

	pSelectZone->Color=(TColor)0xFF0000;
	pDeadZone->Color=(TColor)0x999999;

	sgD->Color = (TColor)65535;
	sgK->Color = (TColor)65280;
	sgE->Color = (TColor)16711680;
}

// ---------------------------------------------------------------------------
void __fastcall TFormColor::BCloseClick(TObject *Sender)
{
	FormColor->Close();
}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::BSaveClick(TObject *Sender)
{
	ini->WriteInteger(L"Color", L"Brack", PanelBrack->Color);
	ini->WriteInteger(L"Color", L"Valid", PanelValid->Color);
	ini->WriteInteger(L"Color", L"SecondClass", PanelSecondClass->Color);

	ini->WriteInteger(L"Color", L"ActiveIn", this->ActiveIn->Color);
	ini->WriteInteger(L"Color", L"NotActiveIn", this->NotInActive->Color);
	ini->WriteInteger(L"Color", L"ActiveOut", this->ActiveOut->Color);
	ini->WriteInteger(L"Color", L"NotActiveOut", this->NotActiveOut->Color);

	ini->WriteInteger(L"Color", L"SelectedZone", this->pSelectZone->Color);
	ini->WriteInteger(L"Color", L"DeadZone", this->pDeadZone->Color);
	ini->WriteInteger(L"Color", L"sgD", this->sgD->Color);  SolidGroup_D = this->sgD->Color;
	ini->WriteInteger(L"Color", L"sgK", this->sgK->Color);  SolidGroup_K = this->sgK->Color;
	ini->WriteInteger(L"Color", L"sgE", this->sgE->Color);  SolidGroup_E = this->sgE->Color;
}
// ---------------------------------------------------------------------------
void __fastcall TFormColor::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	if (Key == 27)
		FormColor->Close();
}
// ---------------------------------------------------------------------------

