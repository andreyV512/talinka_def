// ---------------------------------------------------------------------------

#ifndef ADCSettingsH
#define ADCSettingsH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "cspin.h"
#include "IniFiles.hpp"
#include <AppEvnts.hpp>
#include "Global.h"
#include "Filters.h"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "uTFilterSettings.h"
#include "uTFRSensor.h"
#include <vector>
#include <Registry.hpp>
#include "uCRListT.h"


// ---------------------------------------------------------------------------
class TADCSettForm : public TForm
{
__published: // IDE-managed Components

	TApplicationEvents *ApplicationEvents;
	TPageControl *PageControl;
	TTabSheet *ppPage;
	TGroupBox *gbPP;
	TGroupBox *gbCross;
	TLabel *lnumberChannel;
	TLabel *lamperage;
	TLabel *lTemp;
	TCSpinEdit *spSolenoid1;
	TCSpinEdit *spSolenoid2;
	TTabSheet *OtherPage;
	TCSpinEdit *SpinCurr;
	TCSpinEdit *SpinSG;
	TPopupMenu *CopyValueCrossChannel;
	TMenuItem *CopyAllSensor;
	TPopupMenu *CopyValueLinearChannel;
	TMenuItem *CopyAllSensorLinear;
	TTabSheet *FilterPAge;
	TCheckBox *cbMedianFilter;
	TLabel *lWidthMedianFilter;
	TComboBox *ComboBoxWidthMF;
	TTabSheet *prPage;
	TPanel *pBottom;
	TButton *bSave;
	TButton *bExit;
	TEdit *eFrequency;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *Label2;
	TEdit *ppRate;
	TEdit *ppKadr;
	TGroupBox *gbPR;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label4;
	TLabel *Label5;
	TFilterSettings *FSLineOut;
	TFilterSettings *FSLineIn;
	TFilterSettings *FSCross;
	TCSpinEdit *spIsSolenoidsON;
	TLabel *Label6;
	TEdit *edSolenoidTresholdU;
	TLabel *Label7;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall bSaveClick(TObject *Sender);
	void __fastcall bExitClick(TObject *Sender);
	void __fastcall eThresholdResistKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall CopyAllSensorClick(TObject *Sender);
	void __fastcall ContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall CopyAllSensorLinearClick(TObject *Sender);

private:
	// User declarations
	int LSensors;
	int CSensors;
	CRListT<TFRSensor> CS;
	CRListT<TFRSensor> LS;

	int LinearItemIndex;
	int CrossItemIndex;
	TIniFile *ini;

	void block();

	bool fl;

	void FillAllControls();

public: // User declarations
	__fastcall TADCSettForm(TComponent* Owner);
	bool need_redraw;
//	void SaveTubeSettingsToFile(UnicodeString FileName);
	void SaveTubeSettingsToFile(FILE *file);

};

// ---------------------------------------------------------------------------
extern PACKAGE TADCSettForm *ADCSettForm;
// ---------------------------------------------------------------------------
#endif
