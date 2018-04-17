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

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ���������
	void __fastcall FormActivate(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// ������������� ������ ���������������
	void __fastcall bChooseClick(TObject *Sender);


private:	// User declarations
	// ���� � �����������
	TIniFile *ini;

public:		// User declarations
	// �����������
	__fastcall TAuthorisationForm(TComponent* Owner, TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TAuthorisationForm *AuthorisationForm;
//---------------------------------------------------------------------------
#endif
