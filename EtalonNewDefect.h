//---------------------------------------------------------------------------

#ifndef EtalonNewDefectH
#define EtalonNewDefectH
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
//---------------------------------------------------------------------------
class TEtalonNewDefectForm : public TForm
{
__published:	// IDE-managed Components
	TLabel *lModule;
	TLabel *lDefType;
	TLabel *lDefPoint;
	TLabel *lDefRadius;
	TLabel *lTopBorder;
	TEdit *eDefPoint;
	TEdit *eDefRadius;
	TButton *bSave;
	TButton *bAbort;
	TADOQuery *QEtalons;
	TLabel *lName;
	TEdit *eName;
	TLabel *lIDTube;
	TEdit *eIDTube;
	TLabel *lTypeSize;
	TEdit *eTypeSize;
	TGroupBox *gbTube;
	TGroupBox *gbDefect;
	TLabel *lTopPercent;
	TEdit *eTopPercentage;
	TEdit *eBottomPercentage;
	TLabel *lBottomPercent;
	TLabel *lBottomBorder;
	TComboBox *cbModule;
	TComboBox *cbTopBorder;
	TComboBox *cbBottomBorder;
	TComboBox *cbDefType;
	TButton *bManualBorderSet;

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ��������� ���� ������������ � ��
	void __fastcall FormActivate(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ��������� ������
	void __fastcall bSaveClick(TObject *Sender);
	// ������
	void __fastcall bAbortClick(TObject *Sender);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// ����� ���������� ������
	void __fastcall cbModuleChange(TObject *Sender);
	void __fastcall bManualBorderSetClick(TObject *Sender);


private:	// User declarations
	TIniFile *ini;
public:		// User declarations
	// �����������
	__fastcall TEtalonNewDefectForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ������������� �� ������ ������ ��� ��������� �����
	bool PointEdit;
	// ����� �������
	int PointNum;
	// �������� ������ ������ � ��� ������� �� ����� (��� ��������������)
	void SetModuleType(int _DefModule,int _DefType);
//	int IDTube;    //����� ������� ��� �������� ������� ������
//	AnsiString TypeSize;
//	AnsiString Name;

};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonNewDefectForm *EtalonNewDefectForm;
//---------------------------------------------------------------------------
#endif
