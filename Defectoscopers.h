//---------------------------------------------------------------------------

#ifndef DefectoscopersH
#define DefectoscopersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <IniFiles.hpp>
#include <cstdio.h>
//---------------------------------------------------------------------------
class TDefectoscopersForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridDefectoscopers;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalonCheck;
	TADOQuery *QDefectoscopers;
	TDataSource *DataDefectoscopers;
	TPanel *Panel;
	TButton *bAdd;
	TButton *bRefresh;
	TButton *bPasswordChange;

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ��������� ���� ��������� ������ �������� ����������������
	void __fastcall FormActivate(TObject *Sender);
	// �������� ��� ���������� �� �������� ������
	void __fastcall SelectAllClick(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	void __fastcall FormResize(TObject *Sender);
	// �������� ��������� ���������������� (������ �� ����������� � ��)
	void __fastcall DeleteDefectoscoper(TObject *Sender);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// ��������� ������ �������� ����������������
	void __fastcall bRefreshClick(TObject *Sender);
	// ��������� ������ ���������������
	void __fastcall bAddClick(TObject *Sender);
	// ����� ������
	void __fastcall bPasswordChangeClick(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
public:		// User declarations
	// �����������
	__fastcall TDefectoscopersForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
};
//---------------------------------------------------------------------------
extern PACKAGE TDefectoscopersForm *DefectoscopersForm;
//---------------------------------------------------------------------------
#endif
