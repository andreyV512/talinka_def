#pragma hdrstop
#include "CLinearResult.h"
#include "Global.h"
#pragma package(smart_init)

// ---------------------------------------------------------------------------
CLinearResult::CLinearResult(int sensors, TIniFile* _ini, TComponent* _Owner)
	: Result(sensors, _ini, _Owner)
{
	LoadSettings0();
}

// ---------------------------------------------------------------------------
void CLinearResult::PutSourceDataOnChartX(TFViewZoneS* _F, int zone, int sensor)
{
	_F->Clear();
	for (int j = 0; j < (int)Source_Data[zone][sensor].size(); j++)
	{
		_F->AddSource(Source_Data[zone][sensor][j] * gain[sensor]);
		if (isMedianFilter)
			_F->AddMedian(Median_Data[zone][sensor][j] * gain[sensor]);
		if (SystemConst::isLinearDigitalFilterOut)
			_F->AddFilteredOut(FilteredOut_Data[zone][sensor][j] * gain[sensor]);
		if (SystemConst::isLinearDigitalFilterIn)
			_F->AddFilteredIn(FilteredIn_Data[zone][sensor][j] * gain[sensor]);
	}
	// рисуем пороги брака и класса 2, если он включен
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	if (inner)
	{
		if (bordersIn.Length >= 1)
			_F->SetBorderIn1(bordersIn[0], ZoneColorIn(bordersIn[0], false));
		if (bordersIn.Length >= 2)
			_F->SetBorderIn2(bordersIn[1], ZoneColorIn(bordersIn[1], false));
	}
	_F->SetMetric(zone*zone_length, zone_length);
}

// ---------------------------------------------------------------------------
void CLinearResult::PutResultOnChart(void)
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

// ---------------------------------------------------------------------------
void CLinearResult::LoadSettings0(void)
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");

	double bor;

	borders.set_length(1);
	borders[0] = ini->ReadFloat(sect, "LinearBorder1", 0.0);
	bor = ini->ReadFloat(sect, "LinearBorder2", 0.0);
	if (bor != 0)
	{
		borders.set_length(2);
		borders[1] = bor;
	}
	inner = ini->ReadBool("Filters","isFilterPrIn",0);
	SensorFormD->SetInner(inner);
	if (!inner)
		bordersIn.set_length(0);
	else
	{
		bordersIn.set_length(1);
		bordersIn[0] = ini->ReadFloat(sect, "LinearBorder1Inner", 0.0);
		bor = ini->ReadFloat(sect, "LinearBorder2Inner", 0.0);
		if (bor != 0)
		{
			bordersIn.set_length(2);
			bordersIn[1] = bor;
		}
	}
	dead_zone_start = ini->ReadInteger(sect, "LinearDeadZoneStart", 0);
	dead_zone_finish = ini->ReadInteger(sect, "LinearDeadZoneFinish", 0);
	gain.resize(sensors);
	AnsiString diameter = ini->ReadInteger(sect, "Diameter", 1);
	for (int i = 0; i < sensors; i++)
	{
		AnsiString name;
		name = "Gain";
		name += i;
		gain[i] = ini->ReadFloat("PR", name, 10);
	}
}

// ---------------------------------------------------------------------------
void CLinearResult::LoadSettings(void)
{
	LoadSettings0();
	Result::LoadSettings();
}

// ---------------------------------------------------------------------------
TColor CLinearResult::ZoneColorIn(double meas, bool _IsDead)
{
	if (_IsDead)
		return (Dead);
	if (meas < 0)
		meas = -meas;
	if (meas < bordersIn[bordersIn.get_high()])
		return Good;
	else if (meas >= bordersIn[0])
		return Brack;
	else
		return Class2;
}

// ---------------------------------------------------------------------------
void CLinearResult::PutDataOnChartX(TFViewZoneD* _F, int initZone, int sensor)
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
	int lastSens = this->sensors_a; //(int)FilteredOut_Data[initZone].size()-1;
	if (sensor < 0) sensor = 0;
	if (sensor > lastSens) sensor = lastSens;

	SensorFormD->SetCurrPoint(initZone,sensor,true);
	SensorFormD->SetCaption("Датчик " + IntToStr(sensor + 1) + ", зоны с " +
		(startZone+1) + " по " + (finalZone+1));
	FRM->MarkerSet(initZone,sensor);

	for (int zone = startZone; zone <= finalZone; zone++)
	{
		for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
			_F->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor], Deads[zone][j]));
		if (inner)
		{
			for (int j = 0; j < (int)FilteredIn_Data[zone][sensor].size(); j++)
				_F->AddIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
				ZoneColorIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
				Deads[zone][j]));
		}
	}
	_F->AddDevider1((int)FilteredOut_Data[startZone][sensor].size(), clBlue);
	if((initZone < finalZone) && (initZone > startZone))
		_F->AddDevider2((int)FilteredOut_Data[startZone][sensor].size()
			+ (int)FilteredOut_Data[finalZone-1][sensor].size(), clBlue);
	else _F->AddDevider2(0, clBlue);
	if (inner)
	{
		_F->AddDeviderIn1((int)FilteredIn_Data[startZone][sensor].size(), clBlue);
		if((initZone < finalZone) && (initZone > startZone))
		_F->AddDeviderIn2((int)FilteredIn_Data[startZone][sensor].size()
			+ (int)FilteredIn_Data[finalZone-1][sensor].size(), clBlue);
	}

	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	_F->SetMetric(startZone*zone_length, (finalZone-startZone+1)*zone_length);
	if (inner)
	{
		if (bordersIn.Length >= 1)
			_F->SetBorderIn1(bordersIn[0], ZoneColorIn(bordersIn[0], false));
		if (bordersIn.Length >= 2)
			_F->SetBorderIn2(bordersIn[1], ZoneColorIn(bordersIn[1], false));
		_F->SetZero();
	}
	_F->SetMetric(startZone*zone_length, (finalZone-startZone+1)*zone_length);
	if(ini->ReadBool("OtherSettings","ViewEtalonCheck",0))
		_F->ViewEtalonCheck(startZone,finalZone);
	else _F->Panel1->Visible = false;
}

// ---------------------------------------------------------------------------
void CLinearResult::ComputeSensorClass(int _zone, int _sensor)
{
	if (inner)
	{
		double v0 = AbsMaxV(FilteredOut_Data[_zone][_sensor], _zone);
		double v1 = AbsMaxV(FilteredIn_Data[_zone][_sensor], _zone);
		if (v0 < 0)
			sensor_data[_zone][_sensor] = -1;
		else
			sensor_data[_zone][_sensor] =
				LevelClass(gain[_sensor] * v0, gain[_sensor] * v1);
	}
	else
	{
		double v0 = AbsMaxV(FilteredOut_Data[_zone][_sensor], _zone);
		if (v0 < 0)
			sensor_data[_zone][_sensor] = -1;
		else
			sensor_data[_zone][_sensor] = Result::LevelClass(gain[_sensor] * v0);
	}
}

// ---------------------------------------------------------------------------
double CLinearResult::LevelClass(double _level0, double _level1)
{
	double cl = Result::LevelClass(_level0);
	if (inner)
	{
		double clIn = -1;
		if (bordersIn.get_length() == 1)
		{
			if (_level1 < bordersIn[0])
				clIn = 2;
			else
				clIn = 0;

		}
		else if (bordersIn.get_length() == 2)
		{
			if (_level1 < bordersIn[1])
				clIn = 2;
			else if (_level1 < bordersIn[0])
				clIn = 1;
			else
				clIn = 0;
		}
		if (cl > clIn)
			cl = clIn;
	}
	return (cl);
}

// ---------------------------------------------------------------------------
void CLinearResult::PutDataOnFrame(TFRViewZone* _fr, int zone, int sensor, TColor zoneColor)
{
	_fr->Clear();
	for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
		_fr->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
		ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor], Deads[zone][j]));
	if (inner)
	{
		for (int j = 0; j < (int)FilteredIn_Data[zone][sensor].size(); j++)
			_fr->AddIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
			ZoneColorIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
			Deads[zone][j]));
	}
	// рисуем пороги брака и класса 2, если он включен
	if (borders.Length >= 1)
		_fr->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_fr->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	if (inner)
	{
		if (bordersIn.Length >= 1)
			_fr->SetBorderIn1(bordersIn[0], ZoneColorIn(bordersIn[0], false));
		if (bordersIn.Length >= 2)
			_fr->SetBorderIn2(bordersIn[1], ZoneColorIn(bordersIn[1], false));
		_fr->SetZero();
	}
	_fr->SetGain(gain[sensor]);
	if (zoneColor == Good)
		zoneColor = clBtnFace;
	_fr->SetZoneColor(zoneColor);
}

void CLinearResult::Bind(TFRLine* _FRM)
{
	FRM = _FRM;
	FRM->OnDblClick = OnDblClick;
}

void CLinearResult::OnDblClick(int _zone, int _sensor, bool _IsCtrl)
{
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

void CLinearResult::PutResultOnChart(TChart *C)
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
			{
				C->Series[i]->AddXY(j, 1, "", ClassColor(sensor_data[j][i]));
			}
			else
				C->Series[i]->Add(0, "", clWhite);
	}
}
//----------------------------------------------------------------------------
void CLinearResult::PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
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

	_F->SetCurrPoint(StartZone,FinalZone,sensor,true);
	_F->SetCaption("Датчик " + IntToStr(sensor + 1) + ", зоны с " +
		(StartZone+1) + " по " + (FinalZone+1));
	for (int zone = StartZone; zone <= FinalZone; zone++)
	{
		for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
			_F->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			Deads[zone][j]));
		if (inner)
		{
			for (int j = 0; j < (int)FilteredIn_Data[zone][sensor].size(); j++)
				_F->AddIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
				ZoneColorIn(FilteredIn_Data[zone][sensor][j] * gain[sensor],
				Deads[zone][j]));
		}
	}
	//делители зон
	int dataLength = 0;
	for (int zone = StartZone; zone <= FinalZone; zone++)
	{
		dataLength += (int)FilteredOut_Data[zone][sensor].size();
		_F->AddDeviderIn(dataLength, clBlue);
	}
	//пороги классов отбраковки
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	if (inner)
	{
		if (bordersIn.Length >= 1)
			_F->SetBorderIn1(bordersIn[0], ZoneColorIn(bordersIn[0], false));
		if (bordersIn.Length >= 2)
			_F->SetBorderIn2(bordersIn[1], ZoneColorIn(bordersIn[1], false));
		_F->SetZero();
	}
	_F->SetMetric(StartZone*zone_length,
		(FinalZone - StartZone + 1)*zone_length);
	//пороги поиска дефектов эталона            //todo
	//if (ini->ReadBool("OtherSettings", "ViewEtalonCheck", 0))
		_F->ViewEtalonCheck(StartZone, FinalZone);
	//else
		_F->Panel1->Visible = false;
}

// ---------------------------------------------------------------------------
