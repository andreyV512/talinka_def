// ---------------------------------------------------------------------------

#ifndef uFViewZoneDH
#define uFViewZoneDH
// ---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <IniFiles.hpp>
#include "Chart.hpp"
#include "TeEngine.hpp"
#include "TeeProcs.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "Series.hpp"
// ---------------------------------------------------------------------------
class TFViewZoneD : public TForm
{
__published: // IDE-managed Components
	TChart *Chart1;
	TStatusBar *StatusBar1;
	TBarSeries *BarOut;
	TBarSeries *BarIn;
	TLineSeries *Out1;
	TLineSeries *Out2;
	TLineSeries *In1;
	TLineSeries *In2;
	TLineSeries *Zero;
	void __fastcall FormKeyPress(TObject *Sender,wchar_t &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Chart1ClickBackground(TCustomChart *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
private: // User declarations
	TIniFile* ini;
	AnsiString title;
	double start;
	double zone_size;
	void SetBorder(TLineSeries* _series,double _level, TColor _color);
public: // User declarations
	__fastcall TFViewZoneD(TComponent* Owner,TIniFile* _ini,AnsiString _title);
	void SetCaption(AnsiString _cap);
	void Clear(void);
	void SetMetric(double _start,double _zone_size);
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
};
#endif
