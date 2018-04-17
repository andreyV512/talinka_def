//---------------------------------------------------------------------------

#ifndef ViewH
#define ViewH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <AppEvnts.hpp>
#include "Global.h"
#include <Menus.hpp>
#include "uFRViewZone.h"
#include <IniFiles.hpp>
#include <vector.h>

//---------------------------------------------------------------------------

class TViewForm : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *gbCross;
	TGroupBox *gbLinear;
	TChart *ViewLinearChart;
	TChart *ViewCrossChart;
	TBarSeries *BarSeries1;
	TLineSeries *Series2;
	TChart *Chart1;
	TApplicationEvents *ApplicationEvents;
	TBarSeries *Series14;
	TBarSeries *Series15;
	TBarSeries *Series16;
	TBarSeries *Series17;
	TBarSeries *Series18;
	TBarSeries *Series19;
	TBarSeries *Series20;
	TBarSeries *Series1;
	TBarSeries *Series3;
	TBarSeries *Series4;
	TBarSeries *Series5;
	TBarSeries *Series6;
	TBarSeries *Series7;
	TBarSeries *Series8;
	TBarSeries *Series9;
	TBarSeries *Series10;
	TBarSeries *Series11;
	TBarSeries *Series12;
	TBarSeries *Series13;
	TButton *bRevertGain;
	TComboBox *CBStep;
	TLabel *Label1;
	TButton *bRevertStart;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ViewCrossChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
		  int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ViewLinearChartClickSeries(TCustomChart *Sender, TChartSeries *Series,
          int ValueIndex, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall EditKeyPress(TObject *Sender, wchar_t &Key);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ApplicationEventsMessage(tagMSG &Msg, bool &Handled);
	void __fastcall ViewCrossChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ViewLinearChartKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CBStepChange(TObject *Sender);
	void __fastcall bRevertGainClick(TObject *Sender);
	void __fastcall bRevertStartClick(TObject *Sender);


private:	// User declarations
	TFRViewZone* Carc[Globals::max_sensors];	// массив чартов для отображения инфы по датчикам
	TFRViewZone* Larc[Globals::max_sensors];	// массив чартов для отображения инфы по датчикам
	void  ViewFormInit();				// инициализация формы
	TIniFile *ini;
	int CrossPos;						//Начальная позиция Поперечного
	int LinearPos;						//Начальная позиция Продольного
	TColor SelectedColor;                  //Цвет выделеной зоны
	bool GainEnable;
	bool inited;
	void GainChangeL(int _sensor,double _gain);
	void GainChangeC(int _sensor,double _gain);
	int lsensors_a;
	void Calibrate(TFRViewZone* _arc);
	void SetRevertGain();
	void SetStartGain(bool isLinear);
	void RevertStartGain();
	bool isRevertCalibrateAccept;
	std::vector<double> gainRevertCalibrate;
	std::vector<double> gainStartCross;
	std::vector<double> gainStartLinear;
public:		// User declarations
	__fastcall TViewForm(TComponent* Owner);
	bool need_repaint;
};
//---------------------------------------------------------------------------
extern PACKAGE TViewForm *ViewForm;
//---------------------------------------------------------------------------
#endif


