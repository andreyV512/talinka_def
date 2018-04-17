//---------------------------------------------------------------------------

#ifndef EtalonCheckJournalH
#define EtalonCheckJournalH
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
#include <ComCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TEtalonCheckJournalForm : public TForm
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
	TMenuItem *DeleteEtalonCheck;
	TDataSource *DataEtalons;
	TADOQuery *QChecks;
	TDateTimePicker *dtpTo;
	TDateTimePicker *dtpFrom;
	TLabel *lDateFrom;
	TLabel *lDateTo;
	TMenuItem *LoadTube;

	// ������ "��������" ��������� ������ ������ �� ��������� ����
	void __fastcall ViewClick(TObject *Sender);
	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� ��������� ���� ����������� ������ ������������,
	// ������������� ����� ������ �� ��������� ���� � ������ ������ �������
	void __fastcall FormActivate(TObject *Sender);
	// �������� ��� ���������� �� �������� ������
	void __fastcall SelectAllClick(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ���������� ������� � ��������� �������� ��� ��������� �������� ����
	void __fastcall FormResize(TObject *Sender);
	// �������� ��������� �������
	void __fastcall DeleteEtalonCheckResult(TObject *Sender);
	// �������� ����
	void __fastcall FormShow(TObject *Sender);
	// �������� ����������� ������ �����
	void __fastcall LoadTubeClick(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
public:		// User declarations
	// �����������
	__fastcall TEtalonCheckJournalForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ������� ��� �������� ����������� ������
	typedef void(__closure * OnLoadTubeDef)(UnicodeString _fname);
	OnLoadTubeDef OnLoadTube;
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonCheckJournalForm *EtalonCheckJournalForm;
//---------------------------------------------------------------------------
#endif
