//---------------------------------------------------------------------------

#ifndef EtalonNewDefectH
#define EtalonNewDefectH
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
//---------------------------------------------------------------------------
class TEtalonNewDefectForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *lModule;
	TLabel *lDefType;
	TLabel *lDefPoint;
	TLabel *lDefRadius;
	TLabel *lTopBorder;
	TEdit *eDefPoint;
	TEdit *eDefRadius;
	TButton *bSave;
	TButton *bAbort;
	TADOQuery *QEtalons;
	TLabel *lName;
	TEdit *eName;
	TLabel *lIDTube;
	TEdit *eIDTube;
	TLabel *lTypeSize;
	TEdit *eTypeSize;
	TGroupBox *gbTube;
	TGroupBox *gbDefect;
	TLabel *lTopPercent;
	TEdit *eTopPercentage;
	TEdit *eBottomPercentage;
	TLabel *lBottomPercent;
	TLabel *lBottomBorder;
	TComboBox *cbModule;
	TComboBox *cbTopBorder;
	TComboBox *cbBottomBorder;
	TComboBox *cbDefType;
	TButton *bManualBorderSet;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации окна подключается к БД
	void __fastcall FormActivate(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Сохраняем дефект
	void __fastcall bSaveClick(TObject *Sender);
	// Отмена
	void __fastcall bAbortClick(TObject *Sender);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Смена выбранного модуля
	void __fastcall cbModuleChange(TObject *Sender);
	void __fastcall bManualBorderSetClick(TObject *Sender);


private:	// User declarations
	TIniFile *ini;
public:		// User declarations
	// Конструктор
	__fastcall TEtalonNewDefectForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
	// Редактируется ли старый дефект или создается новый
	bool PointEdit;
	// Номер дефекта
	int PointNum;
	// Выбирает нужный модуль и тип дефекта на форме (при редактировании)
	void SetModuleType(int _DefModule,int _DefType);
//	int IDTube;    //номер эталона для которого создаем дефект
//	AnsiString TypeSize;
//	AnsiString Name;

};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonNewDefectForm *EtalonNewDefectForm;
//---------------------------------------------------------------------------
#endif
