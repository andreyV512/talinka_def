//---------------------------------------------------------------------------

#ifndef EtalonDefectH
#define EtalonDefectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <DBCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <IniFiles.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <cstdio.h>
#include <vector>
//---------------------------------------------------------------------------
class TEtalonDefectForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TLabel *LTS;
	TComboBox *cbTypeSize;
	TButton *View;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *ViewEtalon;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;  //Надо?
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bCheck;

	// Кнопка "показать" выдает список СОП указанного типоразмера
	void __fastcall ViewClick(TObject *Sender);
	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации окна запрашивает список типоразмеров и выдает список СОП
	void __fastcall FormActivate(TObject *Sender);
	// Просмотр списка дефектов для выбранных СОПов
	void __fastcall ViewEtalonTubeDefects(TObject *Sender);
	// Выбирает все выведенные на просмотр записи
	void __fastcall SelectAllClick(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Поправляет размеры и положение объектов при изменении размеров окна
	void __fastcall FormResize(TObject *Sender);
	// Удаление выбранных СОПов
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// Сверка текущей трубы с выбранным СОПом
	void __fastcall bCheckClick(TObject *Sender);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
public:		// User declarations
	// Конструктор
	__fastcall TEtalonDefectForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectForm *EtalonDefectForm;
//---------------------------------------------------------------------------
#endif
