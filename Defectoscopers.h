//---------------------------------------------------------------------------

#ifndef DefectoscopersH
#define DefectoscopersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>
#include <cstdio.h>
//---------------------------------------------------------------------------
class TDefectoscopersForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridDefectoscopers;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalonCheck;
	TADOQuery *QDefectoscopers;
	TDataSource *DataDefectoscopers;
	TPanel *Panel;
	TButton *bAdd;
	TButton *bRefresh;
	TButton *bPasswordChange;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации окна обновляет список активных дефектоскопистов
	void __fastcall FormActivate(TObject *Sender);
	// Выбирает все выведенные на просмотр записи
	void __fastcall SelectAllClick(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Поправляет размеры и положение объектов при изменении размеров окна
	void __fastcall FormResize(TObject *Sender);
	// Удаление выбранных дефектоскопистов (делает их неактивными в БД)
	void __fastcall DeleteDefectoscoper(TObject *Sender);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Обновляет список активных дефектоскопистов
	void __fastcall bRefreshClick(TObject *Sender);
	// Добавляет нового дефектоскописта
	void __fastcall bAddClick(TObject *Sender);
	// Смена пароля
	void __fastcall bPasswordChangeClick(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
public:		// User declarations
	// Конструктор
	__fastcall TDefectoscopersForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TDefectoscopersForm *DefectoscopersForm;
//---------------------------------------------------------------------------
#endif
