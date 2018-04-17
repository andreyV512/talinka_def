// ---------------------------------------------------------------------------

#ifndef uTFRCrossH
#define uTFRCrossH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "Series.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include "uTFRBorders.h"
#include <ExtCtrls.hpp>
#include "uTFRDeads.h"

// ---------------------------------------------------------------------------
class TFRCross : public TFrame
{
__published: // IDE-managed Components

	TGroupBox *gbCrossDef;
	TFRBorders *FRBordersCross;
	TChart *Chart1;
	TPanel *Panel1;
	TBarSeries *Series1;
	TBarSeries *Series2;
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
	TFRDeads *FRDeads1;
	TLabel *Label1;
	TPanel *Panel2;

	void __fastcall SeriesDblClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall SeriesClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall Panel1Resize(TObject *Sender);

private: // User declarations
	void OnChangeChild(void);
	TIniFile* ini;
	int mark_zone;
	int mark_sensor;
	bool mark_is;
	TColor mark_color;
public: // User declarations
	typedef void(__closure * OnChangeDef)(void);
	__fastcall TFRCross(TComponent* Owner);
	void Clear(void);
	void Init(TIniFile* _ini);
	void LoadSettings(void);
	OnChangeDef OnChange;
	void SetEnabledR(bool _v);
	void MarkerSet(int _zone,int _sensor);
	void MarkerOff(void);
	typedef void(__closure * OnClickDef)(int _zone,int _sensor,bool _IsCtrl);
	OnClickDef OnClick;
	typedef void(__closure * OnDblClickDef)(int _zone,int _sensor,bool _IsCtrl);
	OnDblClickDef OnDblClick;
	void SetDefects(int _val);
};

// ---------------------------------------------------------------------------
extern PACKAGE TFRCross *FRCross;
// ---------------------------------------------------------------------------
#endif
