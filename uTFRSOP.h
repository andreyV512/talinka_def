//---------------------------------------------------------------------------

#ifndef uTFRSOPH
#define uTFRSOPH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
class TFRSOP : public TFrame
{
__published:	// IDE-managed Components
	TCheckBox *cbEtalonView;
	TComboBox *cbSelectEtalon;
	TLabel *lEtalon;
	void __fastcall cbSelectEtalonDropDown(TObject *Sender);
	void __fastcall cbSelectEtalonChange(TObject *Sender);
	void __fastcall cbEtalonViewClick(TObject *Sender);
private:	// User declarations
	TIniFile* ini;
public:		// User declarations
	__fastcall TFRSOP(TComponent* Owner);
	void Init(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRSOP *FRSOP;
//---------------------------------------------------------------------------
#endif
