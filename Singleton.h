#ifndef SingletonH
#define SingletonH
#include "CLinearResult.h"
#include "CrossResult.h"
#include "ThicknessResult.h"
#include "SummaryResult.h"

class CSingleton
{
private:
	bool inner;

public:
	CSingleton(TIniFile* _ini,TComponent* _Owner);
	~CSingleton();

	// ! Объект результата поперечного модуля
	CCrossResult *CrossResult;
	// ! Объект результата продольного модуля
	CLinearResult* LinearResult;
	// ! Объект результата толщиномера
	ThicknessResult *ThResult;
	// ! Сумарный результат
	SummaryResult *SumResult;

	// ! Считает суммарный результат по всем модулям, заполянет массив zone_data
	void ComputeZonesData();
	// ! добавляет зону в реальном времени
	void AddZone();

	inline bool IsInner(void)
	{
		return (inner);
	};
	void LoadSettings(void);
	bool FromFile;
	bool isSOP;
};

extern CSingleton* Singleton;
#endif
