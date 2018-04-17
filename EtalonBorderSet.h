// ---------------------------------------------------------------------------

#ifndef EtalonBorderSetH
#define EtalonBorderSetH
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
#include <ADODB.hpp>
#include <DB.hpp>
#include <vector>
#include "EtalonNewDefect.h"
// ---------------------------------------------------------------------------
class TEtalonBorderSet: public TForm
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
	TLineSeries *Devider1;
	TLineSeries *Devider2;
	TLineSeries *DeviderIn1;
	TLineSeries *DeviderIn2;
	TADOQuery *QEtalons;
	TComboBox *cbEtalonTubeDefects;
	TPanel *Panel1;
	void __fastcall FormKeyPress(TObject *Sender,wchar_t &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Chart1ClickBackground(TCustomChart *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall cbEtalonTubeDefectsKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

private: // User declarations
	TIniFile* ini;
	AnsiString title;
	double start;
	double zone_size;
	void SetBorder(TLineSeries* _series,double _level, TColor _color);
	int curr_start_zone;
	int curr_final_zone;
	//int curr_zone;
	int curr_sens;
//	int DevidMeas1;
//	int DevidMeas2;
	bool IsLinear;
	double borderOut1;
	double borderOut2;
	std::vector<TLineSeries*> DefectsSeries;
	std::vector<TLineSeries*> DevidersSeries;
	std::vector<int> DevidersMeasures;
	int curr_defect;
	bool KeyUpFlag;
	TEtalonNewDefectForm* EtalonNewDefectForm;
	void SaveOldBorders();
	void LoadOldBorders();
	int oldTopBorderPercent;
	int oldBottomBorderPercent;
	AnsiString oldTopBorderSource;
	AnsiString oldBottomBorderSource;
public: // User declarations
	__fastcall TEtalonBorderSet(TEtalonNewDefectForm* Owner,TIniFile* _ini,AnsiString _title);
	void SetCaption(AnsiString _cap);
	void Clear(void);
	void SetMetric(double _start,double _zone_size);
	void SetInner(bool _v);
	void SetCurrPoint(int _curr_start_zone, int _curr_final_zone, int _curr_sens, bool _IsLinear);
	//показывает границы поиска дефектов СОП
	void ViewEtalonCheck(int _startZone, int _finalZone);
	//расчитывает измерение на графике в зависимости от расстояния
	int CalcMeasFromRange(int _x);
	//Рисует границы дефектов
	void SetBrackBorder(int _startMeas,int _finalMeas,double _Border,TColor _color);
	//Рисует границы дефектов эталона
	void SetEBorder(int _border,int _Y_coord);
	//Рисует делители зон
	void AddDevider(int _meas,TColor _color);
	void AddDeviderIn(int _meas,TColor _color);
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

//	inline void AddDevider1(int _meas,TColor _color)
//	{
//		DevidMeas1 = _meas;
//		Devider1->AddXY(_meas,0,"",_color);
//		Devider1->AddXY(_meas,100,"",_color);
//	};
//	inline void AddDevider2(int _meas,TColor _color)
//	{
//		DevidMeas2 = _meas;
//		Devider2->AddXY(_meas,0,"",_color);
//		Devider2->AddXY(_meas,100,"",_color);
//	};
//	inline void AddDeviderIn1(int _meas,TColor _color)
//	{
//		DeviderIn1->AddXY(_meas,-100,"",_color);
//		DeviderIn1->AddXY(_meas,0,"",_color);
//	};
//	inline void AddDeviderIn2(int _meas,TColor _color)
//	{
//		DeviderIn2->AddXY(_meas,-100,"",_color);
//		DeviderIn2->AddXY(_meas,0,"",_color);
//	};
	inline void SetBorderOut1(double _level, TColor _color)
	{
		if(_level<0)
			_level=-_level;
		borderOut1 = _level;
		SetBorder(Out1,_level,_color);
	}
	inline void SetBorderOut2(double _level, TColor _color)
	{
		if(_level<0)
			_level=-_level;
		borderOut2 = _level;
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
