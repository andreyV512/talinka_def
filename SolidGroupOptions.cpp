//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SolidGroupOptions.h"
#include "Singleton.h"
#include "Global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSolidGroupOptionsForm *SolidGroupOptionsForm;
//---------------------------------------------------------------------------
__fastcall TSolidGroupOptionsForm::TSolidGroupOptionsForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSolidGroupOptionsForm::FormActivate(TObject *Sender)
{
	cbSolidGroupSwitch->Checked = Singleton->solidGroupSwitch;
	cboxSelectGroup->Text = Singleton->currentSolidGroup;
}
//---------------------------------------------------------------------------
void __fastcall TSolidGroupOptionsForm::cbSolidGroupSwitchClick(TObject *Sender)
{
	 Singleton->solidGroupSwitch = cbSolidGroupSwitch->Checked;
	 TIniFile *_ini = new TIniFile(Globals::IniFileName);
	 AnsiString sect = "Type_" + _ini->ReadString("Default", "TypeSize", "1");
	 _ini->WriteInteger(sect, "solidGroupSwitch", Singleton->solidGroupSwitch);
	 if(!Singleton->solidGroupSwitch)
	 {
		  Singleton->defaultSolidGroup = _ini->ReadString(sect, "defaultSolidGroup", "K")[0];
	 }
	 delete _ini;
}
//---------------------------------------------------------------------------
void __fastcall TSolidGroupOptionsForm::cboxSelectGroupChange(TObject *Sender)
{
   Singleton->defaultSolidGroup =cboxSelectGroup->Text[0];
	 TIniFile *_ini = new TIniFile(Globals::IniFileName);
	 AnsiString sect = "Type_" + _ini->ReadString("Default", "TypeSize", "1");
	 _ini->WriteString(sect, "defaultSolidGroup", Singleton->defaultSolidGroup);
	 delete _ini;
}
//---------------------------------------------------------------------------
