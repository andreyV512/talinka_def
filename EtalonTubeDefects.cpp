//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonTubeDefects.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "EtalonNewDefect.h"
#include "Password.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonTubeDefectsForm *EtalonTubeDefectsForm;
//---------------------------------------------------------------------------
__fastcall TEtalonTubeDefectsForm::TEtalonTubeDefectsForm(TComponent* Owner)	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsForm::ViewClick(TObject *Sender)
{
	// Выведем все дефекты для СОПов выбранного типоразмера
	QEtalons->Close();
	QEtalons->Active = false;
	QEtalons->SQL->Clear();
	AnsiString SQL;
//	SQL = "SELECT * FROM EtalonTubeDefects ";
	SQL = "SELECT ID, IDTube, Name, TypeSize,";
	SQL += " Module = CASE Module when 1 then 'Поперечный' when 2 then 'Продольный'";
	SQL += " when 3 then 'Толщиномер' else 'Unknown' END, ";
	SQL += " DefType,DefPoint,DefRadius,isTopBrackBorder,TopBorderPercent,";
	SQL += " isBottomBrackBorder,BottomBorderPercent";
	SQL += " FROM EtalonTubeDefects ";
	AnsiString WhereStr = "";
	if (cbTypeSize->Text == "Все")
		WhereStr = "";
	else if (cbTypeSize->Text != "Все")
		WhereStr = "where EtalonTubeDefects.TypeSize='" + cbTypeSize->Text + "'";
	SQL += WhereStr;
	SQL += " order by EtalonTubeDefects.id";
	QEtalons->SQL->Add(SQL);

	QEtalons->ExecSQL();
	QEtalons->Active = true;

	// DBGridEtalons->DataSource = DataEtalons;
	// DBNavigatorEtalons->DataSource = DataEtalons;
}

// ---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
//----------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	// Выведем на экран список дефектов для СОПов из списка
	AnsiString Str = "Покажем дефекты эталонов";
	TPr::pr(Str);
	FormActivate(this);
//	QEtalons->Connection = DBS->Connection();
//	QEtalons->Close();
//	QEtalons->Active = false;
//	QEtalons->SQL->Clear();
//	AnsiString SQL;
//	SQL = "SELECT * FROM EtalonTubeDefects ";
//	AnsiString WhereStr = "where ";
//	int ID = 0;
//	while (ID < (int)IndexField.size())
//	{
//		WhereStr += "EtalonTubeDefects.IDTube='" + IntToStr(IndexField[ID]) + "'";
//		ID++;
//		if (ID < (int)IndexField.size())
//		{
//			WhereStr += " or ";
//		}
//	}
//	if (ID > 0)
//		SQL += WhereStr;
//	SQL += " order by EtalonTubeDefects.id";
//	QEtalons->SQL->Add(SQL);
//
//	QEtalons->ExecSQL();
//	QEtalons->Active = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsForm::FormActivate(TObject *Sender)
{
	//QEtalons->Connection = DBS->Connection();
	cbTypeSize->Clear();

	// Получим типоразмеры
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

	// Обновим список контрольных точек
	QEtalons->Connection = DBS->Connection();
	QEtalons->Close();
	QEtalons->Active = false;
	QEtalons->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT ID, IDTube, Name, TypeSize,";
	SQL += " Module = CASE Module when 1 then 'Поперечный' when 2 then 'Продольный'";
	SQL += " when 3 then 'Толщиномер' else 'Unknown' END, ";
	SQL += " DefType,DefPoint,DefRadius,isTopBrackBorder,TopBorderPercent,";
	SQL += " isBottomBrackBorder,BottomBorderPercent";
	SQL += " FROM EtalonTubeDefects ";
	AnsiString WhereStr = "where ";
	int ID = 0;
	while (ID < (int)IndexField.size())
	{
		WhereStr += "EtalonTubeDefects.IDTube='" + IntToStr(IndexField[ID]) + "'";
		ID++;
		if (ID < (int)IndexField.size())
		{
			WhereStr += " or ";
		}
	}
	if (ID > 0)
		SQL += WhereStr;
	SQL += " order by EtalonTubeDefects.id";
	QEtalons->SQL->Add(SQL);

	QEtalons->ExecSQL();
	QEtalons->Active = true;
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::SelectAllClick(TObject *Sender)
{
	// Выберем все записи выведенные на просмотр
	QEtalons->First();
	while (!QEtalons->Eof)
	{
		DBGridEtalons->SelectedRows->CurrentRowSelected = true;
		QEtalons->Next();
	}
}
// ---------------------------------------------------------------------------
void TEtalonTubeDefectsForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::FormResize(TObject *Sender)
{
	// Поправляет размеры и положение объектов при изменении размеров окна
	DBNavigatorEtalons->Top = ClientHeight - DBNavigatorEtalons->Height - 3;
	DBGridEtalons->Height = DBNavigatorEtalons->Top - 2 - DBGridEtalons->Top;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsForm::DeleteEtalonDefect(TObject *Sender)
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
			L"Контрольные точки", MB_YESNO) == IDNO)
			return ;

		// Идем по списку
		QEtalons->First();
		while (!QEtalons->Eof)
		{
			if (DBGridEtalons->SelectedRows->CurrentRowSelected)
			{
				// Удаляем запись из БД
				int id = QEtalons->FieldByName("ID")->AsInteger;
				AnsiString SQL;
				SQL = "delete from dbo.EtalonTubeDefects where id=";
				SQL += id;
				TPr::pr(SQL);
				CExecSQL E(SQL);
			}
			QEtalons->Next();
		}
		FormActivate(this);
	}
}
//---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::bAddClick(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Вызовем форму для добавления нового дефекта
		AnsiString Str = "Добавим дефект";
		TPr::pr(Str);
		TEtalonNewDefectForm* EtalonNewDefectForm=new TEtalonNewDefectForm(this);
		EtalonNewDefectForm->SetIni(ini);
		EtalonNewDefectForm->ShowModal();
		delete EtalonNewDefectForm;
		FormActivate(this);
	}
}
//---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::bCopyClick(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		if (DBGridEtalons->SelectedRows->Count > 0)
		{
			// Находим первый выбранный дефект
			QEtalons->First();
			while (!QEtalons->Eof)
			{
				if (DBGridEtalons->SelectedRows->CurrentRowSelected)
					break;
				QEtalons->Next();
			}
			int id = QEtalons->FieldByName("ID")->AsInteger;
			AnsiString Str = "Скопируем дефект для эталона";
			TPr::pr(Str);
			// Открываем форму добавления нового дефекта с заполненными полями
			TEtalonNewDefectForm* EtalonNewDefectForm=new TEtalonNewDefectForm(this);
			EtalonNewDefectForm->SetIni(ini);
			EtalonNewDefectForm->eIDTube->Text
				= QEtalons->FieldByName("IDTube")->AsInteger;
			EtalonNewDefectForm->eTypeSize->Text
				= QEtalons->FieldByName("TypeSize")->AsAnsiString;
			EtalonNewDefectForm->eName->Text
				= QEtalons->FieldByName("Name")->AsAnsiString;
			EtalonNewDefectForm->cbModule->ItemIndex
				= ModuleToInt(QEtalons->FieldByName("Module")->AsAnsiString) - 1;
			EtalonNewDefectForm->cbDefType->ItemIndex
				= QEtalons->FieldByName("DefType")->AsInteger - 1;
			EtalonNewDefectForm->eDefPoint->Text
				= IntToStr(QEtalons->FieldByName("DefPoint")->AsInteger);
			EtalonNewDefectForm->eDefRadius->Text
				= IntToStr(QEtalons->FieldByName("DefRadius")->AsInteger);
			EtalonNewDefectForm->cbTopBorder->ItemIndex
				= QEtalons->FieldByName("isTopBrackBorder")->AsBoolean;
			EtalonNewDefectForm->eTopPercentage->Text
				= IntToStr(QEtalons->FieldByName("TopBorderPercent")->AsInteger);
			EtalonNewDefectForm->cbBottomBorder->ItemIndex
				= QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean;
			EtalonNewDefectForm->eBottomPercentage->Text
				= IntToStr(QEtalons->FieldByName("BottomBorderPercent")->AsInteger);
			EtalonNewDefectForm->ShowModal();
			delete EtalonNewDefectForm;
			FormActivate(this);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsForm::bEditClick(TObject *Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		if (DBGridEtalons->SelectedRows->Count > 0)
		{
			// Находим первый выбранный дефект
			QEtalons->First();
			while (!QEtalons->Eof)
			{
				if (DBGridEtalons->SelectedRows->CurrentRowSelected)
					break;
				QEtalons->Next();
			}
			int id = QEtalons->FieldByName("ID")->AsInteger;
			AnsiString Str = "Отредактируем дефект для эталона";
			TPr::pr(Str);
			// Открываем форму добавления нового дефекта с заполненными полями
			TEtalonNewDefectForm* EtalonNewDefectForm=new TEtalonNewDefectForm(this);
			EtalonNewDefectForm->SetIni(ini);
			EtalonNewDefectForm->PointEdit=true; // Открываем для редактирования
			EtalonNewDefectForm->PointNum=id;
			EtalonNewDefectForm->eIDTube->Text
				= QEtalons->FieldByName("IDTube")->AsInteger;
			EtalonNewDefectForm->eTypeSize->Text
				= QEtalons->FieldByName("TypeSize")->AsAnsiString;
			EtalonNewDefectForm->eName->Text
				= QEtalons->FieldByName("Name")->AsAnsiString;
			EtalonNewDefectForm->SetModuleType(ModuleToInt(QEtalons->FieldByName("Module")->AsAnsiString) - 1,QEtalons->FieldByName("DefType")->AsInteger - 1);
			EtalonNewDefectForm->eDefPoint->Text
				= IntToStr(QEtalons->FieldByName("DefPoint")->AsInteger);
			EtalonNewDefectForm->eDefRadius->Text
				= IntToStr(QEtalons->FieldByName("DefRadius")->AsInteger);
			EtalonNewDefectForm->cbTopBorder->ItemIndex
				= QEtalons->FieldByName("isTopBrackBorder")->AsBoolean;
			EtalonNewDefectForm->eTopPercentage->Text
				= IntToStr(QEtalons->FieldByName("TopBorderPercent")->AsInteger);
			EtalonNewDefectForm->cbBottomBorder->ItemIndex
				= QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean;
			EtalonNewDefectForm->eBottomPercentage->Text
				= IntToStr(QEtalons->FieldByName("BottomBorderPercent")->AsInteger);
			EtalonNewDefectForm->ShowModal();
			delete EtalonNewDefectForm;
			FormActivate(this);
		}
	}
}
//---------------------------------------------------------------------------
int TEtalonTubeDefectsForm::ModuleToInt(AnsiString Module)
{
	if(Module == "Поперечный") return 1;
	if(Module == "Продольный") return 2;
	if(Module == "Толщиномер") return 3;
	TPr::pr("TEtalonTubeDefectsForm::ошибка неизвестного модуля!");
	return 1;
}
//----------------------------------------------------------------------------
