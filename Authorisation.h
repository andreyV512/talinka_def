//---------------------------------------------------------------------------

#ifndef AuthorisationH
#define AuthorisationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <IniFiles.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <cstdio.h>
#include <vector>
//---------------------------------------------------------------------------
class TAuthorisationForm : public TForm
{
__published:	// IDE-managed Components
	TComboBox *cbSelectDefectoscoper;
	TLabel *lName;
	TButton *bChoose;
	TADOQuery *QDefectoscoper;

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По активации
	void __fastcall FormActivate(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Открытие окна
	void __fastcall FormShow(TObject *Sender);
	// Подтверждение выбора дефектоскописта
	void __fastcall bChooseClick(TObject *Sender);


private:	// User declarations
	// Файл с настройками
	TIniFile *ini;

public:		// User declarations
	// Конструктор
	__fastcall TAuthorisationForm(TComponent* Owner, TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TAuthorisationForm *AuthorisationForm;
//---------------------------------------------------------------------------
#endif
