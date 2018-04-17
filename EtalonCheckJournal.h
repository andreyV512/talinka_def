//---------------------------------------------------------------------------

#ifndef EtalonCheckJournalH
#define EtalonCheckJournalH
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
#include <ComCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TEtalonCheckJournalForm : public TForm
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
	TMenuItem *DeleteEtalonCheck;
	TDataSource *DataEtalons;
	TADOQuery *QChecks;
	TDateTimePicker *dtpTo;
	TDateTimePicker *dtpFrom;
	TLabel *lDateFrom;
	TLabel *lDateTo;
	TMenuItem *LoadTube;

	// Кнопка "показать" обновляет список сверок за указанный срок
	void __fastcall ViewClick(TObject *Sender);
	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации окна запрашивает список типоразмеров,
	// устанавливает сроки выдачи на последний день и выдает список записей
	void __fastcall FormActivate(TObject *Sender);
	// Выбирает все выведенные на просмотр записи
	void __fastcall SelectAllClick(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Поправляет размеры и положение объектов при изменении размеров окна
	void __fastcall FormResize(TObject *Sender);
	// Удаление выбранных записей
	void __fastcall DeleteEtalonCheckResult(TObject *Sender);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Загрузка сохраненных данных трубы
	void __fastcall LoadTubeClick(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
public:		// User declarations
	// Конструктор
	__fastcall TEtalonCheckJournalForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
	// Функция для загрузки сохраненной работы
	typedef void(__closure * OnLoadTubeDef)(UnicodeString _fname);
	OnLoadTubeDef OnLoadTube;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonCheckJournalForm *EtalonCheckJournalForm;
//---------------------------------------------------------------------------
#endif
