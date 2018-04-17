//---------------------------------------------------------------------------

#ifndef EtalonTubeDefectsH
#define EtalonTubeDefectsH
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
class TEtalonTubeDefectsForm : public TForm
{
__published:	// IDE-managed Components
	TDBGrid *DBGridEtalons;
	TPanel *Panel;
	TLabel *LTS;
	TComboBox *cbTypeSize;
	TButton *View;
	TDBNavigator *DBNavigatorEtalons;
	TPopupMenu *pmExport;
	TMenuItem *SelectAll;
	TMenuItem *DeleteEtalon;
	TDataSource *DataEtalons;
	TADOQuery *QEtalons;
	TButton *bAdd;
	TButton *bCopy;
	TButton *bEdit;

	//���������� ����������� ����� ��� ����� ���������� �����������
	void __fastcall ViewClick(TObject *Sender);
	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// �� ��������� ���� ����������� ������ ������������,
	// � ������ ������ �������� ��� ��������� ����
	void __fastcall FormActivate(TObject *Sender);
	// �������� ��� ���������� �� �������� �������
	void __fastcall SelectAllClick(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	void __fastcall FormResize(TObject *Sender);
	// �������� ��������� ��������
	void __fastcall DeleteEtalonDefect(TObject *Sender);
	// �������� ����� ������
	void __fastcall bAddClick(TObject *Sender);
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
	__fastcall TEtalonTubeDefectsForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ������ �������� ��� ������� ���� �������� �������
	std::vector <int> IndexField;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonTubeDefectsForm *EtalonTubeDefectsForm;
//---------------------------------------------------------------------------
#endif
