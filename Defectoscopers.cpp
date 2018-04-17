// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Defectoscopers.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "Main.h"
#include "NewDefectoscoper.h"
#include "Password.h"
#include "ChangePassword.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDefectoscopersForm *DefectoscopersForm;

// ---------------------------------------------------------------------------
__fastcall TDefectoscopersForm::TDefectoscopersForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::FormActivate(TObject *Sender)
{
	//Подключимся к БД и обновим список активных дефектоскопистов
	QDefectoscopers->Connection = DBS->Connection();
	bRefreshClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TDefectoscopersForm::SelectAllClick(TObject *Sender)
{
	// Выберем все записи выведенные на просмотр
	QDefectoscopers->First();
	while (!QDefectoscopers->Eof)
	{
		DBGridDefectoscopers->SelectedRows->CurrentRowSelected = true;
		QDefectoscopers->Next();
	}
}

// ---------------------------------------------------------------------------
void TDefectoscopersForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}

// ----------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TDefectoscopersForm::FormResize(TObject *Sender)
{
	// Поправляет размеры и положение объектов при изменении размеров окна
	DBGridDefectoscopers->Height = ClientHeight - 2 - DBGridDefectoscopers->Top;
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::DeleteDefectoscoper(TObject *Sender)
{
	// Предупреждение
	if (Application->MessageBoxW(L"Удалить записи?", L"Дефектоскописты",
		MB_YESNO) == IDNO)
		return;

	// Защита мастер-паролем
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Идем по списку
		QDefectoscopers->First();
		while (!QDefectoscopers->Eof)
		{
			if ((DBGridDefectoscopers->SelectedRows->CurrentRowSelected)&&
				(QDefectoscopers->FieldByName("Name")->AsAnsiString!="Master"))
			{
                // Переводим дефектоскописта в неактивные
				int id = QDefectoscopers->FieldByName("ID")->AsInteger;
				AnsiString SQL;
				SQL = "update dbo.Defectoscopers set Active=0 where ID=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL E(SQL);
			}
			QDefectoscopers->Next();
		}
		bRefreshClick(this);
	}
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::bRefreshClick(TObject *Sender)
{
	// Обновим список активных дефектоскопистов
	QDefectoscopers->Close();
	QDefectoscopers->Active = false;
	QDefectoscopers->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT ID, Name FROM Defectoscopers ";
	AnsiString WhereStr = "";
	WhereStr += "where Defectoscopers.Active = 1";
	SQL += WhereStr;
	SQL += " order by Defectoscopers.ID";
	QDefectoscopers->SQL->Add(SQL);

	QDefectoscopers->ExecSQL();
	QDefectoscopers->Active = true;
}

// ---------------------------------------------------------------------------
void __fastcall TDefectoscopersForm::bAddClick(TObject *Sender)
{
	// Защита мастер-паролем
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Открываем форму ввода нового дефектоскописта
		TNewDefectoscoperForm* f = new TNewDefectoscoperForm(this,ini);
		f->ShowModal();
		delete f;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TDefectoscopersForm::bPasswordChangeClick(TObject *Sender)
{
	if (DBGridDefectoscopers->SelectedRows->Count > 0)
	{
		// Защита мастер-паролем
		TPasswordForm* PasswordForm = new TPasswordForm(this);
		PasswordForm->SetIni(ini);
		PasswordForm->SetUserName("Master");
		PasswordForm->ShowModal();
		delete PasswordForm;
		if (ini->ReadBool("Default", "IsPasswordOk", false))
		{
			// Ищем первого выбранного
			QDefectoscopers->First();
			while (!QDefectoscopers->Eof)
			{
				if (DBGridDefectoscopers->SelectedRows->CurrentRowSelected)
				{
					// Открываем форму смены пароля
					TChangePasswordForm* ChangePasswordForm=new TChangePasswordForm(this);
					ChangePasswordForm->SetIni(ini);
					ChangePasswordForm->eName->Text =
						QDefectoscopers->FieldByName("Name")->AsAnsiString;
					ChangePasswordForm->ShowModal();
					delete ChangePasswordForm;
					break;
				}
				QDefectoscopers->Next();
			}
			bRefreshClick(this);
		}
	}
}
// ---------------------------------------------------------------------------
