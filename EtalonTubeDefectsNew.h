//---------------------------------------------------------------------------

#ifndef EtalonTubeDefectsNewH
#define EtalonTubeDefectsNewH
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
#include <IniFiles.hpp>
#include <DB.hpp>
#include <ADODB.hpp>
#include <cstdio.h>
#include <vector>
//---------------------------------------------------------------------------
class TEtalonTubeDefectsNewForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TButton *bAdd;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bAbort;
	TButton *bSave;
	TButton *bCopy;
	TButton *bEdit;

	// �������� ����� ������
	void __fastcall bAddClick(TObject *Sender);
	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// �������� ��� ���������� �� �������� �������
	void __fastcall SelectAllClick(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	void __fastcall FormResize(TObject *Sender);
	// �������� ��������� ��������
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// ������ � �������� ������ ���� �� ����� ��� ���������
	void __fastcall bAbortClick(TObject *Sender);
	// ���������� ������ ��� � ��� ��������
	void __fastcall bSaveClick(TObject *Sender);
	// ���������� ������ ��������� ������
	void __fastcall bCopyClick(TObject *Sender);
	// ������������� ������ ��������� ������
	void __fastcall bEditClick(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;

	int ModuleToInt(AnsiString Module);
public:		// User declarations
	// �����������
	__fastcall TEtalonTubeDefectsNewForm(TComponent* Owner);
	// ��������� �� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ����� ������ ���� ��� �������� ���� �������� �������
	int IDTube;
	// ���������� ������ ����
	AnsiString TypeSize;
	// �������� ������ ����
	AnsiString Name;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonTubeDefectsNewForm *EtalonTubeDefectsNewForm;
//---------------------------------------------------------------------------
#endif
