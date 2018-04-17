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

	// Контроллирует нажатие Esc для закрытия окна
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// По открытию окна запускает процесс сверки
	void __fastcall FormShow(TObject *Sender);
	// Закрытие окна
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	// Отмена
	void __fastcall bCancelClick(TObject *Sender);
	// Сохранение результатов контроля СОП
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall cbSelectDefectoscoperChange(TObject *Sender);

private:	// User declarations
	// Файл с настройками
	TIniFile *ini;
	// Сверка с эталоном текущей трубы
	void EtalonCheck();
	// сверка трубы с конкретным дефектом эталона
	bool DefectCheck(int _Module, int _DefType, int _DefPoint,
		int _DefRadius, bool _isTopBrackBorder, int _TopBorderPercent,
		bool _isBottomBrackBorder, int _BottomBorderPercent);
	// ID СОПа
	int IDTube;
	// Число дефектов в СОПе
	int numOfDefects;
	// Число совпавших дефектов
	int matchedDefects;
	// Проверка сигнала по трубе в заданном диапозоне для конкретного датчика
	bool CheckDiap(int _Module, int _sensor, bool _isBrackBorder,
		int _borderPercent, int _startPoint, int _endPoint);
	// Проверка на соответствие типу дефекта
	bool CheckDefType(int _Module, int _DefType, vector<int> dirtyFind);
	// Добавление результата сверки дефекта на боковую панель
	bool AddMemoResult(bool _matched, int _module, int _defPoint);

	// Подготовка данных по трубе
	void SetData();
	// Данные по трубе
	vector<vector<vector<double> > > CrossFilteredOut_Data;
	vector<double> CrossGain;
	vector<vector<bool> > CrossDeads;
	vector<vector<vector<double> > > LinearFilteredOut_Data;
	vector<vector<vector<double> > > LinearFilteredIn_Data;
	vector<double> LinearGain;
	vector<vector<bool> > LinearDeads;
    //сохраняет данные по эталону для файла сверки
//	void SaveEtalonDefectsToFile(UnicodeString FileName);
	void SaveEtalonDefectsToFile(FILE *file);

public:		// User declarations
	// Конструктор
	__fastcall TEtalonDefectsCheckForm(TComponent* Owner);
	// Указывает файл с настройками
	void SetIni(TIniFile* _ini);
	// Указывает ID СОПа для сверки
	void SetIDTube(int _IDTube);
};
//---------------------------------------------------------------------------
extern PACKAGE TEtalonDefectsCheckForm *EtalonDefectsCheckForm;
//---------------------------------------------------------------------------
#endif
