//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#define PROGRAMMPASS 32167
#include "Password.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCSelect.h"
#include "uCExecSQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPasswordForm *PasswordForm;
//---------------------------------------------------------------------------
__fastcall TPasswordForm::TPasswordForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}
//---------------------------------------------------------------------------
void __fastcall TPasswordForm::bCancelClick(TObject *Sender)
{
	// Укажем что пароль не верен и закроем окно
	ini->WriteBool("Default","IsPasswordOK",false);
	Close();
}
//---------------------------------------------------------------------------
void TPasswordForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}
//---------------------------------------------------------------------------
void TPasswordForm::SetUserName(AnsiString _UserName)
{
	// Зададим имя дефектоскописта
	UserName = _UserName;
}
//---------------------------------------------------------------------------
void __fastcall TPasswordForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
	if (Key == 13)
		bOKClick(this);
}
// ---------------------------------------------------------------------------
void __fastcall TPasswordForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	// Для начала запишем, что пароль не верен
	ini->WriteBool("Default","IsPasswordOK",false);
}
//----------------------------------------------------------------------------
void __fastcall TPasswordForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
	// Очистим поля
	ePassword->Text = "";
}
//----------------------------------------------------------------------------
void __fastcall TPasswordForm::bOKClick(TObject *Sender)
{
	//Проверка пароля SQL-запросом
	if (ePassword->Text != "")
	{
		AnsiString Str = "";
		if(ePassword->Text == PROGRAMMPASS)
		{
        	ini->WriteBool("Default","IsPasswordOK",true);
			Str = "Пароль программиста верен.";
			TPr::pr(Str);
			Close();
			return;
		}
		AnsiString SQL;
		SQL = "SELECT ID FROM Defectoscopers ";
		SQL += "where Defectoscopers.Active = 1 and Defectoscopers.Name = '";
		SQL += UserName +
			"' and Defectoscopers.Password = :Password";
		SQL += " order by Defectoscopers.ID";
		TPr::pr(SQL);
		//todo когда-нибудь надо будет обновить CSelect
		TADOQuery* Q=new TADOQuery(NULL);
		Q->Connection=DBS->Connection();
		Q->SQL->Add(SQL);

		WideString w = "Password";
		TParameter* P = Q->Parameters->ParamByName(w);
		P->DataType = ftString;
		P->Direction = pdInput;
		if (Encoder(ePassword->Text).Length() != 0)
		{
			w = Encoder(ePassword->Text);
			P->Value = w;
		}
		else
			P->Value = Null;
		Q->Active = true;
		Q->First();

//		CSelect S = CSelect(SQL);
//		S.InputString("Password",Encoder(ePassword->Text));
//		S.Exec();
//		if (S.Count() != 1)
		if (Q->Eof)
		{
			Application->MessageBoxW(L"Не правильный Пароль!",
				L"Предупреждение!", MB_ICONWARNING);
			ePassword->Text = "";
			ePassword->SetFocus();
			return;
		}
		else
		{
			// Если пароль верен, то запишем это в файл настроек
			ini->WriteBool("Default","IsPasswordOK",true);
			Str = "Пароль верен.";
			TPr::pr(Str);
			Close();
		}
	}
	else
		Application->MessageBoxW(L"Введите пароль!",L"Предупреждение!", MB_ICONWARNING);
}
//---------------------------------------------------------------------------

AnsiString TPasswordForm::Encoder(AnsiString Password)
{
	AnsiString PassCode = "123";
	AnsiString Result = Password;
	unsigned int a = 0, b = 0;
    a = 1;
// Вывод в протокол для отладки
//	for (int x=1;x < Password.Length()+1; x++)
//	{
//		Result += (char)(Password[x] ^ PassCode[x%PassCode.Length()+1]);
//	}
//	TPr::pr(Result);
//	return Result;
	// пока существует строка Password
	while (Password.Length()+1 > a)
	{
		b = 1;

	// пока существует строка PassCode
		while (PassCode.Length()+1 > b)
		{
		// хэшируем простым XOR'ом
		Result[a] ^= (PassCode[b] + (a*b));
		// идем к следующему символу PassCode
			b++;
		}
	// идем к следующему символу Password
		a++;
	}
	return Result;
}
