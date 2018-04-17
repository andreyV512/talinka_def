// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFRSOP.h"
#include "uCSelect.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFRSOP *FRSOP;

// ---------------------------------------------------------------------------
__fastcall TFRSOP::TFRSOP(TComponent* Owner) : TFrame(Owner)
{

}

// ---------------------------------------------------------------------------
void TFRSOP::Init(TIniFile* _ini)
{
	ini = _ini;
	AnsiString SQL = "SELECT * FROM EtalonDefect where EtalonDefect.ID=";
	SQL += ini->ReadInteger("OtherSettings", "SOPCheckID", 0);
	CSelect S(SQL);
	if (S.Count() != 0)
		cbSelectEtalon->Text = S.AsAnsiString("Name");
	else
	{
		CSelect S1("SELECT * FROM EtalonDefect order by ID");
		if (S1.Count() != 0)
			cbSelectEtalon->Text = S1.AsAnsiString("Name");
	}
	cbSelectEtalonDropDown(NULL);
	cbEtalonView->Checked = ini->ReadBool("OtherSettings",
		"ViewEtalonCheck", 0);
}

// ---------------------------------------------------------------------------
void __fastcall TFRSOP::cbSelectEtalonDropDown(TObject *Sender)
{
	CSelect S("SELECT * FROM EtalonDefect order by ID");
	cbSelectEtalon->Items->Clear();
	for (int i = 0; i < S.Count(); i++)
	{
		cbSelectEtalon->Items->Add(S.AsAnsiString("Name"));
		S.Next();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFRSOP::cbSelectEtalonChange(TObject *Sender)
{
	AnsiString SQL = "SELECT * FROM EtalonDefect where Name='";
	SQL += cbSelectEtalon->Text;
	SQL += "' order by ID";
	CSelect S(SQL);
	if (S.Count() == 0)
		return;
	ini->WriteInteger("OtherSettings", "SOPCheckID", S.AsInt("ID"));
}

// ---------------------------------------------------------------------------
void __fastcall TFRSOP::cbEtalonViewClick(TObject *Sender)
{
	ini->WriteBool("OtherSettings", "ViewEtalonCheck", cbEtalonView->Checked);
}
// ---------------------------------------------------------------------------
