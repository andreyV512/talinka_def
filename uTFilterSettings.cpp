// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uTFilterSettings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFilterSettings *FilterSettings;
// ---------------------------------------------------------------------------
__fastcall TFilterSettings::TFilterSettings(TComponent* Owner)
	: TFrame(Owner)
{
}
// ---------------------------------------------------------------------------
void TFilterSettings::Load(TIniFile *_ini,AnsiString _PostFix)
{
	CBUsed->Checked=_ini->ReadBool("Filters",AnsiString("isFilter")+_PostFix,0);

	rgFilterType->ItemIndex=_ini->ReadInteger("Filters",AnsiString("FilterType")+_PostFix,0);
	rgFilterSubType->ItemIndex=_ini->ReadInteger("Filters",AnsiString("FilterSubType")+_PostFix,0);

	csOrder->Value=_ini->ReadInteger("Filters",AnsiString("order")+_PostFix,2);
	eCutoffFreq->Text=FloatToStr(_ini->ReadFloat("Filters",AnsiString("cutoffFrequency")+_PostFix,300.0));
	eWidthFreq->Text=FloatToStr(_ini->ReadFloat("Filters",AnsiString("widthFrequency")+_PostFix,300.0));
	eCenterFreq->Text=FloatToStr(_ini->ReadFloat("Filters",AnsiString("centerFrequency")+_PostFix,300.0));
	eRippleDb->Text=FloatToStr(_ini->ReadFloat("Filters",AnsiString("rippleDb")+_PostFix,0.3));
	eRolloff->Text=FloatToStr(_ini->ReadFloat("Filters",AnsiString("rolloff")+_PostFix,0.3));
	ChooseFilter();
}
void TFilterSettings::Save(TIniFile *_ini,AnsiString _PostFix)
{
	_ini->WriteBool("Filters",AnsiString("isFilter")+_PostFix,CBUsed->Checked);

	_ini->WriteInteger("Filters",AnsiString("FilterType")+_PostFix,rgFilterType->ItemIndex);
	_ini->WriteInteger("Filters",AnsiString("FilterSubType")+_PostFix,rgFilterSubType->ItemIndex);

	_ini->WriteInteger("Filters",AnsiString("order")+_PostFix,csOrder->Value);
	_ini->WriteFloat("Filters",AnsiString("cutoffFrequency")+_PostFix,(double)StrToFloat(eCutoffFreq->Text));
	_ini->WriteFloat("Filters",AnsiString("widthFrequency")+_PostFix,(double)StrToFloat(eWidthFreq->Text));
	_ini->WriteFloat("Filters",AnsiString("centerFrequency")+_PostFix,(double)StrToFloat(eCenterFreq->Text));
	_ini->WriteFloat("Filters",AnsiString("rippleDb")+_PostFix,(double)StrToFloat(eRippleDb->Text));
	_ini->WriteFloat("Filters",AnsiString("rolloff")+_PostFix,(double)StrToFloat(eRolloff->Text));
}
void TFilterSettings::ChooseFilter()
{
	bool m[6]=
	{0,0,0,0,0,0};
	switch(rgFilterType->ItemIndex)
	{
	case 0:
		if(rgFilterSubType->ItemIndex<=1)
		{
			m[0]=1;
			m[2]=1;
		}
		else
		{
			m[0]=1;
			m[1]=1;
			m[3]=1;
		}
		break;
	case 1:
		if(rgFilterSubType->ItemIndex<=1)
		{
			m[0]=1;
			m[2]=1;
			m[4]=1;
		}
		else
		{
			m[0]=1;
			m[1]=1;
			m[3]=1;
			m[4]=1;
		}
		break;
	case 2:
		if(rgFilterSubType->ItemIndex<=1)
		{
			m[0]=1;
			m[2]=1;
			m[4]=1;
			m[5]=1;
		}
		else
		{
			m[0]=1;
			m[1]=1;
			m[3]=1;
			m[4]=1;
			m[5]=1;
		}
		break;
	}
	csOrder->Enabled=m[0];
	eCenterFreq->Enabled=m[1];
	eCutoffFreq->Enabled=m[2];
	eWidthFreq->Enabled=m[3];
	eRippleDb->Enabled=m[4];
	eRolloff->Enabled=m[5];
}

void __fastcall TFilterSettings::rgFilterTypeClick(TObject *Sender)
{
	ChooseFilter();
}
void __fastcall TFilterSettings::rgFilterSubTypeClick(TObject *Sender)
{
	ChooseFilter();
}
void TFilterSettings::SetEnable(bool _v)
{
	Enabled=_v;
	CBUsed->Enabled=_v;
	rgFilterType->Enabled=_v;
	rgFilterSubType->Enabled=_v;

	if(_v)
		ChooseFilter();
	else
	{
		csOrder->Enabled=false;
		eCutoffFreq->Enabled=false;
		eWidthFreq->Enabled=false;
		eCenterFreq->Enabled=false;
		eRippleDb->Enabled=false;
		eRolloff->Enabled=false;
	}
}
