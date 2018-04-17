#ifndef SummaryResultH
#define SummaryResultH
#include "ResultBase.h"
#include <Chart.hpp>
#include "uTFRSum.h"
class SummaryResult : public ResultBase
{
private:
	TFRSum* FRM;
public:
	//! Конструктор
	SummaryResult(int _sensors,TIniFile* _ini);
	//! Деструктор
	~SummaryResult();
	//! номер зоны реза 1 и 2
	short cut1,cut2;
	//! минимальный годный участок
	short min_good_length;
	//! отрезок с обоих концов трубы
	short end_cut;
	//! решение о трубе (Годно, Брак, Класс 2)
	String decision;
	//! Возвращает цвет зоны
	TColor ZoneColor(double meas);
	// Рисует результат на чарте
	void PutResultOnChart(void);
	//! Принимает решение по всей трубе и вычисляет зоны отреза
	void MakeDecision();
	void MakeDecision2();
	void Bind(TFRSum* _FRM);
};
#endif
