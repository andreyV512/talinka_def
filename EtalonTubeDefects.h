//---------------------------------------------------------------------------

#ifndef EtalonTubeDefectsH
#define EtalonTubeDefectsH
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
class TEtalonTubeDefectsForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TLabel *LTS;
	TComboBox *cbTypeSize;
	TButton *View;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bAdd;
	TButton *bCopy;
	TButton *bEdit;

	//Показывает контрольные точки для СОПов выбранного типоразмера
	void __fastcall ViewClick(TObject *Sender);
	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// По активации окна запрашивает список типоразмеров,
	// и выдает список дефектов для заданного СОПа
	void __fastcall FormActivate(TObject *Sender);
	// Выбирает все выведенные на просмотр дефекты
	void __fastcall SelectAllClick(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Поправляет размеры и положение объектов при изменении размеров окна
	void __fastcall FormResize(TObject *Sender);
	// Удаление выбранных дефектов
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// Добавить новый дефект
	void __fastcall bAddClick(TObject *Sender);
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
	__fastcall TEtalonTubeDefectsForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
	// Номера эталонов для которых надо показать дефекты
	std::vector <int> IndexField;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonTubeDefectsForm *EtalonTubeDefectsForm;
//---------------------------------------------------------------------------
#endif
