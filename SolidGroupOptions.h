//---------------------------------------------------------------------------

#ifndef SolidGroupOptionsH
#define SolidGroupOptionsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSolidGroupOptionsForm : public TForm
{
__published:	// IDE-managed Components
	TCheckBox *cbSolidGroupSwitch;
	TComboBox *cboxSelectGroup;
	TLabel *Label1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall cbSolidGroupSwitchClick(TObject *Sender);
	void __fastcall cboxSelectGroupChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TSolidGroupOptionsForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSolidGroupOptionsForm *SolidGroupOptionsForm;
//---------------------------------------------------------------------------
#endif
