//---------------------------------------------------------------------------

#ifndef PasswordH
#define PasswordH
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
class TPasswordForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *lPassword;
	TEdit *ePassword;
	TButton *bOK;
	TButton *bCancel;
	TADOQuery *QPassword;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Отмена
	void __fastcall bCancelClick(TObject *Sender);
	// Подтверждение ввода пароля
	void __fastcall bOKClick(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
	// Имя дефектоскописта
	AnsiString UserName;

public:		// User declarations
	// Конструктор
	__fastcall TPasswordForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
	// Указывает имя дефектоскописта
	void SetUserName(AnsiString _UserName);
	// Шифрование пароля
	AnsiString Encoder(AnsiString Password);
};
//---------------------------------------------------------------------------
extern PACKAGE TPasswordForm *PasswordForm;
//---------------------------------------------------------------------------
#endif
