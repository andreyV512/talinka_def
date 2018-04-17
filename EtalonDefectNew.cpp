// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonDefectNew.h"
#include "EtalonTubeDefectsNew.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "EtalonDefectsCheck.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonDefectNewForm *EtalonDefectNewForm;

// ---------------------------------------------------------------------------
__fastcall TEtalonDefectNewForm::TEtalonDefectNewForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectNewForm::FormKeyPress(TObject *Sender,
	wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectNewForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectNewForm::FormActivate(TObject *Sender)
{
	// Подключимся к БД
	QEtalons->Connection = DBS->Connection();
	// Заполним поля
	eTypeSize->Text = ini->ReadInteger("Default", "TypeSize", 0);
	eDate->Text = DateTimeToStr((Date() + Time()));
}

// ---------------------------------------------------------------------------
void TEtalonDefectNewForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectNewForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectNewForm::bSaveClick(TObject *Sender)
{
	if (eEtalonName->Text != "")
	{
		// Вводим новый СОП в БД
		AnsiString Str = "";
		AnsiString SQL;
		SQL = "insert into dbo.EtalonDefect (Date,Name,TypeSize) values ('";
		SQL += eDate->Text;
		SQL += "','";
		SQL += eEtalonName->Text;
		SQL += "','";
		SQL += eTypeSize->Text;
		SQL += "')";
		CExecSQL E(SQL);
		if (E.IsOk() != "Ok")
		{
			// Ошибка сохранения
			Str = "Не смогли сохранить данные эталона!";
			TPr::pr(Str);
			Application->MessageBoxW
				(L"Не смогли сохранить данные эталона! Попробуйте сменить название.",
				L"Предупреждение!", MB_ICONWARNING);
		}
		else
		{
			Str = "Сохранили данные эталона";
			TPr::pr(Str);
			// Находим новую запись
			int IDTube = 0;
			QEtalons->Close();
			QEtalons->Active = false;
			QEtalons->SQL->Clear();
			SQL = "SELECT TOP 1 * FROM EtalonDefect ORDER BY ID DESC";
			QEtalons->SQL->Add(SQL);
			QEtalons->ExecSQL();
			QEtalons->Active = true;

			// Открываем окно ввода дефектов для нового СОП
			IDTube = QEtalons->FieldByName("ID")->AsInteger;
			if (IDTube > 0)
			{
				TEtalonTubeDefectsNewForm* EtalonTubeDefectsNewForm =
					new TEtalonTubeDefectsNewForm(this);
				EtalonTubeDefectsNewForm->IDTube = IDTube;
				Str = QEtalons->FieldByName("TypeSize")->AsAnsiString;
				EtalonTubeDefectsNewForm->TypeSize = Str;
				Str = QEtalons->FieldByName("Name")->AsAnsiString;
				EtalonTubeDefectsNewForm->Name = Str;
				EtalonTubeDefectsNewForm->SetIni(ini);
				EtalonTubeDefectsNewForm->ShowModal();
				delete EtalonTubeDefectsNewForm;
			}
			else
				Application->MessageBoxW(L"Ошибка: Неверный ID нового Эталона!",
				L"Предупреждение!", MB_ICONWARNING);

			Close();
		}
	}
	else
		Application->MessageBoxW(L"Введите имя нового эталона!",
		L"Предупреждение!", MB_ICONWARNING);
}
// ---------------------------------------------------------------------------
