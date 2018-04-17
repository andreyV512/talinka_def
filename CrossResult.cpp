// ---------------------------------------------------------------------------
#pragma hdrstop
#include "CrossResult.h"
#include "Global.h"
#include "Protocol.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

CCrossResult::CCrossResult(int sensors, TIniFile* _ini, TComponent* _Owner)
	: Result(sensors, _ini, _Owner)
{
	LoadSettings0();
	FRM = NULL;
}

void CCrossResult::PutSourceDataOnChartX(TFViewZoneS* _F, int zone, int sensor)
{
	_F->Clear();
	for (int j = 0; j < (int)Source_Data[zone][sensor].size(); j++)
	{
		_F->AddSource(Source_Data[zone][sensor][j] * gain[sensor]);
		if (isMedianFilter)
			_F->AddMedian(Median_Data[zone][sensor][j] * gain[sensor]);
		if (SystemConst::isCrossDigitalFilter)
			_F->AddFilteredOut(FilteredOut_Data[zone][sensor][j] * gain[sensor]);
	}
	// рисуем пороги брака и класса 2, если он включен
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	_F->SetMetric(zone*zone_length, zone_length);
}

void CCrossResult::PutResultOnChart(void)
{
	if (FRM == NULL)
		return;
	FRM->Clear();
	int defects=0;
	bool isDef = false;
	for (int j = 0; j < Globals::max_zones; j++)
	{
		isDef = false;
		for (int i = 0; i < sensors; i++)
		{
			if (j < zones)
			{
				FRM->Chart1->Series[i]->AddXY(j, 1, "",
				ClassColor(sensor_data[j][i]));
				if((sensor_data[j][i]==0||sensor_data[j][i]==1) && !isDef)
				{
					defects++;
					isDef = true;
				}
			}
			else
				FRM->Chart1->Series[i]->Add(0, "", clWhite);
		}
	}
	FRM->SetDefects(defects);
}

void CCrossResult::LoadSettings0(void)
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");

	double bor;
	sensors_a = ini->ReadInteger(sect, "CrossSensors", sensors);

	borders.set_length(1);
	borders[0] = ini->ReadFloat(sect, "CrossBorder1", 0.0);
	bor = ini->ReadFloat(sect, "CrossBorder2", 0.0);
	if (bor != 0)
	{
		borders.set_length(2);
		borders[1] = bor;
	}
	dead_zone_start = ini->ReadInteger(sect, "CrossDeadZoneStart", 0);
	dead_zone_finish = ini->ReadInteger(sect, "CrossDeadZoneFinish", 0);
	gain.resize(sensors);
	AnsiString diameter = ini->ReadInteger(sect, "Diameter", 1);
	for (int i = 0; i < sensors; i++)
	{
		AnsiString section;
		section = "Gain ";
		section += diameter;
		AnsiString name;
		name = "Gain";
		name += i;
		gain[i] = ini->ReadFloat(section, name, 10);
	}
}

// ---------------------------------------------------------------------------
void CCrossResult::LoadSettings(void)
{
	LoadSettings0();
	Result::LoadSettings();
}

void CCrossResult::PutTest(TChart *C)
{
	for (int i = 0; i < 12; i++)
	{
		((TBarSeries*) C->Series[i])->OnDblClick = SeriesDblClick;
		C->Series[i]->Clear();
		for (int j = 0; j < Globals::max_zones; j++)
		{
			C->Series[i]->AddXY(j, 1, "", clBlue);
		}
	}
}

void CCrossResult::Bind(TFRCross* _FRM)
{
	FRM = _FRM;
	FRM->OnDblClick = OnDblClick;
}

void CCrossResult::OnDblClick(int _zone, int _sensor, bool _IsCtrl)
{
	TPr::pr("CCrossResult::OnDblClick");
	if (_IsCtrl)
	{
		SensorFormS->SetCaption("Датчик " + IntToStr(_sensor + 1) + ", зона " +
			_zone);
		PutSourceDataOnChartX(SensorFormS, _zone, _sensor);
		SensorFormS->Show();
	}
	else
	{
		PutDataOnChartX(SensorFormD, _zone, _sensor);
		SensorFormD->Show();
	}
}

void CCrossResult::PutResultOnChart(TChart *C)
{
	// рисует результат по зонам на чарте С;
	// чтобы графики были адекватными, все оставшиеся зоны необходимо забить нулями (глюк Teechart)
	C->LeftAxis->Maximum = sensors;
	for (int i = 0; i < sensors; i++)
	{
		C->Series[i]->Tag = i;
		((TBarSeries*) C->Series[i])->OnDblClick = SeriesDblClick;
		C->Series[i]->Clear();
		for (int j = 0; j < Globals::max_zones; j++)
			if (j < zones)
				C->Series[i]->AddXY(j, 1, "", ClassColor(sensor_data[j][i]));
			else
				C->Series[i]->Add(0, "", clWhite);
	}
}
// ---------------------------------------------------------------------------
void CCrossResult::PutDataOnChartX(TFViewZoneD* _F, int initZone, int sensor)
{
	// рисует измерения в зоне/датчике на графике
	_F->Clear();
		//КызылОрда Показ 3 зон
	int startZone;
	int finalZone;
	int lastZone = zones-1;
	if (initZone <= 0)
		initZone = 0;
	if (initZone >= lastZone)
		initZone = lastZone;
	if (initZone == 0)
		startZone = initZone;
	else startZone = initZone - 1;
	if (initZone == lastZone)
		finalZone = initZone;
	else finalZone = initZone + 1;
	int lastSens = this->sensors_a;
	if (sensor < 0) sensor = 0;
	if (sensor > lastSens) sensor = lastSens;

	SensorFormD->SetCurrPoint(initZone,sensor,false);
	SensorFormD->SetCaption("Датчик " + IntToStr(sensor + 1) + ", зоны с " +
		(startZone+1) + " по " + (finalZone+1));
	FRM->MarkerSet(initZone,sensor);

	for (int zone = startZone; zone <= finalZone; zone++)
	{
		for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
			_F->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor], Deads[zone][j]));
	}
	_F->AddDevider1((int)FilteredOut_Data[startZone][sensor].size(), clBlue);
	if((initZone < finalZone) && (initZone > startZone))
		_F->AddDevider2((int)FilteredOut_Data[startZone][sensor].size()
			+ (int)FilteredOut_Data[finalZone-1][sensor].size(), clBlue);
	else _F->AddDevider2(0, clBlue);
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	_F->SetMetric(startZone*zone_length, (finalZone-startZone+1)*zone_length);
	if(ini->ReadBool("OtherSettings","ViewEtalonCheck",0))
		_F->ViewEtalonCheck(startZone,finalZone);
	else _F->Panel1->Visible = false;
}
//----------------------------------------------------------------------------
void CCrossResult::PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
	, int FinalZone, int sensor)
{
	// рисует измерения в зоне/датчике на графике
	_F->Clear();
	// КызылОрда Показ 3 зон
	int lastZone = (int)FilteredOut_Data.size() - 1;
	if (StartZone <= 0)
		StartZone = 0;
	else if (StartZone >= lastZone)
		StartZone = lastZone;
	if (FinalZone <= StartZone)
		FinalZone = StartZone;
	else if (FinalZone >= lastZone)
		FinalZone = lastZone;

	int lastSens = this->sensors_a;
	if (sensor < 0)
		sensor = 0;
	else if (sensor > lastSens)
		sensor = lastSens;

	_F->SetCurrPoint(StartZone,FinalZone,sensor,false);
	_F->SetCaption("Датчик " + IntToStr(sensor + 1) + ", зоны с " +
		(StartZone+1) + " по " + (FinalZone+1));
	for (int zone = StartZone; zone <= FinalZone; zone++)
	{
		for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
			_F->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			Deads[zone][j]));
	}
	//делители зон
	int dataLength = 0;
	for (int zone = StartZone; zone <= FinalZone; zone++)
	{
		dataLength += (int)FilteredOut_Data[zone][sensor].size();
		_F->AddDevider(dataLength, clBlue);
	}
	//пороги классов отбраковки
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	_F->SetMetric(StartZone*zone_length,
		(FinalZone - StartZone + 1)*zone_length);
	//пороги поиска дефектов эталона            //todo
	//if (ini->ReadBool("OtherSettings", "ViewEtalonCheck", 0))
		_F->ViewEtalonCheck(StartZone, FinalZone);
	//else
		_F->Panel1->Visible = false;
}

// ---------------------------------------------------------------------------
