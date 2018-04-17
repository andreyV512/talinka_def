#pragma hdrstop
#include "ResultBase.h"
#include "Global.h"
#include <Forms.hpp>
#pragma package(smart_init)

// -----------------------------------------------------------------------------
ResultBase::ResultBase(short _sensors, TIniFile* _ini)
{
	ini = _ini;
	sensors = _sensors;
	sensors_a = _sensors;
	zones = 0;
	meas_per_zone = 0;
	borders.set_length(2);

	// задаем цвета, которые хочет юзер
	Brack = (TColor) ini->ReadInteger("Color", "Brack", 0);
	Class2 = (TColor) ini->ReadInteger("Color", "SecondClass", 0);
	Good = (TColor) ini->ReadInteger("Color", "Valid", 0);
	Dead = (TColor) ini->ReadInteger("Color", "DeadZone", 0);
	ZoneLength=ini->ReadInteger("Size","ZoneLength", 200);

	zone_data.set_length(Globals::max_zones);

	sensor_data.resize(Globals::max_zones);
	Deads.resize(Globals::max_zones);
	for (int i = 0; i < Globals::max_zones; i++)
	{
		sensor_data[i].resize(_sensors);
	}
}

ResultBase::~ResultBase()
{
}

// ----------------------------------------------------------------------------
void ResultBase::DeleteData()
{
	zone_data.set_length(0);
	zone_data.set_length(Globals::max_zones);
	zones = 0;
	meas_per_zone = 0;
}

// ------------------------------------------------------------------------------
TColor ResultBase::ClassColor(double _cl)
{
	if (_cl == 0)
		return (Brack);
	else if (_cl == 1)
		return (Class2);
	else if (_cl == 2)
		return (Good);
	else
		return (Dead);
}
