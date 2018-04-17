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

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// ������
	void __fastcall bCancelClick(TObject *Sender);
	// ������������� ����� ������
	void __fastcall bOKClick(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
	// ��� ���������������
	AnsiString UserName;

public:		// User declarations
	// �����������
	__fastcall TPasswordForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ��������� ��� ���������������
	void SetUserName(AnsiString _UserName);
	// ���������� ������
	AnsiString Encoder(AnsiString Password);
};
//---------------------------------------------------------------------------
extern PACKAGE TPasswordForm *PasswordForm;
//---------------------------------------------------------------------------
#endif
