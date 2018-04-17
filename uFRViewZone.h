//---------------------------------------------------------------------------

#ifndef uFRViewZoneH
#define uFRViewZoneH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include "Series.hpp"
#include "ArrowCha.hpp"
#include "cspin.h"
#include <Menus.hpp>pp"
#include <Menus.hpp>
#include "cspin.h"

// ---------------------------------------------------------------------------
class TFRViewZone : public TFrame
{
__published:	// IDE-managed Components
	TChart *Chart1;
	TBarSeries *BarOut;
	TLineSeries *Out1;
	TLineSeries *Out2;
	TEdit *Edit1;
	TBarSeries *BarIn;
	TLineSeries *In1;
	TLineSeries *In2;
	TLineSeries *Zero;
	TArrowSeries *Cursor;
	TPopupMenu *PopupMenu1;
	TMenuItem *N1;
	TCSpinButton *CSpinButton1;
	TCheckBox *CheckBox1;

	void __fastcall FrameResize(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall Chart1ClickSeries(TCustomChart *Sender,
		TChartSeries *Series, int ValueIndex, TMouseButton Button,
		TShiftState Shift, int X, int Y);
	void __fastcall Chart1ClickBackground(TCustomChart *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall N1Click(TObject *Sender);
	void __fastcall CSpinButton1UpClick(TObject *Sender);
	void __fastcall CSpinButton1DownClick(TObject *Sender);

private: // User declarations
	void SetBorder(TLineSeries* _series, double _level, TColor _color);

	int sensor;

	class RCursor
	{
	private:
		TArrowSeries* series;
		int max;

	public:
		RCursor(TArrowSeries* _series);
		void SetMax(int _max);
		void Set(int _val);

	};

	RCursor rCursor;
	double step;

public: // User declarations

	__fastcall TFRViewZone(TComponent* Owner, int _sensor, bool _gain_visible,
		bool _check_visible);
	void Clear(void);
	void SetGain(double _gain);
	void TrigerVisible(void);
	void SetInner(bool _v);
	inline void AddOut(double _level,TColor _color)
	{
		if(_level<0)
			_level=-_level;
		BarOut->AddY(_level,"",_color);
	};
	inline void AddIn(double _level,TColor _color)
	{
		if(_level>0)
			_level=-_level;
		BarIn->AddY(_level,"",_color);
	};

	inline void SetBorderOut1(double _level, TColor _color)
	{
		if(_level<0)
			_level=-_level;
		SetBorder(Out1,_level,_color);
	}
	inline void SetBorderOut2(double _level, TColor _color)
	{
		if(_level<0)
			_level=-_level;
		SetBorder(Out2,_level,_color);
	}
	inline void SetBorderIn1(double _level, TColor _color)
	{
		if(_level>0)
			_level=-_level;
		SetBorder(In1,_level,_color);
	}
	inline void SetBorderIn2(double _level, TColor _color)
	{
		if(_level>0)
			_level=-_level;
		SetBorder(In2,_level,_color);
	}
	inline void SetZero(void)
	{
		SetBorder(Zero,0,clBlack);
	}

	inline int GetSensor(void)
	{
		return (sensor);
	}

	inline void SetZoneColor(TColor zoneColor)
	{
		Chart1->Color = zoneColor;
	}

	inline void SetStep(double _step)
	{
		step = _step;
	}
	typedef void(__closure * OnChangeGainDef)(int _sensor, double _gain);

	OnChangeGainDef OnChangeGain;

	typedef void(__closure * OnCalibrateDef)(TFRViewZone * _arc);

	OnCalibrateDef OnCalibrate;

	inline bool NeedCalibrate(void)
	{
		return(CheckBox1->Checked);
    }
};
#endif
