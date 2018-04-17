#ifndef CLinearResultH
#define CLinearResultH
#include "Result.h"
#include "uTFRLine.h"

class CLinearResult : public Result
{
private:
	bool inner;
	void PutSourceDataOnChartX(TFViewZoneS* _F, int zone, int sensor);
	void ComputeSensorClass(int _zone, int _sensor);
	double LevelClass(double _level0,double _level1);
	void LoadSettings0(void);
	TFRLine* FRM;
	void OnDblClick(int _zone,int _sensor,bool _IsCtrl);
public:
	void PutDataOnChartX(TFViewZoneD* _F, int zone, int sensor);
	CLinearResult(int sensors, TIniFile* _ini, TComponent* _Owner);
	void PutResultOnChart(TChart *C);
	void PutResultOnChart(void);

	// ! массив порогов - [0] - порог брака, [1] - порог класса 2  (%)
	DynamicArray<double>bordersIn;
	inline bool IsInner(void)
	{
		return (inner);
	}
	void LoadSettings(void);
	// ! возвращает цвет измерения/датчика/зоны
	TColor ZoneColorIn(double meas, bool _IsDead);
	void PutDataOnFrame(TFRViewZone* _fr, int zone, int sensor, TColor zoneColor);
	void Bind(TFRLine* _FRM);
	inline void ComputeZoneData(void)
	{
		Result::ComputeZoneData(true);
	}
	void PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
		, int FinalZone, int sensor);
};
#endif
