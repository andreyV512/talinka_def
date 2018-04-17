//---------------------------------------------------------------------------

#ifndef EtalonDefectNewH
#define EtalonDefectNewH
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
class TEtalonDefectNewForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *LTS;
	TADOQuery *QEtalons;
	TButton *bSave;
	TLabel *lName;
	TEdit *eEtalonName;
	TEdit *eDate;
	TLabel *lDate;
	TEdit *eTypeSize;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации окна заполняет поля и подключается к БД
	void __fastcall FormActivate(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Сохранить новый СОП
	void __fastcall bSaveClick(TObject *Sender);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);


private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
public:		// User declarations
	// Конструктор
	__fastcall TEtalonDefectNewForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);

};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectNewForm *EtalonDefectNewForm;
//---------------------------------------------------------------------------
#endif
