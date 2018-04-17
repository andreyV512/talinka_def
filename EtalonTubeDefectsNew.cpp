//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonTubeDefectsNew.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "EtalonNewDefect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonTubeDefectsNewForm *EtalonTubeDefectsNewForm;
//---------------------------------------------------------------------------
__fastcall TEtalonTubeDefectsNewForm::TEtalonTubeDefectsNewForm(TComponent* Owner)	: TForm(Owner)
{
	KeyPreview = true;
}
//---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::bAddClick(TObject *Sender)
{
	// Вызовем форму для добавления нового дефекта
	AnsiString Str = "Добавим дефект для нового эталона";
	TPr::pr(Str);
	TEtalonNewDefectForm* EtalonNewDefectForm=new TEtalonNewDefectForm(this);
	EtalonNewDefectForm->SetIni(ini);
	EtalonNewDefectForm->eIDTube->Text = IDTube;
	EtalonNewDefectForm->eTypeSize->Text = TypeSize;
	EtalonNewDefectForm->eName->Text = Name;
	EtalonNewDefectForm->ShowModal();
	delete EtalonNewDefectForm;
	QEtalons->Active = false;
	QEtalons->Active = true;
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::FormKeyPress(TObject *Sender, wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}
//----------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	// Выведем на экран список дефектов для СОПа
	AnsiString Str = "Покажем дефекты нового эталона";
	TPr::pr(Str);
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
//	SQL = "SELECT * FROM dbo.EtalonTubeDefects ";
	SQL += "where EtalonTubeDefects.IDTube='" + IntToStr(IDTube) + "'";
	SQL += " order by EtalonTubeDefects.id";
	QEtalons->SQL->Add(SQL);
	QEtalons->ExecSQL();
	QEtalons->Active = true;
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::SelectAllClick(TObject *Sender)
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
void TEtalonTubeDefectsNewForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}
//----------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::FormResize(TObject *Sender)
{
	// Поправляет размеры и положение объектов при изменении размеров окна
	DBNavigatorEtalons->Top = ClientHeight - DBNavigatorEtalons->Height - 3;
	DBGridEtalons->Height = DBNavigatorEtalons->Top - 2 - DBGridEtalons->Top;
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::DeleteEtalonDefect(TObject *Sender)
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
	QEtalons->Active=false;
	QEtalons->Active=true;
}
//---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::bAbortClick(TObject *Sender)
{
	// Предупреждение
	if (Application->MessageBoxW(L"Удалить все записи о новом СОП?",
		L"Новый СОП", MB_YESNO) == IDNO)
		return ;

	// Удаляем дефекты
	AnsiString SQL;
	SQL = "delete from dbo.EtalonTubeDefects where IDTube=";
	SQL += IDTube;
	TPr::pr(SQL);
	CExecSQL E(SQL);
	// Удаляем СОП
	SQL = "delete from dbo.EtalonDefect where ID=";
	SQL += IDTube;
	TPr::pr(SQL);
	CExecSQL EE(SQL);
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::bSaveClick(TObject *Sender)
{
	// Закрытие
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TEtalonTubeDefectsNewForm::bCopyClick(TObject *Sender)
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
		AnsiString Str = "Скопируем дефект для нового эталона";
		TPr::pr(Str);
		// Открываем форму добавления нового дефекта с заполненными полями
		TEtalonNewDefectForm* EtalonNewDefectForm=new TEtalonNewDefectForm(this);
		EtalonNewDefectForm->SetIni(ini);
		EtalonNewDefectForm->eIDTube->Text = IDTube;
		EtalonNewDefectForm->eTypeSize->Text = TypeSize;
		EtalonNewDefectForm->eName->Text = Name;
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
		QEtalons->Active=false;
		QEtalons->Active=true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TEtalonTubeDefectsNewForm::bEditClick(TObject *Sender)
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
		EtalonNewDefectForm->PointEdit=true;
		EtalonNewDefectForm->PointNum=id;
		EtalonNewDefectForm->eIDTube->Text = IDTube;
		EtalonNewDefectForm->eTypeSize->Text = TypeSize;
		EtalonNewDefectForm->eName->Text = Name;
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
		QEtalons->Active=false;
		QEtalons->Active=true;
	}
}
//---------------------------------------------------------------------------
int TEtalonTubeDefectsNewForm::ModuleToInt(AnsiString Module)
{
	if(Module == "Поперечный") return 1;
	if(Module == "Продольный") return 2;
	if(Module == "Толщиномер") return 3;
	TPr::pr("TEtalonTubeDefectsForm::ошибка неизвестного модуля!");
	return 1;
}
//----------------------------------------------------------------------------
