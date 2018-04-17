// ---------------------------------------------------------------------------
#ifndef ResultH
#define ResultH
#include "ResultBase.h"
#include <vector>
#include "uFViewZoneS.h"
#include "uFViewZoneD.h"
#include "uFRViewZone.h"
#include "CBank.h"
#include "EtalonBorderSet.h"
using namespace std;

// ! Класс результата дефектоскопии
class Result : public ResultBase
{
	friend class ButterworthFilter;

private:
	// ! Вспомогательная функция для фильтрации
	void _medianfilter(double* signal, double* result, int N);

	// ! Ширина окна для медианной фильтрации
	int WidthMedianFilter;
	// ! если 0 то простой просмотр, если 1 исходный
	int TypeView;
	// ! зона начала вывода результата
	int shiftZone;
	// ! номер датчика
	int sens;
	// ! зона начала вывода результата для обсчета СтатусБара
	int startZone;
	// ! Количество зон вывода результата
	int countZone;

	void ComputeFiltered(int _zone, int _sensor, bool _isLinear);

	void CalcDeadsZoneBegin(int _ZoneLength, int _Begin, int _zone,
		int _ZoneSize);
	void CalcDeadsEnd(int _ZoneLength, int _End, int _zones);

protected:
	// ! Конструктор
	Result(int _sensors, TIniFile* _ini, TComponent* _Owner);

	// ! Форма для отрисовки измерений по датчику (0-сигнал по модулю, 1-исходный сигнал)
	TFViewZoneD* SensorFormD;
	TFViewZoneS* SensorFormS;
	// ! кол-во измерений первой мертвой зоны
	double meas_dead_zone_start;
	// ! кол-во измерений второй мертвой зоны
	double meas_dead_zone_finish;
	vector<double>gain;

	virtual void PutDataOnChartX(TFViewZoneD* _F, int zone, int sensor);

	virtual void PutSourceDataOnChartX(TFViewZoneS* _F, int zone, int sensor)
	{
	};

	virtual	void PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
		, int FinalZone, int sensor);

	vector<vector<vector<double> > >Median_Data;
	// ! Длина зоны
	int zone_length;
	// ! вектор отфильтрованых значений - по всем датчикам , по всем зонам [зона][датчик][измерение]
	vector<vector<vector<double> > >FilteredOut_Data;
	vector<vector<vector<double> > >FilteredIn_Data;

	virtual void ComputeSensorClass(int _zone, int _sensor);
	double AbsMaxV(vector<double>_v, int _zone);
	double LevelClass(double _level);
	// ! Двойной клик по датчику открывает окно с измерениями
	void __fastcall SeriesDblClick(TChartSeries *Sender, int ValueIndex,
		TMouseButton Button, TShiftState Shift, int X, int Y);

	// зона.датчик.измерение
	vector<vector<vector<double> > >Source_Data;
	bool isMedianFilter;

	// ! считает данные, заполняя массивы "zone_data" и "sensor_data"
	void ComputeZoneData(bool isLinear);
	void LoadSettings(void);
public:
	// ! Деструктор
	virtual ~Result();
	// ! @brief Медианный фильтр
	// ! @param signal - исходный сигнал
	// ! @param result - отфильтрованый сигнал
	// ! @param N - длина массива
	void medianfilter(double* signal, double* result, int N);

	// ! Мертвая зона в начале трубы(mm.)
	int dead_zone_start;
	// ! Мёртвая зона в конце трубы(mm.)
	int dead_zone_finish;

	// ! Распределяет данные по вектору data из буфера АЦП
	void FillData(vector<vector<double> >sourceData);
	// ! Распределяет данные по вектору data из буфера АЦП
	void FillDataR(vector<vector<double> > * _SD);
	// ! возвращает цвет измерения/датчика/зоны
	TColor ZoneColor(double meas, bool _IsDead);
	// ! Удалить все измерения
	void DeleteData();
	// ! Устанавливает все усиления датчиков
	void SetGains(vector<double>_Gain);
	// ! Устанавливает усиление 1 конкртеного датчика
	void SetSensorGain(double _gain, int sensor);
	// ! Добавляет рассчитанную зону в реальном времени
	// ! Вернет True если зона бракованая, иначе false
	bool AddZone(vector<vector<double> >new_data, bool isLinear = false);
	// ! Автоматичекая калибровка поперечных датчиков
	void AutoCalibration(int zone);
	void PutDataOnFrame(TFRViewZone* _fr, int zone, int sensor,
		TColor zoneColor);
	// ! Сохраняем данные по трубе в файл (текстовый)
//	void SaveTubeToFile(UnicodeString FileName);
	void SaveTubeToFile(FILE *file);
	// ! Загрузка данных по трубе из файла (текстового)
	void LoadTubeFromFile(FILE *file);
	bool AddBankZone(CBank* _bank, bool _isLinear);
	// Альметьевск
	// ! Возвращает отфильтрованные данные
	vector<vector<vector<double> > >GetFilteredOut_Data();
	vector<vector<vector<double> > >GetFilteredIn_Data();
	// ! Возвращает коэффициенты усиления датчиков
	vector<double>GetGain();
	double GetSensorGain(int sensor);
	// ! Возвращает данные о мертвых зонах
	vector<vector<bool> >GetDeads();
	// ! Возвращает длину зоны
	int GetZoneLength();
	// -----
	//выборочная автокалибровка датчиков
	void Calibrate(int _zone, int _sensor,bool* _mb);
	double GetMaxZoneSensor(int _zone, int _sensor);
	//ручная установка порогов эталона
	void EtalonBorderSet(int _StartZone, int _LastZone, TEtalonNewDefectForm* _Owner
		, TIniFile* _ini);

};
#endif
