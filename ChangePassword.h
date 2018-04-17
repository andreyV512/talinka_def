//---------------------------------------------------------------------------

#ifndef ChangePasswordH
#define ChangePasswordH
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
class TChangePasswordForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *lName;
	TLabel *lPassword;
	TEdit *eName;
	TEdit *ePassword;
	TButton *bChange;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации
	void __fastcall FormActivate(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Подтверждение изменения пароля
	void __fastcall bChangeClick(TObject *Sender);


private:	// User declarations
	// Файл с настройками
	TIniFile *ini;

public:		// User declarations
	// Конструктор
	__fastcall TChangePasswordForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TChangePasswordForm *ChangePasswordForm;
//---------------------------------------------------------------------------
#endif
