#pragma hdrstop
#include "Result.h"
#include <algorithm>
#include "Global.h"
#include "Filters.h"
#include "Protocol.h"
#include "uFunctions.h"
#pragma package(smart_init)

// -----------------------------------------------------------------------------
// abs работает только с long, пришлось запилить свою функцию
template<typename T>
T Abs(T value)
{
	if (value < 0)
		value = value*-1;

	return value;
}

vector<double>Abs(vector<double>V)
{
	for (int i = 0; i < (int)V.size(); i++)
	{
		V[i] = Abs(V[i]);
	}
	return V;
}

// -----------------------------------------------------------------------------
Result::Result(int _sensors, TIniFile* _ini, TComponent* _Owner)
	: ResultBase((short)_sensors, _ini)
{
	SensorFormD = new TFViewZoneD(_Owner, _ini, "");
	SensorFormS = new TFViewZoneS(_Owner, _ini, "");
	shiftZone = -1;
	dead_zone_start = 0;
	dead_zone_finish = 0;
	meas_dead_zone_start = 0;
	meas_dead_zone_finish = 0;
	TypeView = 0;
	zone_length = 200;

	gain.resize(_sensors);
	LoadSettings();
	Source_Data.resize(Globals::max_zones);
	Median_Data.resize(Globals::max_zones);
	FilteredOut_Data.resize(Globals::max_zones);
	FilteredIn_Data.resize(Globals::max_zones);
	for (int i = 0; i < Globals::max_zones; i++)
	{
		Source_Data[i].resize(_sensors);
		Median_Data[i].resize(_sensors);
		FilteredOut_Data[i].resize(_sensors);
		FilteredIn_Data[i].resize(_sensors);
	}
}

Result::~Result()
{
	delete SensorFormS;
	delete SensorFormD;
}

// ----------------------------------------------------------------------------
void Result::DeleteData()
{
	ResultBase::DeleteData();
	for (int i = 0; i < (int)Source_Data.size(); i++)
	{
		for (long j = 0; j < (int)Source_Data[i].size(); j++)
		{
			Source_Data[i][j].clear();
			Median_Data[i][j].clear();
			FilteredOut_Data[i][j].clear();
			FilteredIn_Data[i][j].clear();
		}
	}
}

// ---------------------------------------------------------------------------
void Result::ComputeZoneData(bool isLinear)
{
	// пробегает по всем значениям, выискивая максимальные дефекты по зонам и датчикам
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors_a; i++)
		{
			if (i == 0)
				CalcDeadsZoneBegin(ZoneLength, dead_zone_start, j,
				Source_Data[j][i].size());
			ComputeFiltered(j, i, isLinear);
		}
	}
	CalcDeadsEnd(ZoneLength, dead_zone_finish, zones);
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors_a; i++)
			ComputeSensorClass(j, i);
		// zone_data[j] = *std::min_element(sensor_data[j].begin(),
		// sensor_data[j].end());
		zone_data[j] = *std::min_element(sensor_data[j].begin(),
			sensor_data[j].begin() + sensors_a);
	}
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
void Result::_medianfilter(double* signal, double* result, int N)
{

	// Move window through all elements of the signal
	for (int i = 2; i < N - 2; ++i)
	{
		// Pick up window elements
		vector<double>window;
		window.resize(WidthMedianFilter);
		for (int j = 0; j < WidthMedianFilter; ++j)
			window[j] = signal[i - 2 + j];
		// Order elements (only half of them)
		for (int j = 0; j < 3; ++j)
		{
			// Find position of minimum element
			int min = j;
			for (int k = j + 1; k < WidthMedianFilter; ++k)
				if (window[k] < window[min])
					min = k;
			// Put found minimum element in its place
			const double temp = window[j];
			window[j] = window[min];
			window[min] = temp;
		}
		// Get result - the middle element
		result[i - 2] = window[2];
	}
}

// ------------------------------------------------------------------------------
// 1D MEDIAN FILTER wrapper
// signal - input signal
// result - output signal
// N      - length of the signal
void Result::medianfilter(double* signal, double* result, int N)
{
	// Check arguments
	if (!signal || N < 1)
		return;
	// Treat special case N = 1
	if (N == 1)
	{
		if (result)
			result[0] = signal[0];
		return;
	}
	// Allocate memory for signal extension
	double* extension = new double[N + 4];
	// Check memory allocation
	if (!extension)
		return;
	// Create signal extension
	memcpy(extension + 2, signal, N * sizeof(double));
	for (int i = 0; i < 2; ++i)
	{
		extension[i] = signal[1 - i];
		extension[N + 2 + i] = signal[N - 1 - i];
	}
	// Call median filter implementation
	_medianfilter(extension, result ? result : signal, N + 4);
	// Free memory
	delete[]extension;
}

// ---------------------------------------------------------------------------
bool Result::AddZone(vector<vector<double> >new_data, bool isLinear)
{
	if (zones >= Globals::max_zones)
	{
		AnsiString a = "Result::AddZone: Количество зон ";
		a += zones;
		a += " больше или равно максимума ";
		a += Globals::max_zones;
		TPr::pr(a);
		return false;
	}
	DWORD begin = GetTickCount();
	sensors = (short) new_data.size();
	for (long i = 0; i < sensors_a; i++)
	{
		Source_Data[zones][i] = new_data[i];
		if (i == 0)
			CalcDeadsZoneBegin(ZoneLength, dead_zone_start, zones,
			Source_Data[zones][i].size());
		ComputeFiltered(zones, i, isLinear);
		ComputeSensorClass(zones, i);
	}
	zone_data[zones] = *std::min_element(sensor_data[zones].begin(),
		sensor_data[zones].begin() + sensors_a);
	AnsiString a;
	a.cat_printf("AddZone[%d]=%0.2f ", zones, zone_data[zones]);
	TPr::pr(a);

	zones++;
	if (zone_data[zones - 1] == -1)
		return true;
	return false;
}

// ---------------------------------------------------------------------------
bool Result::AddBankZone(CBank* _bank, bool _isLinear)
{
	if (_bank->GetNextZone(&Source_Data) < 0)
		return (false);
	for (int i = 0; i < sensors; i++)
	{
		if (i == 0)
			CalcDeadsZoneBegin(ZoneLength, dead_zone_start, zones,
			Source_Data[zones][i].size());
		ComputeFiltered(zones, i, _isLinear);
		ComputeSensorClass(zones, i);
	}
	zone_data[zones] = *std::min_element(sensor_data[zones].begin(),
		sensor_data[zones].begin() + sensors_a);
	AnsiString a;
	a.cat_printf("AddZone[%d]=%0.3f ", zones, zone_data[zones]);
	if (_isLinear)
		a += " Linear";
	else
		a += " Cross";
	TPr::pr(a);
	zones++;
	if (zone_data[zones - 1] == 0)
		return true;
	return false;
}

// ---------------------------------------------------------------------------
void Result::CalcDeadsZoneBegin(int _ZoneLength, int _Begin, int _zone,
	int _ZoneSize)
{
	double A = (double)_ZoneLength / (double)_ZoneSize;
	int B = _zone * _ZoneLength;
	Deads[_zone].resize(_ZoneSize);
	AnsiString a;
	for (int m = 0; m < _ZoneSize; m++)
		Deads[_zone][m] = (int)(A * (double)m) + B <= _Begin;
}

// ---------------------------------------------------------------------------
void Result::CalcDeadsEnd(int _ZoneLength, int _End, int _zones)
{
	_End = _zones * _ZoneLength - _End;
	int Pos;
	double A;
	int B;
	for (int z = 0; z < _zones; z++)
	{
		A = ((double)_ZoneLength) / ((double)Deads[z].size());
		B = _ZoneLength * z;
		for (int m = 0; m < (int)Deads[z].size(); m++)
		{
			if (((int)(A * ((double)m))) + B >= _End)
				Deads[z][m] = true;
		}
	}
}

// ---------------------------------------------------------------------------
TColor Result::ZoneColor(double meas, bool _IsDead)
{
	if (_IsDead)
		return (Dead);
	if (meas < 0)
		meas = -meas;
	if (meas < borders[borders.get_high()])
		return Good;
	else if (meas >= borders[0])
		return Brack;
	else
		return Class2;
}

// ---------------------------------------------------------------------------
void __fastcall Result::SeriesDblClick(TChartSeries *Sender, int ValueIndex,
	TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TPr::pr("Result::SeriesDblClick");
	int sn = Sender->Tag;
	Sender->ValueColor[ValueIndex] = clNavy;
	if (Shift.Contains(ssCtrl))
	{
		SensorFormS->SetCaption("Датчик " + IntToStr(sn + 1) + ", зона " +
			IntToStr(ValueIndex + 1));
		PutSourceDataOnChartX(SensorFormS, ValueIndex, sn);
		SensorFormS->Show();
		TypeView = 1;
	}
	else
	{
		SensorFormD->SetCaption("Датчик " + IntToStr(sn + 1) + ", зона " +
			IntToStr(ValueIndex + 1));
		PutDataOnChartX(SensorFormD, ValueIndex, sn);
		SensorFormD->Show();
		TypeView = 0;
	}
	startZone = ValueIndex;
	countZone = 1;
}

// ---------------------------------------------------------------------------

void Result::FillData(vector<vector<double> >sourceData)
{
	long MeasPerSensor = sourceData[0].size();
	Result::sensors_a = (short)sourceData.size();
	long i, j, p, q;

	if ((MeasPerSensor % zones) != 0)
	{
		// если последняя зона не является короткой
		// заполняем целые зоны
		int pos = (int)(MeasPerSensor / zones); // кол-во измерений в одной зоне

		Result::meas_per_zone = pos;
		for (i = 0; i < zones; i++)
			for (j = 0; j < sensors_a; j++)
				for (p = pos * i; p < pos * (i + 1); p++)
					Source_Data[i][j].push_back(sourceData[j][p]);

	}
	else
	{
		// заполняем целые зоны
		int pos = ((int)(MeasPerSensor)) / (zones - 1);
		// кол-во измерений в одной полной	зоне
		Result::meas_per_zone = pos;
		for (i = 0; i < zones - 1; i++)
			for (j = 0; j < sensors_a; j++)
				for (p = pos * i; p < pos * (i + 1); p++)
					Source_Data[i][j].push_back(sourceData[j][p]);

		// Заполняем последнюю зону
		for (j = 0; j < sensors_a; j++)
			for (q = pos * (zones - 1); q < MeasPerSensor; q++)
				Source_Data[zones - 1][j].push_back(sourceData[j][q]);
	}
}

// ------------------------------------------------------------------------------

void Result::SetGains(vector<double>_Gain)
{
	gain = _Gain;
}

// ----------------------------------------------------------------------------
void Result::SetSensorGain(double _gain, int sensor)
{
	gain[sensor] = _gain;
}

// ----------------------------------------------------------------------------
void Result::AutoCalibration(int zone)
{
	double averageMaximus;
	double min;
	double max;
	min = sensor_data[zone][0];
	max = sensor_data[zone][0];
	for (int i = 0; i < sensors_a; i++)
	{
		if ((sensor_data[zone][i] / gain[i]) > max)
			max = sensor_data[zone][i] / gain[i];

		if ((sensor_data[zone][i] / gain[i]) < min)
			min = sensor_data[zone][i] / gain[i];
	}
	averageMaximus = max; // (min + max) / 2;

	for (int i = 0; i < sensors_a; i++)
	{
		gain[i] = averageMaximus / ((sensor_data[zone][i] / gain[i]));
		String gain_str = "Gain " + IntToStr(Globals::current_diameter);
		ini->WriteFloat(gain_str, "Gain" + IntToStr(i),
			(double)StrToFloat(gain[i]));
	}
}

// ---------------------------------------------------------------------------
void Result::PutDataOnChartX(TFViewZoneD* _F, int initZone, int sensor)
{
	// рисует измерения в зоне/датчике на графике
	_F->Clear();
	// КызылОрда Показ 3 зон
	int startZone;
	int finalZone;
	int lastZone = (int)FilteredOut_Data.size() - 1;
	if (initZone <= 0)
		initZone = 0;
	if (initZone >= lastZone)
		initZone = lastZone;
	if (initZone == 0)
		startZone = initZone;
	else
		startZone = initZone - 1;
	if (initZone == lastZone)
		finalZone = initZone;
	else
		finalZone = initZone + 1;
	int lastSens = this->sensors_a; // (int)FilteredOut_Data[initZone].size()-1;
	if (sensor < 0)
		sensor = 0;
	if (sensor > lastSens)
		sensor = lastSens;
	for (int zone = startZone; zone <= finalZone; zone++)
	{
		for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
			_F->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor],
			Deads[zone][j]));
	}
	_F->AddDevider1((int)FilteredOut_Data[startZone][sensor].size(), clBlue);
	if ((initZone < finalZone) && (initZone > startZone))
		_F->AddDevider2((int)FilteredOut_Data[startZone][sensor].size() +
		(int)FilteredOut_Data[finalZone - 1][sensor].size(), clBlue);
	else
		_F->AddDevider2(0, clBlue);
	if (borders.Length >= 1)
		_F->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_F->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	_F->SetMetric(startZone*zone_length,
		(finalZone - startZone + 1)*zone_length);
	if (ini->ReadBool("OtherSettings", "ViewEtalonCheck", 0))
		_F->ViewEtalonCheck(startZone, finalZone);
	else
		_F->Panel1->Visible = false;
}

// ---------------------------------------------------------------------------
void Result::PutDataOnFrame(TFRViewZone* _fr, int zone, int sensor,
	TColor zoneColor)
{
	_fr->Clear();
	for (int j = 0; j < (int)FilteredOut_Data[zone][sensor].size(); j++)
		_fr->AddOut(FilteredOut_Data[zone][sensor][j] * gain[sensor],
		ZoneColor(FilteredOut_Data[zone][sensor][j] * gain[sensor],
		Deads[zone][j]));
	if (borders.Length >= 1)
		_fr->SetBorderOut1(borders[0], ZoneColor(borders[0], false));
	if (borders.Length >= 2)
		_fr->SetBorderOut2(borders[1], ZoneColor(borders[1], false));
	// рисуем пороги брака и класса 2, если он включен
	_fr->SetGain(gain[sensor]);
	if (zoneColor == Good)
		zoneColor = clBtnFace;
	_fr->SetZoneColor(zoneColor);
}

// ---------------------------------------------------------------------------
void Result::ComputeFiltered(int _zone, int _sensor, bool _isLinear)
{
	if (isMedianFilter)
	{
		Median_Data[_zone][_sensor].resize(Source_Data[_zone][_sensor].size());
		medianfilter(&Source_Data[_zone][_sensor][0],
			&Median_Data[_zone][_sensor][0],
			Source_Data[_zone][_sensor].size());
	}
	else
		Median_Data[_zone][_sensor] = Source_Data[_zone][_sensor];
	FilteredOut_Data[_zone][_sensor] = Median_Data[_zone][_sensor];
	FilteredIn_Data[_zone][_sensor] = Median_Data[_zone][_sensor];
	if (_isLinear)
	{
		if (SystemConst::isLinearDigitalFilterOut)
			LinearFilterOut->toFilter(&FilteredOut_Data[_zone][_sensor][0],
			FilteredOut_Data[_zone][_sensor].size());
		if (SystemConst::isLinearDigitalFilterIn)
			LinearFilterIn->toFilter(&FilteredIn_Data[_zone][_sensor][0],
			FilteredIn_Data[_zone][_sensor].size());
	}
	else
	{
		if (SystemConst::isCrossDigitalFilter)
			CrossFilter->toFilter(&FilteredOut_Data[_zone][_sensor][0],
			FilteredOut_Data[_zone][_sensor].size());
	}
	CalcDeadsZoneBegin(ZoneLength, dead_zone_start, _zone,
		Source_Data[_zone][_sensor].size());
}

// ---------------------------------------------------------------------------
void Result::ComputeSensorClass(int _zone, int _sensor)
{
	double v = AbsMaxV(FilteredOut_Data[_zone][_sensor], _zone);
	if (v < 0)
		sensor_data[_zone][_sensor] = -1;
	else
		sensor_data[_zone][_sensor] = LevelClass(gain[_sensor] * v);
}

// ---------------------------------------------------------------------------
double Result::AbsMaxV(vector<double>_v, int _zone)
{
	double ret = 0;
	double v;
	int sz = _v.size();
	bool Ok = false;
	for (int i = 0; i < sz; i++)
	{
		if (Deads[_zone][i])
			continue;
		Ok = true;
		v = _v[i];
		if (v < 0)
			v = -v;
		if (ret < v)
			ret = v;
	}
	if (Ok)
		return ret;
	else
		return -1;
}

// ---------------------------------------------------------------------------
double Result::LevelClass(double _level)
{
	double cl = 0;
	if (borders.get_length() == 1)
	{
		if (_level < borders[0])
			cl = 2;
		else
			cl = 0;

	}
	else if (borders.get_length() == 2)
	{
		if (_level < borders[1])
			cl = 2;
		else if (_level < borders[0])
			cl = 1;
		else
			cl = 0;
	}
	return (cl);
}

//void Result::SaveTubeToFile(UnicodeString FileName)
void Result::SaveTubeToFile(FILE* file)
{
	// Формат файла .dkb для дефектоскопа
	// 1/0 метка RunTime/Normal
	// zones,sensors
	// meas_per_zone / meas_per_sensor[]
	// zone_data[]
	// sensor_data[][]
	// Source_Data[][][]

//	FILE *file;
//	file = fopen(AnsiString(FileName).c_str(), "a");
//	if (file == NULL)
//		Application->MessageBoxW(L"Не удалось открыть файл для записи",
//		L"Ошибка", MB_ICONERROR | MB_OK);

	fprintf(file, "%d", 1); // Ставим метку, если это RunTime труба
	fprintf(file, "\n");
	fprintf(file, "%d %d ", zones, sensors_a); // кол-во зон,датчиков

	// Если дефектоскоп RunTime, то пишем длину каждой из зон
	for (int i = 0; i < zones; i++)
		fprintf(file, "%d ", Deads[i].size());

	// пишем данные по зонам
	fprintf(file, "\n");
	for (int i = 0; i < zones; i++)
		fprintf(file, "%f ", zone_data[i]);
	fprintf(file, "\n");
	// пишем данные по датчикам
	for (int i = 0; i < zones; i++)
		for (int j = 0; j < sensors_a; j++)
			fprintf(file, "%f ", sensor_data[i][j]);

	// пишем все собранные данные
	fprintf(file, "\n");
	for (int j = 0; j < zones; j++) // Зона
	{
		fprintf(file, "\n");
		for (int i = 0; i < sensors_a; i++) // Датчик
		{
			// Если RunTime, то в каждой зоне разное кол-во измерений
			for (int k = 0; k < (int)Source_Data[j][i].size(); k++)
				fprintf(file, "%f ", Source_Data[j][i][k]);
		}
	}

//	fprintf(file, "%d", 1); // Ставим метку, если это RunTime труба
//	fprintf(file, "\n");
//	fprintf(file, "%d %d ", zones, sensors_a); // кол-во зон,датчиков
//
//	// Если дефектоскоп RunTime, то пишем длину каждой из зон
//	for (int i = 0; i < zones; i++)
//		fprintf(file, "%d ", Deads[i].size());
//
//	// пишем данные по зонам
//	fprintf(file, "\n");
//	for (int i = 0; i < zones; i++)
//		fprintf(file, "%4.1f ", zone_data[i]);
//	fprintf(file, "\n");
//	// пишем данные по датчикам
//	for (int i = 0; i < zones; i++)
//		for (int j = 0; j < sensors_a; j++)
//			fprintf(file, "%4.1f ", sensor_data[i][j]);
//
//	// пишем все собранные данные
//	fprintf(file, "\n");
//	for (int j = 0; j < zones; j++) // Зона
//	{
//		fprintf(file, "\n");
//		for (int i = 0; i < sensors_a; i++) // Датчик
//		{
//			// Если RunTime, то в каждой зоне разное кол-во измерений
//			for (int k = 0; k < (int)Source_Data[j][i].size(); k++)
//				fprintf(file, "%4.1f ", Source_Data[j][i][k]);
//		}
//	}
//	fclose(file);
}

// ------------------------------------------------------------------------------
void Result::LoadTubeFromFile(FILE* file)
{
//	int aaa = sensors_a;
	int XisRunTime = 0;
	fscanf(file, "%d", &XisRunTime);
	fscanf(file, "%hd%hd", &zones, &sensors); // Кол-во зон и датчиков

	// Если дефектоскоп RunTime, то читаем массив длин каждой зоны
	int tmp;
	vector<int>meas_per_sensor;
	meas_per_sensor.resize(zones);
	for (int i = 0; i < zones; i++)
		fscanf(file, "%d", &meas_per_sensor[i]);
	// Читаем данные по зонам
	for (int i = 0; i < zones; i++)
		fscanf(file, "%lf ", &zone_data[i]);

	// Читаем данные по датчикам
	for (int i = 0; i < zones; i++)
		for (int j = 0; j < sensors; j++)
			fscanf(file, "%lf", &sensor_data[i][j]);

	// Читаем все собранные данные
	for (int j = 0; j < zones; j++)
	{
		for (int i = 0; i < sensors; i++)
		{
			Source_Data[j][i].resize(meas_per_sensor[j]);
			for (int k = 0; k < meas_per_sensor[j]; k++)
			{
				fscanf(file, "%lf ", &Source_Data[j][i][k]);
			}
		}
	}
	// Файл не закрываем, он будет закрыт в главной функции передающий указатель на открытый файл
}

// ---------------------------------------------------------------------------
void Result::LoadSettings(void)
{
	WidthMedianFilter = ini->ReadInteger("OtherSettings",
		"WidthMedianFilter", 5);
	isMedianFilter = ini->ReadBool("OtherSettings", "isMedianFilter", 0);
}

// ---------------------------------------------------------------------------
// Альметьевск
// ! Возвращает отфильтрованные данные
vector<vector<vector<double> > >Result::GetFilteredOut_Data()
{
	return FilteredOut_Data;
}

// ---------------------------------------------------------------------------
vector<vector<vector<double> > >Result::GetFilteredIn_Data()
{
	return FilteredIn_Data;
}

// ---------------------------------------------------------------------------
// ! Возвращает коэффициенты усиления датчиков
vector<double>Result::GetGain()
{
	return gain;
}
// ----------------------------------------------------------------------------
double Result::GetSensorGain(int sensor)
{
	if(sensor > gain.size())
		return 0.0;
	return gain[sensor];
}
// ---------------------------------------------------------------------------
// ! Возвращает данные о мертвых зонах
vector<vector<bool> >Result::GetDeads()
{
	return Deads;
}

// ---------------------------------------------------------------------------
// ! Возвращает длину зоны
int Result::GetZoneLength()
{
	return zone_length;
}

// ---------------------------------------------------------------------------
void Result::FillDataR(vector<vector<double> > * _SD)
{
	for (int z = 0; z < zones; z++)
	{
		for (unsigned int s = 0; s < (*_SD).size(); s++)
		{
			int MeasPerZone = (*_SD)[s].size() / zones;
			Source_Data[z][s].clear();
			Source_Data[z][s].reserve((*_SD)[s].size());
			for (unsigned int i=0; i < MeasPerZone; i++)
				Source_Data[z][s].push_back((*_SD)[s][i + z*MeasPerZone]);
		}
	}
}
// ------------------------------------------------------------------------------
void Result::Calibrate(int _zone, int _sensor,bool* _mb)
{
	double smax = GetMaxZoneSensor(_zone, _sensor);
	double sgain = gain[_sensor];
	double k = sgain * smax;
	for (int s = 0; s < sensors_a; s++)
	{
		if(_mb[s])
			gain[s] = RoundTo(k / GetMaxZoneSensor(_zone, s), -2);
	}
}
// ------------------------------------------------------------------------------
double Result::GetMaxZoneSensor(int _zone, int _sensor)
{
	double ret = 0;
	double val;
	for (int j = 0; j < (int)FilteredOut_Data[_zone][_sensor].size(); j++)
	{
		val = FilteredOut_Data[_zone][_sensor][j];
		if (val < 0)
			val = -val;
		if (ret < val)
			ret = val;
	}
	return (ret);
}
// ---------------------------------------------------------------------------

void Result::EtalonBorderSet(int _StartZone, int _FinalZone, TEtalonNewDefectForm* _Owner
	, TIniFile* _ini)
{
	TPr::pr("Result::EtalonBorderSet");
	TEtalonBorderSet *EtalonBorderSetForm = new TEtalonBorderSet(_Owner, _ini, "");
	int sn = 0;
	EtalonBorderSetForm->SetCaption("Датчик " + IntToStr(sn + 1) + ", зоны с " +
		IntToStr(_StartZone + 1) + " по " + IntToStr(_FinalZone + 1));
	PutDataOnChartEtalonBorder(EtalonBorderSetForm, _StartZone, _FinalZone, sn);
	EtalonBorderSetForm->ShowModal();
	delete EtalonBorderSetForm;
}
// ---------------------------------------------------------------------------
void Result::PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
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

	//_F->SetCurrPoint(StartZone,FinalZone,sensor,false);
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
