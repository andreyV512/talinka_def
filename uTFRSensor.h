//---------------------------------------------------------------------------

#ifndef uTFRSensorH
#define uTFRSensorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IniFiles.hpp"
#include "cspin.h"
//---------------------------------------------------------------------------
class TFRSensor : public TFrame
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *LNN;
	TComboBox *CBRange;
	TCSpinEdit *SEChannel;
	TComboBox *CBMode;
private:	// User declarations
	AnsiString tp;
	TIniFile* ini;
	int nn;
public:		// User declarations
	__fastcall TFRSensor(TComponent* Owner,TIniFile* _ini, AnsiString _tp,int _nn);
	void SetEnabledR(bool _v);
	void LoadSettings(void);
	void SaveSettings(void);
	int GetRange(void);
	int GetChannel(void);
	int GetMode(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFRSensor *FRSensor;
//---------------------------------------------------------------------------
#endif
