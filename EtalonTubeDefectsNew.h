//---------------------------------------------------------------------------

#ifndef EtalonTubeDefectsNewH
#define EtalonTubeDefectsNewH
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
#include <IniFiles.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <cstdio.h>
#include <vector>
//---------------------------------------------------------------------------
class TEtalonTubeDefectsNewForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TButton *bAdd;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bAbort;
	TButton *bSave;
	TButton *bCopy;
	TButton *bEdit;

	// Добавить новый дефект
	void __fastcall bAddClick(TObject *Sender);
	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Выбирает все выведенные на просмотр дефекты
	void __fastcall SelectAllClick(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Поправляет размеры и положение объектов при изменении размеров окна
	void __fastcall FormResize(TObject *Sender);
	// Удаление выбранных дефектов
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// Отмена и удаление нового СОПа со всеми его дефектами
	void __fastcall bAbortClick(TObject *Sender);
	// Сохранение нового СОП и его дефектов
	void __fastcall bSaveClick(TObject *Sender);
	// Копировать первый выбранный дефект
	void __fastcall bCopyClick(TObject *Sender);
	// Редактировать первый выбранный дефект
	void __fastcall bEditClick(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;

	int ModuleToInt(AnsiString Module);
public:		// User declarations
	// Конструктор
	__fastcall TEtalonTubeDefectsNewForm(TComponent* Owner);
	// Указываем на файл с настройками
	void SetIni(TIniFile* _ini);
	// Номер нового СОПа для которого надо показать дефекты
	int IDTube;
	// Типоразмер нового СОПа
	AnsiString TypeSize;
	// Название нового СОПа
	AnsiString Name;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonTubeDefectsNewForm *EtalonTubeDefectsNewForm;
//---------------------------------------------------------------------------
#endif
