//---------------------------------------------------------------------------

#ifndef uTFRThickH
#define uTFRThickH
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
//---------------------------------------------------------------------------
class TFRThick : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *gbThickness;
	TChart *Chart1;
	TBarSeries *Series3;
	TFRBorders *FRBordersThick;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TPanel *Panel2;
	void __fastcall FrameResize(TObject *Sender);
private:	// User declarations
	void OnChangeChild(void);
	TIniFile* ini;
	void LoadSettings0(void);
public:		// User declarations
	__fastcall TFRThick(TComponent* Owner);
	typedef void(__closure * OnChangeDef)(void);
	OnChangeDef OnChange;
	void Clear(void);
	void Init(TIniFile* _ini);
	void LoadSettings(void);
	void SetEnabledR(bool _v);
	void SetDefects(int _val);
	void SetMinThick(double _val);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRThick *FRThick;
//---------------------------------------------------------------------------
#endif
