//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewDefectoscoper.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewDefectoscoperForm *NewDefectoscoperForm;
//---------------------------------------------------------------------------
__fastcall TNewDefectoscoperForm::TNewDefectoscoperForm(TComponent* Owner,TIniFile* _ini)
	: TForm(Owner)
{
	KeyPreview = true;
	// ������ ���� � �����������
	ini = _ini;
}
//---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// �������� ���� �� ������� Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormShow(TObject *Sender)
{
	// ����������� ��������� ���� �� ������ � ��� �������
	LoadFormPos(this, ini);
}
//---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormActivate(TObject *Sender)
{
	//TO DO
}
// ---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// �������� ��������� ���� �� ������ � ��� �������
	SaveFormPos(this, ini);
	// ������� ����
	eName->Text = "";
	ePassword->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::bAddClick(TObject *Sender)
{
	if ((eName->Text != "") && (ePassword->Text != ""))
	{
		//�������� ��� �� ������ �� ������� �����
		QDefectoscoper->Connection = DBS->Connection();
		QDefectoscoper->Close();
		QDefectoscoper->Active = false;
		QDefectoscoper->SQL->Clear();

		AnsiString Str = "";
		AnsiString SQL;
		SQL = "SELECT ID, Name, Password, Active FROM Defectoscopers ";
		SQL += "where Defectoscopers.Name = '";
		SQL += eName->Text + "'";
		SQL += " order by Defectoscopers.ID";
		TPr::pr(SQL);

		QDefectoscoper->SQL->Add(SQL);
		QDefectoscoper->ExecSQL();
		QDefectoscoper->Active = true;
		QDefectoscoper->First();
		if(!QDefectoscoper->Eof)
		{
			//���� ��� ���� ����� ��������, �� �������� ��� ����������
			bool _Active = QDefectoscoper->FieldByName("Active")->AsBoolean;
			if(_Active)
			{
				//���� ��� ���� ����� �������� ��������, �� ������� �� ������
				Application->MessageBoxW(L"��� ���� �������� �������������� � ���� ������!",
					L"��������������!", MB_ICONWARNING);
			}
			else
			{
				//���� �������� �� �������, �� ���������� ��� ������� ������
				SQL = "update dbo.Defectoscopers set Active=1, Password='";
				TPasswordForm* f=new TPasswordForm(this);
				SQL += f->Encoder(ePassword->Text);
				delete f;
				SQL += "' where Defectoscopers.Name = '";
				SQL += eName->Text;
				SQL += "'";
				CExecSQL E(SQL);
				if (E.IsOk() != "Ok")
				{
					Str = "�� ������ ������������ ������� ���������������!";
					TPr::pr(Str);
					Application->MessageBoxW(L"�� ������ ������������ ������� ���������������! ���������� ������� ���.",
						L"��������������!", MB_ICONWARNING);
				}
				else
				{
					Str = "������������ ������� ���������������";
					TPr::pr(Str);
					Close();
				}
			}
		}
		else
		{
			//��������� � ����� ������ ��� ��� - ������� ���
			SQL = "insert into dbo.Defectoscopers (Name,Password,Active) values ('";
			SQL += eName->Text;
			SQL += "','";
			TPasswordForm* f=new TPasswordForm(this);
			SQL += f->Encoder(ePassword->Text);
			delete f;
			SQL += "',1)";
			CExecSQL E(SQL);
			if (E.IsOk() != "Ok")
			{
				Str = "�� ������ �������� ������ ���������������!";
				TPr::pr(Str);
				Application->MessageBoxW(L"�� ������ �������� ������ ���������������! ���������� ������� ���.",
					L"��������������!", MB_ICONWARNING);
			}
			else
			{
				Str = "�������� ������ ���������������";
				TPr::pr(Str);
				Close();
			}
		}
	}
	else Application->MessageBoxW(L"������� ��� � ������ ������ ���������������!",
					L"��������������!", MB_ICONWARNING);
}
//---------------------------------------------------------------------------
