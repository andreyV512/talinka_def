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
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
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
	// Укажем файл с настройками
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TChangePasswordForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
	// Очистим поля
	eName->Text = "";
    ePassword->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TChangePasswordForm::bChangeClick(TObject *Sender)
{
	// Проверим что поля заполнены
	if ((eName->Text != "") && (ePassword->Text != ""))
	{
		// Пробуем сменить пароль
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
			Str = "Не смогли сменить пароль дефектоскописта!";
			TPr::pr(Str);
			Application->MessageBoxW(L"Не смогли сменить пароль дефектоскописта!",
				L"Предупреждение!", MB_ICONWARNING);
		}
		else
		{
			Str = "Сменили пароль дефектоскописта";
			TPr::pr(Str);
			Close();
		}
	}
	else Application->MessageBoxW(L"Введите новый пароль дефектоскописта!",
					L"Предупреждение!", MB_ICONWARNING);
}
//---------------------------------------------------------------------------
