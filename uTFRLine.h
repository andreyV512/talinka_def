//---------------------------------------------------------------------------

#ifndef uTFRLineH
#define uTFRLineH
//---------------------------------------------------------------------------
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
//---------------------------------------------------------------------------
class TFRLine : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *gbLinearDef;
	TCheckBox *cbLinear;
	TFRBorders *FRBordersLine;
	TPanel *Panel1;
	TChart *Chart1;
	TBarSeries *Series1;
	TBarSeries *Series2;
	TBarSeries *Series3;
	TFRDeads *FRDeads1;
	TBarSeries *Series4;
	TPanel *Panel2;
	TLabel *Label1;
	void __fastcall cbLinearClick(TObject *Sender);
	void __fastcall SeriesDblClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall SeriesClick(TChartSeries *Sender, int ValueIndex, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall Panel1Resize(TObject *Sender);
private:	// User declarations
	void LoadSettings0(void);
	TIniFile* ini;
	int mark_zone;
	int mark_sensor;
	bool mark_is;
	TColor mark_color;
	void OnChangeChild(void);

public:		// User declarations
	__fastcall TFRLine(TComponent* Owner);
	void Clear(void);
	void Init(TIniFile* _ini);
	void LoadSettings(void);
	void SetEnabledR(bool _v);
	inline bool IsLinear(void)
	{
		return(cbLinear->Checked);
	}
	void MarkerSet(int _zone,int _sensor);
	void MarkerOff(void);
	typedef void(__closure * OnClickDef)(int _zone,int _sensor,bool _IsCtrl);
	OnClickDef OnClick;
	typedef void(__closure * OnDblClickDef)(int _zone,int _sensor,bool _IsCtrl);
	OnDblClickDef OnDblClick;

	typedef void(__closure * OnChangeDef)(void);
	OnChangeDef OnChange;
	void SetDefects(int _val);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRLine *FRLine;
//---------------------------------------------------------------------------
#endif
