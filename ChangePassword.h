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

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ���������
	void __fastcall FormActivate(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// ������������� ��������� ������
	void __fastcall bChangeClick(TObject *Sender);


private:	// User declarations
	// ���� � �����������
	TIniFile *ini;

public:		// User declarations
	// �����������
	__fastcall TChangePasswordForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TChangePasswordForm *ChangePasswordForm;
//---------------------------------------------------------------------------
#endif
