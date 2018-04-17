//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonCheckJournal.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Password.h"
//#include "Main.h"
//-----------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonCheckJournalForm *EtalonCheckJournalForm;
//---------------------------------------------------------------------------
__fastcall TEtalonCheckJournalForm::TEtalonCheckJournalForm(TComponent* Owner)	: TForm(Owner)
{
	// �����������
	KeyPreview = true;
	OnLoadTube=NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::ViewClick(TObject *Sender)
{
	// ������� � ������� ��� ������ �� ��������� ����
	QChecks->Close();
	QChecks->Active = false;
	QChecks->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT * FROM EtalonCheckResult ";
	AnsiString WhereStr = "";
	if (cbTypeSize->Text == "���")
		WhereStr = "where";
	else if (cbTypeSize->Text != "���")
		WhereStr += "where EtalonCheckResult.TypeSize='" + cbTypeSize->Text + "' and";
	WhereStr += " EtalonCheckResult.Date > '" + DateToStr(dtpFrom->DateTime) + "'";
	WhereStr += " and EtalonCheckResult.Date <= '" + DateToStr(dtpTo->DateTime + 1) + "'";
	SQL += WhereStr;
	SQL += " order by EtalonCheckResult.id";
	QChecks->SQL->Add(SQL);

	QChecks->ExecSQL();
	QChecks->Active = true;

	// DBGridEtalons->DataSource = DataEtalons;
	// DBNavigatorEtalons->DataSource = DataEtalons;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// �������� ���� �� ������� Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormShow(TObject *Sender)
{
	// ����������� ��������� ���� �� ������ � ��� �������
	LoadFormPos(this, ini);
}
//---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormActivate(TObject *Sender)
{
	// ����������� � ��
	QChecks->Connection = DBS->Connection();
	cbTypeSize->Clear();

	// ������� ������ ������������
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	cbTypeSize->Items->Add("���");
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	//cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(ini->ReadString("Default", "TypeSize", "1"));
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW
			(L"�� ������ ����������� �� �������!",
		L"��������������!", MB_ICONWARNING);

	cbTypeSize->ItemIndex = 0;
	dtpFrom->DateTime = Date() - 1;
	dtpTo->DateTime = Date();
	ViewClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonCheckJournalForm::SelectAllClick(TObject *Sender)
{
	// ������� ��� ������ ���������� �� ��������
	QChecks->First();
	while (!QChecks->Eof)
	{
		DBGridEtalons->SelectedRows->CurrentRowSelected = true;
		QChecks->Next();
	}
}

// ---------------------------------------------------------------------------
void TEtalonCheckJournalForm::SetIni(TIniFile* _ini)
{
	// ������ ���� � �����������
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// �������� ��������� ���� �� ������ � ��� �������
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonCheckJournalForm::FormResize(TObject *Sender)
{
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	DBNavigatorEtalons->Top = ClientHeight - DBNavigatorEtalons->Height - 3;
	DBGridEtalons->Height = DBNavigatorEtalons->Top - 2 - DBGridEtalons->Top;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::DeleteEtalonCheckResult(TObject *Sender)
{
	// ��������������
	if (Application->MessageBoxW(L"������� ������?",
		L"������ ��������", MB_YESNO) == IDNO)
		return ;
	// ������ ������-�������
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// ���� �� ������ �������
		QChecks->First();
		while (!QChecks->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
			{
                // ������ ������ �� ��
				int id = QChecks->FieldByName("ID")->AsInteger;
				AnsiString SQL;
				SQL = "delete from dbo.EtalonCheckResult where ID=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL E(SQL);
				// ������ ���� � �������
				if(remove(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str()))
				{
					UnicodeString warnMSG = "���� �� ��� ������, ";
					warnMSG += "������, ������ �� ���� ���� �������! ";
					warnMSG += "��������, ���� ��� ������ ��� ��������� �����?";
					Application->MessageBoxW(warnMSG.w_str(),
						L"��������������!", MB_ICONWARNING);
				}
			}
			// ��������� ������
			QChecks->Next();
		}
		ViewClick(this);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::LoadTubeClick(TObject *Sender)
{
	if (DBGridEtalons->SelectedRows->Count > 0)
	{
		// ������� ������ ��������� ������
		QChecks->First();
		while (!QChecks->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
				break;
			QChecks->Next();
		}
		// �������� ������� ����������� ������ �� ����� �� �����
		if(OnLoadTube!=NULL)
		{
			FILE *file;
			file = fopen(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str(), "r+");
	        if (file == NULL)
			{
				Application->MessageBoxW(L"�� ������� ������� ���� ��� ������", L"������",
				MB_ICONERROR | MB_OK);
				return;
			}
			OnLoadTube(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str());
		}
		Close();
	}
}
//---------------------------------------------------------------------------

