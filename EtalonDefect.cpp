//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonDefect.h"
#include "EtalonTubeDefects.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "EtalonDefectsCheck.h"
#include "Password.h"
#include "Singleton.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonDefectForm *EtalonDefectForm;
//---------------------------------------------------------------------------
__fastcall TEtalonDefectForm::TEtalonDefectForm(TComponent* Owner)	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::ViewClick(TObject *Sender)
{
	QEtalons->Close();
	QEtalons->Active = false;
	QEtalons->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT * FROM EtalonDefect ";
	AnsiString WhereStr = "";
	if (cbTypeSize->Text == "Все")
		WhereStr = "";
	else if (cbTypeSize->Text != "Все")
		WhereStr = "where EtalonDefect.TypeSize='" + cbTypeSize->Text + "'";
	SQL += WhereStr;
	SQL += " order by EtalonDefect.id";
	QEtalons->SQL->Add(SQL);

	QEtalons->ExecSQL();
	QEtalons->Active = true;

	// DBGridEtalons->DataSource = DataEtalons;
	// DBNavigatorEtalons->DataSource = DataEtalons;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	// Активируем окно и составим список типоразмеров
	FormActivate(this);
	// Выведем список СОП
	ViewClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::FormActivate(TObject *Sender)
{
	// Подключимся к БД
	QEtalons->Connection = DBS->Connection();
	cbTypeSize->Clear();

	// Получим список типоразмеров
	TStringList * sections = new TStringList();
	ini->ReadSections(sections);
	cbTypeSize->Items->Add("Все");
	for (int i = 0; i < sections->Count; i++)
		if (sections->Strings[i].SubString(1, 5) == "Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6, 20), NULL);

	//cbTypeSize->ItemIndex = cbTypeSize->Items->IndexOf(ini->ReadString("Default", "TypeSize", "1"));
	sections->~TStringList();
	if (cbTypeSize->Items->Count == 0)
		Application->MessageBoxW
			(L"Ни одного типоразмера не найдено!",
		L"Предупреждение!", MB_ICONWARNING);

	cbTypeSize->ItemIndex = 0;
}
// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::ViewEtalonTubeDefects(TObject *Sender)
{
	if (DBGridEtalons->SelectedRows->Count > 0)
	{
		// Составим список выбранных СОП
		std::vector<int>IndexSelectedField;
		QEtalons->First();
		while (!QEtalons->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
				IndexSelectedField.push_back(QEtalons->FieldByName("ID")->AsInteger);
			QEtalons->Next();
		}
		//QEtalons->Active=false;
		// Откроем просмотр списка дефектов для выбранных СОПов
		TEtalonTubeDefectsForm* EtalonTubeDefectsForm=new TEtalonTubeDefectsForm(this);
		EtalonTubeDefectsForm->IndexField.clear();
		EtalonTubeDefectsForm->IndexField = IndexSelectedField;
		EtalonTubeDefectsForm->SetIni(ini);
		EtalonTubeDefectsForm->ShowModal();
		delete EtalonTubeDefectsForm;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonDefectForm::SelectAllClick(TObject *Sender)
{
	// Выберем все выведенные на просмотр СОП
	QEtalons->First();
	while (!QEtalons->Eof)
	{
		DBGridEtalons->SelectedRows->CurrentRowSelected = true;
		QEtalons->Next();
	}
}

// ---------------------------------------------------------------------------
void TEtalonDefectForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonDefectForm::FormResize(TObject *Sender)
{
	// Поправляет размеры и положение объектов при изменении размеров окна
	bCheck->Top = ClientHeight - bCheck->Height - 3;
	DBNavigatorEtalons->Top = bCheck->Top - DBNavigatorEtalons->Height - 2;
	DBGridEtalons->Height = DBNavigatorEtalons->Top - 2 - DBGridEtalons->Top
		- 2 - bCheck->Height;
	bCheck->Top = DBNavigatorEtalons->Top + DBNavigatorEtalons->Height + 2;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::DeleteEtalonDefect(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Предупреждение
		if (Application->MessageBoxW(L"Удалить записи?",
			L"СОП", MB_YESNO) == IDNO)
			return ;

		// Идем по списку записей
		QEtalons->First();
		while (!QEtalons->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
			{
				// Удаляем из БД записи с дефектами СОП
				int id = QEtalons->FieldByName("ID")->AsInteger;
				AnsiString SQL;
				SQL = "delete from dbo.EtalonTubeDefects where IDTube=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL E(SQL);
				// Удаляем из БД запись с самим СОП
				SQL = "delete from dbo.EtalonDefect where ID=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL EE(SQL);
			}
			QEtalons->Next();
		}
		// Обновим список в окне
		ViewClick(this);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectForm::bCheckClick(TObject *Sender)
{
	if(Singleton->isSOP)
	{
		if (DBGridEtalons->SelectedRows->Count > 0)
		{
			// Идем по списку и ищем первый выбранный СОП
			int ID = 0;
			QEtalons->First();
			while (!QEtalons->Eof)
			{
				if (DBGridEtalons->SelectedRows->CurrentRowSelected)
				{
					ID = QEtalons->FieldByName("ID")->AsInteger;
					break;
				}
				QEtalons->Next();
			}
			// Проверяем выбран ли СОП для проверки
			if (ID > 0)
			{
				// Проверяем совпадает ли типоразмер текущей трубы с СОПом
				if (ini->ReadString("Default", "TypeSize", "1")
					== QEtalons->FieldByName("TypeSize")->AsInteger)
				{
						// Вызываем форму сверки трубы с СОПом
						TEtalonDefectsCheckForm* EtalonDefectsCheckForm=new TEtalonDefectsCheckForm(this);
						EtalonDefectsCheckForm->SetIni(ini);
						EtalonDefectsCheckForm->SetIDTube(ID);
						EtalonDefectsCheckForm->ShowModal();
						delete EtalonDefectsCheckForm;

				}
				else Application->MessageBoxW(L"Текущий типоразмер не совпадает с эталоном!",
						L"Предупреждение!", MB_ICONWARNING);
			}
			else Application->MessageBoxW(L"Выберете эталон для проверки!",
				L"Предупреждение!", MB_ICONWARNING);
		}
	}
	else
		Application->MessageBoxW(L"Не включен Режим СОП! Сверка запрещена!",
			L"Ошибка", MB_ICONERROR | MB_OK);
}
//---------------------------------------------------------------------------
