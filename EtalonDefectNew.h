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

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ��������� ���� ��������� ���� � ������������ � ��
	void __fastcall FormActivate(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ��������� ����� ���
	void __fastcall bSaveClick(TObject *Sender);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);


private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
public:		// User declarations
	// �����������
	__fastcall TEtalonDefectNewForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);

};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectNewForm *EtalonDefectNewForm;
//---------------------------------------------------------------------------
#endif
