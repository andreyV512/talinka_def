//---------------------------------------------------------------------------

#ifndef EtalonDefectsCheckH
#define EtalonDefectsCheckH
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
#include <Dialogs.hpp>
#include <vector>
using namespace std;
//---------------------------------------------------------------------------
class TEtalonDefectsCheckForm : public TForm
{
__published:	// IDE-managed Components
	TADOQuery *QEtalons;
	TPanel *pResultNum;
	TPanel *pResultBit;
	TLabel *lDefectoscoper;
	TEdit *eDate;
	TEdit *eTypeSize;
	TEdit *eEtalonName;
	TLabel *lDate;
	TLabel *lEtalonName;
	TLabel *lTypeSize;
	TButton *bSave;
	TButton *bCancel;
	TSaveDialog *SaveToFileDialog;
	TMemo *mCheckedDefects;
	TComboBox *cbSelectDefectoscoper;

	// ������������� ������� Esc ��� �������� ����
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// �� �������� ���� ��������� ������� ������
	void __fastcall FormShow(TObject *Sender);
	// �������� ����
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// ������
	void __fastcall bCancelClick(TObject *Sender);
	// ���������� ����������� �������� ���
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall cbSelectDefectoscoperChange(TObject *Sender);

private:	// User declarations
	// ���� � �����������
	TIniFile *ini;
	// ������ � �������� ������� �����
	void EtalonCheck();
	// ������ ����� � ���������� �������� �������
	bool DefectCheck(int _Module, int _DefType, int _DefPoint,
		int _DefRadius, bool _isTopBrackBorder, int _TopBorderPercent,
		bool _isBottomBrackBorder, int _BottomBorderPercent);
	// ID ����
	int IDTube;
	// ����� �������� � ����
	int numOfDefects;
	// ����� ��������� ��������
	int matchedDefects;
	// �������� ������� �� ����� � �������� ��������� ��� ����������� �������
	bool CheckDiap(int _Module, int _sensor, bool _isBrackBorder,
		int _borderPercent, int _startPoint, int _endPoint);
	// �������� �� ������������ ���� �������
	bool CheckDefType(int _Module, int _DefType, vector<int> dirtyFind);
	// ���������� ���������� ������ ������� �� ������� ������
	bool AddMemoResult(bool _matched, int _module, int _defPoint);

	// ���������� ������ �� �����
	void SetData();
	// ������ �� �����
	vector<vector<vector<double> > > CrossFilteredOut_Data;
	vector<double> CrossGain;
	vector<vector<bool> > CrossDeads;
	vector<vector<vector<double> > > LinearFilteredOut_Data;
	vector<vector<vector<double> > > LinearFilteredIn_Data;
	vector<double> LinearGain;
	vector<vector<bool> > LinearDeads;
    //��������� ������ �� ������� ��� ����� ������
//	void SaveEtalonDefectsToFile(UnicodeString FileName);
	void SaveEtalonDefectsToFile(FILE *file);

public:		// User declarations
	// �����������
	__fastcall TEtalonDefectsCheckForm(TComponent* Owner);
	// ��������� ���� � �����������
	void SetIni(TIniFile* _ini);
	// ��������� ID ���� ��� ������
	void SetIDTube(int _IDTube);
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectsCheckForm *EtalonDefectsCheckForm;
//---------------------------------------------------------------------------
#endif
