//---------------------------------------------------------------------------
#ifndef CrossResultH
#define CrossResultH
#include "Result.h"
#include "uTFRCross.h"
class CCrossResult:public Result
{
private:
	void LoadSettings0(void);
	TFRCross* FRM;
	void OnDblClick(int _zone,int _sensor,bool _IsCtrl);
public:
	CCrossResult(int sensors,TIniFile* _ini,TComponent* _Owner);
	void PutSourceDataOnChartX(TFViewZoneS* _F, int zone, int sensor);
	void PutDataOnChartX(TFViewZoneD* _F, int initZone, int sensor);
	void PutResultOnChart(void);
	void LoadSettings(void);
	void PutTest(TChart *C);
	void Bind(TFRCross* _FRM);
	void PutResultOnChart(TChart *C);
	inline void ComputeZoneData(void)
	{
		Result::ComputeZoneData(false);
	}
	void PutDataOnChartEtalonBorder(TEtalonBorderSet* _F, int StartZone
		, int FinalZone, int sensor);
};
#endif
