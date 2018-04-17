//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFNewTypeSize.h"
#include "uFunctions.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFNewTypeSize *FNewTypeSize;
//---------------------------------------------------------------------------
__fastcall TFNewTypeSize::TFNewTypeSize(TComponent* Owner,TIniFile* _ini)
	: TForm(Owner)
{
	typeSize="";
	ini=_ini;
}
//---------------------------------------------------------------------------
void __fastcall TFNewTypeSize::Button1Click(TObject *Sender)
{
	typeSize=Edit1->Text.Trim();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFNewTypeSize::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key==VK_ESCAPE)
		Close();

}
//---------------------------------------------------------------------------
void __fastcall TFNewTypeSize::FormCreate(TObject *Sender)
{
	LoadFormPos(this,ini);
}
//---------------------------------------------------------------------------

void __fastcall TFNewTypeSize::FormDestroy(TObject *Sender)
{
	SaveFormPos(this,ini);
}
//---------------------------------------------------------------------------

