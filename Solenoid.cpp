// ---------------------------------------------------------------------------
#pragma hdrstop
#include "Solenoid.h"
#include <IniFiles.hpp>
#include "LCardData.h"
#include "Protocol.h"
#include "Global.h"
#include "tools_debug/DebugMess.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
Solenoid *CrossSolenoid = NULL;
Solenoid *LinearSolenoid = NULL;

Solenoid::Solenoid()//String TypeOfControl)
{
	// typeOfControl - PP или PR
	TIniFile *ini = new TIniFile(Globals::IniFileName);
	/*
	if (TypeOfControl == "PP")
	{
		chAmperage = 0;
		chVoltage = 1;
		AlarmLevel = (float)(ini->ReadFloat("PP", "ResistSolenoid", 90.0) * 1.4);
		AlarmULevel = (float)ini->ReadFloat("PP", "VoltageSolenoid", 270);
		DifULevel = (float)ini->ReadFloat("PP", "DifVoltageSolenoid", 20);
	}
	else
	{
		chAmperage = 2;
		chVoltage = 3;
		AlarmLevel = (float)(ini->ReadFloat("PR", "ResistSolenoid", 90.0) * 1.4);
		AlarmULevel = (float)ini->ReadFloat("PR", "VoltageSolenoid", 270);
		DifULevel = (float)ini->ReadFloat("PR", "DifVoltageSolenoid", 20);
	}
	Devider = ini->ReadFloat(TypeOfControl, "Devider", 72.0);
	 */
	channelSolenoidsON = ini->ReadInteger("PP", "spIsSolenoidsON", 27);
	solenoidTresholdU = ini->ReadFloat("PP", "solenoidTresholdU", 9.0);
	channelSolinoid1 = ini->ReadInteger("PP", "spSolenoid1", 28);
	channelSolinoid2 = ini->ReadInteger("PP", "spSolenoid2", 29);
	delete ini;
}

// ---------------------------------------------------------------------------
bool Solenoid::SolenoidOn()
{
	double t = lcard->GetValue(channelSolenoidsON);
	dprint("solenoid on %f\n", t);
	return 2 >  t;
}

bool Solenoid::Solenoid1U(double &t)
{
	t = lcard->GetValue(channelSolinoid1);
   return t < solenoidTresholdU;
}
bool Solenoid::Solenoid2U(double &t)
{
   t = lcard->GetValue(channelSolinoid2);
   return t < solenoidTresholdU;
}
/*
double Solenoid::getAmperage()
{
	double I = lcard->GetValue(chAmperage);
	return I = (I - 2.5) * 10;
}

double Solenoid::getVoltage()
{
	double U = lcard->GetValue(chVoltage);
	U = U * Devider;
	return U;
}

double Solenoid::getResist()
{
	double i = getAmperage();
	double u = getVoltage();
	double R = (double)(u / i);
	return R;
}

bool Solenoid::OkU()
{
	bool pu = (
		(getVoltage() < DifULevel + AlarmULevel)
		&& (getVoltage() >AlarmULevel - DifULevel)
		);
	return pu;
}
bool Solenoid::OkResist()
{
	double t = getResist();
	bool p = (t < AlarmLevel);
	if(!p)
	{
		AnsiString a;
		a+="!OkResist: ";
		a+=GetUIR();
		a.cat_printf(" AlarmLevel=%.3f",AlarmLevel);
		TPr::pr(a);
	}
	return p;
}

AnsiString Solenoid::GetUIR(void)
{
	double i = getAmperage();
	double u = getVoltage();
	double R = (double)(u / i);
	AnsiString ret;
	ret.printf("U=%.3lf I=%.3lf R=%.3lf", u, i, R);
	return (ret);
}
*/
