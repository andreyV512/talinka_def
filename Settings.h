//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include "IniFiles.hpp"


//---------------------------------------------------------------------------
class TSettingsForm : public TForm
{
__published:	// IDE-managed Components
	TValueListEditor *ValueListEditor;
	TStatusBar *StatusBarBottom;
	TComboBox *cbTypeSize;
	TButton *bCreateTypeSize;
	TButton *bDeleteTypeSize;
	void __fastcall ValueListKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall bCreateTypeSizeClick(TObject *Sender);
	void __fastcall bDeleteTypeSizeClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ValueListEditorValidate(TObject *Sender, int ACol, int ARow, const UnicodeString KeyName,
		  const UnicodeString KeyValue);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ValueListEditorGetEditText(TObject *Sender, int ACol, int ARow,
          UnicodeString &Value);

private:	// User declarations
	TIniFile *ini;					// ini файл с настройками типоразмеров
	TStringList *names;				// имена настроек, которые необходимо и писать в ini файл
	bool validate(void);
	void LoadTypeSize(AnsiString _typeSize);
	void SaveTypeSize(AnsiString _typeSize);
	void DeleteTypeSize(AnsiString _typeSize);
	AnsiString typeSizeCur;
public:		// User declarations

	__fastcall TSettingsForm(TComponent* Owner,TIniFile* _ini);
	bool need_loadsettings;
};
//---------------------------------------------------------------------------
extern PACKAGE TSettingsForm *SettingsForm;
//---------------------------------------------------------------------------
#endif
