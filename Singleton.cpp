#pragma hdrstop
#include "Singleton.h"
#include "Protocol.h"
#pragma package(smart_init)
#include "tools_debug/DebugMess.h"
// ---------------------------------------------------------------------------
CSingleton* Singleton = NULL;

// ---------------------------------------------------------------------------
CSingleton::CSingleton(TIniFile* _ini, TComponent* _Owner)
{
	AnsiString sect = "Type_" + _ini->ReadString("Default", "TypeSize", "1");
	if (!_ini->ValueExists(sect, "CrossSensors"))
		_ini->WriteString(sect, "CrossSensors", "12");
	if (!_ini->ValueExists(sect, "LineSensors"))
		_ini->WriteString(sect, "LineSensors", "4");

	int LSensors=_ini->ReadInteger(sect,"LineSensors",4);
	int CSensors=12;//ўукин 20151201 //_ini->ReadInteger(sect,"CrossSensors",12);

	CrossResult = new CCrossResult(CSensors, _ini, _Owner);
	LinearResult = new CLinearResult(LSensors, _ini, _Owner);
	int max_sensors=CSensors;
	if(max_sensors<LSensors)
		max_sensors=LSensors;
	ThResult = new ThicknessResult(max_sensors, _ini);
	SumResult = new SummaryResult(max_sensors, _ini);
	FromFile=false;
	isSOP=false;
}

// ---------------------------------------------------------------------------
CSingleton::~CSingleton()
{
	delete CrossResult;
	delete LinearResult;
	delete ThResult;
	delete SumResult;
}

// ---------------------------------------------------------------------------
void CSingleton::ComputeZonesData()
{
	bool xL = LinearResult->zones > 0;
	bool xC = CrossResult->zones > 0;
	bool xT = ThResult->zones > 0;

   dprint("long %d cross %d thik %d\n"
	  , LinearResult->zones
	, CrossResult->zones
	, ThResult->zones
   );
	SumResult->zones = LinearResult->zones;
	if(SumResult->zones<CrossResult->zones)
		SumResult->zones=CrossResult->zones;
	if(SumResult->zones<ThResult->zones)
		SumResult->zones=ThResult->zones;
	if (xL)
	{
		if (SumResult->zones > LinearResult->zones)
			SumResult->zones = LinearResult->zones;
	}
	if (xC)
	{
		if (SumResult->zones > CrossResult->zones)
			SumResult->zones = CrossResult->zones;
	}
	if (xT)
	{
		if (SumResult->zones > ThResult->zones)
			SumResult->zones = ThResult->zones;
	}
	AnsiString a;
	a.cat_printf("SumResult->zones=[%d] ", SumResult->zones);
    TPr::pr(a);
	// 1 - годно, 0 - брак, 2 - 2 класс, 3 - 3 класс...
	for (int i = 0; i < SumResult->zones; i++)
	{

		if ((xC) && CrossResult->zone_data[i] == 0 ||
			((xL) && LinearResult->zone_data[i] == 0) ||
			((xT) && ThResult->LevelClass(ThResult->zone_data[i]) == 0))
			SumResult->zone_data[i] = 0;
		else if ((xC) && CrossResult->zone_data[i] == 1 ||
			((xL) && LinearResult->zone_data[i] == 1) ||
			((xT) && ThResult->LevelClass(ThResult->zone_data[i]) == 1))
			SumResult->zone_data[i] = 1;
		else
			SumResult->zone_data[i] = 2;
		/*
		 AnsiString a;
		 a.cat_printf("CZD[%d] ",i);
		 a+=CrossResult->zone_data[i];
		 a+=" ";
		 if(lin)
		 a+= LinearResult->zone_data[i];
		 else
		 a+="?";
		 a+=" ";
		 if(th)
		 a+= ThResult->LevelClass(ThResult->zone_data[i]);
		 else
		 a+="?";
		 a+=" = ";
		 a+=SumResult->zone_data[i];
		 TPr::pr(a);
		 */
	}
}

// ---------------------------------------------------------------------------
void CSingleton::AddZone()
{
	// функци€ вычисл€ет решение по зоне из совокупности дефектов и толщины
	// смотрим, какие модули принимали участие в работе
	bool lin = LinearResult->zones > 0;
	bool th = ThResult->zones > 0;

	// 1 - годно, 0 - брак, 2 - 2 класс, 3 - 3 класс...
	int i = SumResult->zones;
	if (CrossResult->zone_data[i] >= CrossResult->borders[0] ||
		((lin) && LinearResult->zone_data[i] >= LinearResult->borders[0]) ||
		((th) && ThResult->zone_data[i] < ThResult->borders[0]))
	{
		SumResult->zone_data[i] = 0;
	}
	else if (CrossResult->zone_data[i] <
		CrossResult->borders[CrossResult->borders.get_high()] &&
		((!lin) || LinearResult->zone_data[i] <
		LinearResult->borders[LinearResult->borders.get_high()]) &&
		((!th) || ThResult->zone_data[i] >=
		ThResult->borders[ThResult->borders.get_high()]))
	{
		SumResult->zone_data[i] = 1;
	}
	else
		SumResult->zone_data[i] = 2;
	SumResult->zones++;
}

// ---------------------------------------------------------------------------
void CSingleton::LoadSettings(void)
{
	CrossResult->LoadSettings();
	LinearResult->LoadSettings();
	ThResult->LoadSettings();
}
