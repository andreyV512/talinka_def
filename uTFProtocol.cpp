#include <vcl.h>
#pragma hdrstop
#include "uTFProtocol.h"
#include <IniFiles.hpp>
#include "uFunctions.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TFProtocol *FProtocol;

// ---------------------------------------------------------------------------
__fastcall TFProtocol::TFProtocol(TComponent* Owner, TCriticalSection* _cs)
	: TForm(Owner)
{
	cs = _cs;
}
// ---------------------------------------------------------------------------

void __fastcall TFProtocol::FormCreate(TObject *Sender)
{
	L = new TStringList();
	TIniFile* ini = new TIniFile("..\\..\\Settings\\SettingsDefectoscope.ini");
	Timer1->Interval = ini->ReadInteger("Default", "ProtocolPeriod", 400);
	LoadFormPos(this, ini);
	cbProtocolToFile->Checked = ini->ReadBool("Default", "ProtocolToFile", 0);
	AnsiString FileName = "..\\..\\Protocols\\Protocol_" + DateToStr(Date()) + ".txt";
	file = fopen(FileName.c_str(), "a");
	if (file == NULL)
	{
		CreateDirectoryW(L"..\\..\\Protocols", 0);
		file = fopen(FileName.c_str(), "a");
	}
	// mProtocol->Perform(WM_VSCROLL,SB_BOTTOM,0);
	L->Add("Открыли протокол: " + TimeToStr(Time()));
	if (ini->ReadBool("OtherSettings", "ProtocolVisible", false))
		Show();
	delete ini;
	Timer1->Enabled = true;
}

// ---------------------------------------------------------------------------
void __fastcall TFProtocol::FormDestroy(TObject *Sender)
{
	Timer1->Enabled = false;
	fclose(file);
	delete L;
}
void TFProtocol::Save(void)
{
	TIniFile *ini = new TIniFile("..\\..\\Settings\\SettingsDefectoscope.ini");
	ini->WriteBool("Default", "ProtocolToFile", cbProtocolToFile->Checked);
	SaveFormPos(this, ini);
	ini->WriteBool("OtherSettings", "ProtocolVisible", Visible);
	delete ini;
}


// ---------------------------------------------------------------------------
void TFProtocol::Add(AnsiString _msg)
{
	L->Add(_msg);
}

void __fastcall TFProtocol::Timer1Timer(TObject *Sender)
{
	TTimer* s = (TTimer*)Sender;
	s->Enabled = false;
	cs->Enter();
	{

		for (int i = 0; i < L->Count; i++)
		{
			AnsiString a = L->Strings[i];
			if (Visible)
				mProtocol->Lines->Add(a);
			if (cbProtocolToFile->Checked)
			{
				fprintf(file, a.c_str());
				fprintf(file, "\n");
			}
		}
		if (cbProtocolToFile->Checked && L->Count > 0)
			fflush(file);
		L->Clear();
	} cs->Leave();
	s->Enabled = true;
}
// ---------------------------------------------------------------------------

void __fastcall TFProtocol::FormResize(TObject *Sender)
{
	cs->Enter();
	{
		mProtocol->Width = ClientWidth - mProtocol->Left * 2;
		mProtocol->Height = ClientHeight - mProtocol->Top - mProtocol->Left;
	} cs->Leave();
}
// ---------------------------------------------------------------------------
void __fastcall TFProtocol::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key!=27)
		return;
	cs->Enter();
	{
		Close();
	} cs->Leave();
}
//---------------------------------------------------------------------------

