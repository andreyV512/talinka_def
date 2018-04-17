//---------------------------------------------------------------------------

#ifndef EtalonDefectH
#define EtalonDefectH
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
class TEtalonDefectForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TLabel *LTS;
	TComboBox *cbTypeSize;
	TButton *View;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *ViewEtalon;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;  //����?
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bCheck;

	// ������ "��������" ������ ������ ��� ���������� �����������
	void __fastcall ViewClick(TObject *Sender);
	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ��������� ���� ����������� ������ ������������ � ������ ������ ���
	void __fastcall FormActivate(TObject *Sender);
	// �������� ������ �������� ��� ��������� �����
	void __fastcall ViewEtalonTubeDefects(TObject *Sender);
	// �������� ��� ���������� �� �������� ������
	void __fastcall SelectAllClick(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	void __fastcall FormResize(TObject *Sender);
	// �������� ��������� �����
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// ������ ������� ����� � ��������� �����
	void __fastcall bCheckClick(TObject *Sender);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
public:		// User declarations
	// �����������
	__fastcall TEtalonDefectForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectForm *EtalonDefectForm;
//---------------------------------------------------------------------------
#endif
