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
	// Укажем файл с настройками
	ini = _ini;
}
//---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
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
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
	// Очистим поля
	eName->Text = "";
	ePassword->Text = "";
}
//---------------------------------------------------------------------------
void __fastcall TNewDefectoscoperForm::bAddClick(TObject *Sender)
{
	if ((eName->Text != "") && (ePassword->Text != ""))
	{
		//Проверка нет ли такого же старого имени
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
			//Если уже есть такой оператор, то проверим его активность
			bool _Active = QDefectoscoper->FieldByName("Active")->AsBoolean;
			if(_Active)
			{
				//Если уже есть такой активный оператор, то сообщим об ошибке
				Application->MessageBoxW(L"Уже есть активный дефектоскопист с этим именем!",
					L"Предупреждение!", MB_ICONWARNING);
			}
			else
			{
				//Если оператор не активен, то активируем его учетную запись
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
					Str = "Не смогли активировать старого дефектоскописта!";
					TPr::pr(Str);
					Application->MessageBoxW(L"Не смогли активировать старого дефектоскописта! Попробуйте сменить имя.",
						L"Предупреждение!", MB_ICONWARNING);
				}
				else
				{
					Str = "Активировали старого дефектоскописта";
					TPr::pr(Str);
					Close();
				}
			}
		}
		else
		{
			//Оператора с таким именем еще нет - добавим его
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
				Str = "Не смогли добавить нового дефектоскописта!";
				TPr::pr(Str);
				Application->MessageBoxW(L"Не смогли добавить нового дефектоскописта! Попробуйте сменить имя.",
					L"Предупреждение!", MB_ICONWARNING);
			}
			else
			{
				Str = "Добавили нового дефектоскописта";
				TPr::pr(Str);
				Close();
			}
		}
	}
	else Application->MessageBoxW(L"Введите имя и пароль нового дефектоскописта!",
					L"Предупреждение!", MB_ICONWARNING);
}
//---------------------------------------------------------------------------
