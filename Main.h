// ---------------------------------------------------------------------------

#ifndef MainH
#define MainH
// ---------------------------------------------------------------------------

#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <TeeProcs.hpp>
#include <ExtCtrls.hpp>
#include <Series.hpp>
#include <Dialogs.hpp>
#include <AppEvnts.hpp>
#include <XPMan.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Chart.hpp>
#include <TeEngine.hpp>
#include "uTFRBorders.h"
#include "uTFRCross.h"
#include "uTFRLine.h"
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include "uTFRThick.h"
#include "uTFRSum.h"
#include "TeCanvas.hpp"
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <ActnMenus.hpp>
#include <PlatformDefaultStyleActnCtrls.hpp>
#include <ToolWin.hpp>
#include "uFRStatist.h"
#include "uTFRSOP.h"
#include <vector>
#include "CBank.h"
class ThreadOnLine;

// ---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published: // IDE-managed Components

	TButton *bTest;
	TButton *bWork;
	TButton *bView;
	TButton *bManage;
	TButton *bCancelWork;
	TStatusBar *StatusBarBottom;
	TStatusBar *StatusBarTop;
	TCheckBox *cbInterruptView;
	TSaveDialog *SaveToFileDialog;
	TOpenDialog *OpenDialogFromFile;
	TComboBox *cbTypeSize;
	TApplicationEvents *ApplicationEvents;
	TPopupMenu *OtherTest;
	TMenuItem *TestRotation;
	TXPManifest *XPManifest1;
	TFRCross *FRCross1;
	TFRLine *FRLine1;
	TFRThick *FRThick1;
	TFRSum *FRSum1;
	TButton *Button1;
	TActionManager *ActionManager1;
	TAction *Action1;
	TAction *Action2;
	TAction *Action3;
	TAction *Action4;
	TAction *Action5;
	TAction *Action7;
	TAction *Action8;
	TAction *Action9;
	TAction *Action10;
	TAction *Action11;
	TAction *Action12;
	TAction *Action13;
	TAction *Action14;
	TAction *Action15;
	TAction *Action16;
	TAction *Action17;
	TAction *Action18;
	TAction *Action19;
	TAction *Action20;
	TAction *Action21;
	TActionMainMenuBar *ActionMainMenuBar1;
	TAction *Action6;
	TFRStatist *FRStatist1;
	TAction *Action22;
	TAction *Action23;
	TAction *Action24;
	TAction *Action25;
	TFRSOP *FRSOP1;
	TPanel *WinWorkPanel;
	TLabel *lWinWorkMessage;
	TTimer *WinWorkTimer;
	TButton *bWinWorkOk;
	TButton *ExitTube; // очищает все графики

	void __fastcall MainFormInit(TObject *Sender);
	// выравнивает объекты на форме, работает в начале работы и при Resize
	// стандартные функции
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyPress(TObject *Sender, wchar_t &Key);
	// нажатие пунктов главного меню
	void __fastcall menuTypeSizeClick(TObject *Sender);
	void __fastcall menuMeasuresClick(TObject *Sender);
	void __fastcall menuSignalsStateClick(TObject *Sender);
	void __fastcall menuSaveTubeClick(TObject *Sender);
	void __fastcall menuLoadTubeClick(TObject *Sender);
	void __fastcall menuTestAdvantechClick(TObject *Sender);
	void __fastcall menuF1Click(TObject *Sender);
	void __fastcall menuProtocolClick(TObject *Sender);
	void __fastcall menuColorsClick(TObject *Sender);
	// нажатие кнопок на форме
	void __fastcall bTestClick(TObject *Sender);
	void __fastcall bViewClick(TObject *Sender);
	void __fastcall bWorkClick(TObject *Sender);
	void __fastcall bCancelWorkClick(TObject *Sender);
	void __fastcall bManageClick(TObject *Sender);
	void __fastcall cbTypeSizeSelect(TObject *Sender);
	void __fastcall eSettingsChange(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall bManualSGClick(TObject *Sender);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Action22Execute(TObject *Sender);
	void __fastcall Action23Execute(TObject *Sender);
	void __fastcall Action24Execute(TObject *Sender);
	void __fastcall Action25Execute(TObject *Sender);
	void __fastcall FRSOP1cbEtalonViewClick(TObject *Sender);
	void __fastcall WinWorkTimerTimer(TObject *Sender);
	void __fastcall bWinWorkOkClick(TObject *Sender);
	void __fastcall ExitTubeClick(TObject *Sender);

private: // User declarations

	// настройки типоразмеров
	void LoadSettings(void);
	// читает настройки из ini файла, заполняет едиты, устанавли пороги, мертвые зоны

	TIniFile *ini; // ини файл с настройками

	void UpdateComboBox();
	// обновляет ComboBox с типоразмерами (после добавления или удаления, в начале работы)

	// ЛИР
	ThreadOnLine *workonline; // тред отрисовки в реальном времени

	// блокировка
public:
	void SetAbleButtons(bool state);
private:
	// блокировка/разблокировка клавиш
	void SetProjectSpecialty(); // устанавливает характеристики проекта

//	bool GoodTube; // оператор меняет результат принудительно на Годно
//	bool BadTube; // оператор меняет результат на Брак
	// костыли ГП Пыть-ях
	// int manual_num;

	void ReadFromFile(UnicodeString path);
	// для реализации Drag & Drop
	void __fastcall WmDropFiles(TWMDropFiles& Message);
	void __fastcall ReadFile(AnsiString FileName);
	BEGIN_MESSAGE_MAP MESSAGE_HANDLER(WM_DROPFILES, TWMDropFiles, WmDropFiles)
		END_MESSAGE_MAP(TForm);

	void Post(void);

	UINT set_msg;
	UINT thread_msg;
	UINT sms_msg;
	UINT spectro_msg;
	bool isView;
	CBank* BankLine;
	CBank* BankCross;
	bool ComWithASU;

	void ReDraw(void);
	void pr(AnsiString _msg);

//	AnsiString tube_number;
	void UpdateStatus(AnsiString _text1, AnsiString _text2);
	void ClearCharts(void);
	void ReCalcSG(void);
	void Start(void);
	void InteruptView(void);
	void InteruptSG(void);
	void ReStart(void);
	void Stop(void);
	bool isInteruptSG;
	void BlockMainMenu(bool _state);
	vector <double> SGbuffer;
	int CSensors;
	int LSensors;
	//Критическое время работы Виндоус, для выдачи предложения перезагрузки
	int MuchWinWorkTime;

public: // User declarations
       HANDLE hEvent;
	__fastcall TMainForm(TComponent* Owner);
};

// ---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
// ---------------------------------------------------------------------------
#endif
