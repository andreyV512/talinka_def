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
	// Конструктор
	KeyPreview = true;
	OnLoadTube=NULL;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::ViewClick(TObject *Sender)
{
	// Вывести в таблицу все записи за указанный срок
	QChecks->Close();
	QChecks->Active = false;
	QChecks->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT * FROM EtalonCheckResult ";
	AnsiString WhereStr = "";
	if (cbTypeSize->Text == "Все")
		WhereStr = "where";
	else if (cbTypeSize->Text != "Все")
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
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
}
//---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormActivate(TObject *Sender)
{
	// Подключимся к БД
	QChecks->Connection = DBS->Connection();
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
	dtpFrom->DateTime = Date() - 1;
	dtpTo->DateTime = Date();
	ViewClick(this);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonCheckJournalForm::SelectAllClick(TObject *Sender)
{
	// Выберем все записи выведенные на просмотр
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
	// Укажем файл с настройками
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonCheckJournalForm::FormResize(TObject *Sender)
{
	// Поправляет размеры и положение объектов при изменении размеров окна
	DBNavigatorEtalons->Top = ClientHeight - DBNavigatorEtalons->Height - 3;
	DBGridEtalons->Height = DBNavigatorEtalons->Top - 2 - DBGridEtalons->Top;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonCheckJournalForm::DeleteEtalonCheckResult(TObject *Sender)
{
	// Предупреждение
	if (Application->MessageBoxW(L"Удалить записи?",
		L"Журнал контроля", MB_YESNO) == IDNO)
		return ;
	// Защита мастер-паролем
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Идем по списку записей
		QChecks->First();
		while (!QChecks->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
			{
                // Удалим запись из БД
				int id = QChecks->FieldByName("ID")->AsInteger;
				AnsiString SQL;
				SQL = "delete from dbo.EtalonCheckResult where ID=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL E(SQL);
				// Удалим файл с данными
				if(remove(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str()))
				{
					UnicodeString warnMSG = "Файл не был удален, ";
					warnMSG += "однако, запись из базы была удалена! ";
					warnMSG += "Возможно, файл был удален или перемещен ранее?";
					Application->MessageBoxW(warnMSG.w_str(),
						L"Предупреждение!", MB_ICONWARNING);
				}
			}
			// Следующая запись
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
		// Находим первую выбранную запись
		QChecks->First();
		while (!QChecks->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
				break;
			QChecks->Next();
		}
		// Пытаемся открыть сохраненные данные по трубе из файла
		if(OnLoadTube!=NULL)
		{
			FILE *file;
			file = fopen(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str(), "r+");
	        if (file == NULL)
			{
				Application->MessageBoxW(L"Не удалось открыть файл для чтения", L"Ошибка",
				MB_ICONERROR | MB_OK);
				return;
			}
			OnLoadTube(QChecks->FieldByName("SaveAddress")->AsAnsiString.c_str());
		}
		Close();
	}
}
//---------------------------------------------------------------------------

