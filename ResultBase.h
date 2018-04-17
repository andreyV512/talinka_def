#ifndef ResultBaseH
#define ResultBaseH
#include <IniFiles.hpp>
#include <Graphics.hpp>
#include <vector>
using namespace std;

// ! Класс результата дефектоскопии
class ResultBase
{
	friend class ButterworthFilter;

private:
protected:
	// ! Конструктор
	ResultBase(short _sensors, TIniFile* _ini);

	TIniFile* ini;
	// ! массив итоговых значений по датчикам
	vector<vector<double> >sensor_data;
	// ! Вектор исходных значений - по всем датчикам , по всем зонам [зона][датчик][измерение]
	vector<vector<bool> >Deads;
	int ZoneLength;

public:
	// ! Деструктор
	virtual ~ResultBase();

	// ! Кол-во отображаемых зон в результате
	short zones;
	// ! массив итоговых значений по зонам
	DynamicArray<double>zone_data;

	// ! Удалить все измерения
	void DeleteData();

	// ! Кол-во измерений в 1 зоне 1 датчике
	int meas_per_zone;
	// ! Кол-во сенсоров (датчиков)
	short sensors;
	// ! Кол-во активных сенсоров (датчиков)
	short sensors_a;
	// ! Цвет мертвой зоны
	TColor Dead;
	// ! Цвет брака
	TColor Brack;
	// ! Цвет второго класса
	TColor Class2;
	// ! Цвет годной, хорошей трубы
	TColor Good;

	TColor ClassColor(double _cl);

	// ! массив порогов - [0] - порог брака, [1] - порог класса 2  (%)
	DynamicArray<double>borders;

	inline vector<vector<bool> >GetDeads()
	{
		return (Deads);
	};

	inline int GetZoneLength()
	{
		return (ZoneLength);
	}
};
#endif
