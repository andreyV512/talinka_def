//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Authorisation.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAuthorisationForm *AuthorisationForm;
//---------------------------------------------------------------------------
__fastcall TAuthorisationForm::TAuthorisationForm(TComponent* Owner,
	TIniFile* _ini): TForm(Owner)
{
	KeyPreview = true;
	// Укажем файл с настройками
	ini = _ini;
}
//---------------------------------------------------------------------------
void __fastcall TAuthorisationForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
	if (Key == 13)
		bChooseClick(this);
}
// ---------------------------------------------------------------------------
void __fastcall TAuthorisationForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	// Для начала запишем, что авторизация не пройдена
	ini->WriteBool("Default","IsAuthorisationOk",false);
	// Составим список активных дефектоскопистов
	cbSelectDefectoscoper->Items->Clear();
	QDefectoscoper->Connection = DBS->Connection();
	QDefectoscoper->Close();
	QDefectoscoper->Active = false;
	QDefectoscoper->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT * FROM Defectoscopers ";
	SQL += " where Defectoscopers.Active = 1";
	SQL += " order by Defectoscopers.ID";
	QDefectoscoper->SQL->Add(SQL);
	QDefectoscoper->ExecSQL();
	QDefectoscoper->Active = true;
	QDefectoscoper->First();
	// Прочитаем ID последнего актвиного дефектоскописта
	int IDLastDef = ini->ReadInteger("Default","LastAuthorisationID",0);
	AnsiString LastDef = "";
	// Составим список
	while (!QDefectoscoper->Eof)
	{
		cbSelectDefectoscoper->Items->Add
			(QDefectoscoper->FieldByName("Name")->AsString);
		if (IDLastDef == QDefectoscoper->FieldByName("ID")->AsInteger)
			LastDef = QDefectoscoper->FieldByName("Name")->AsString;
		QDefectoscoper->Next();
	}
	// Выбор последнего активного
	if (LastDef != "")
		cbSelectDefectoscoper->ItemIndex = cbSelectDefectoscoper->Items->IndexOf(LastDef);
	// -----
}
//---------------------------------------------------------------------------
void __fastcall TAuthorisationForm::FormActivate(TObject *Sender)
{
	//TO DO
}
// ---------------------------------------------------------------------------
void __fastcall TAuthorisationForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
	cbSelectDefectoscoper->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TAuthorisationForm::bChooseClick(TObject *Sender)
{
	if (cbSelectDefectoscoper->Text != "")
	{
		// Ввод пароля дефектоскописта
		TPasswordForm* f = new TPasswordForm(this);
		f->SetIni(ini);
		f->SetUserName(cbSelectDefectoscoper->Text);
		f->ShowModal();
		delete f;
		AnsiString Str = "";
		if (ini->ReadBool("Default", "IsPasswordOk", false))
		{
			Str = "Авторизация подтверждена: " +
				cbSelectDefectoscoper->Text;
			TPr::pr(Str);
			ini->WriteBool("Default","IsAuthorisationOk", true);
            // Сохраним ID активного дефектоскописта
			QDefectoscoper->First();
			while (!QDefectoscoper->Eof)
			{
				if (QDefectoscoper->FieldByName("Name")->AsAnsiString
					== cbSelectDefectoscoper->Text)
				{
					ini->WriteInteger("Default","LastAuthorisationID",
						QDefectoscoper->FieldByName("ID")->AsInteger);
					ini->WriteString("Default","LastAuthorisation",
						QDefectoscoper->FieldByName("Name")->AsAnsiString);
					break;
				}
				QDefectoscoper->Next();
			}
			Close();
		}
		else
		{
			Str = "В авторизации отказано!";
			TPr::pr(Str);
			Application->MessageBoxW
				(L"В авторизации отказано! Проверьте Имя / Пароль!",
				L"Предупреждение!", MB_ICONWARNING);
		}
	}
}
//---------------------------------------------------------------------------
