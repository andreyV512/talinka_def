//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ChangePassword.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChangePasswordForm *ChangePasswordForm;
//---------------------------------------------------------------------------
__fastcall TChangePasswordForm::TChangePasswordForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}
//---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// �������� ���� �� ������� Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormShow(TObject *Sender)
{
	// ����������� ��������� ���� �� ������ � ��� �������
	LoadFormPos(this, ini);
}
//---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormActivate(TObject *Sender)
{
	//TO DO
}
// ---------------------------------------------------------------------------
void TChangePasswordForm::SetIni(TIniFile* _ini)
{
	// ������ ���� � �����������
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// �������� ��������� ���� �� ������ � ��� �������
	SaveFormPos(this, ini);
	// ������� ����
	eName->Text = "";
    ePassword->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::bChangeClick(TObject *Sender)
{
	// �������� ��� ���� ���������
	if ((eName->Text != "") && (ePassword->Text != ""))
	{
		// ������� ������� ������
		AnsiString Str = "";
		AnsiString SQL;
		SQL = "UPDATE dbo.Defectoscopers Set Password = '";
		SQL += PasswordForm->Encoder(ePassword->Text);
		SQL += "' where Name = '";
		SQL += eName->Text;
		SQL += "'";
		CExecSQL E(SQL);
		if (E.IsOk() != "Ok")
		{
			Str = "�� ������ ������� ������ ���������������!";
			TPr::pr(Str);
			Application->MessageBoxW(L"�� ������ ������� ������ ���������������!",
				L"��������������!", MB_ICONWARNING);
		}
		else
		{
			Str = "������� ������ ���������������";
			TPr::pr(Str);
			Close();
		}
	}
	else Application->MessageBoxW(L"������� ����� ������ ���������������!",
					L"��������������!", MB_ICONWARNING);
}
//---------------------------------------------------------------------------
