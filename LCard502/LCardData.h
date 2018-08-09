#ifndef LCardDataH
#define LCardDataH
#include <vector>
#include <IniFiles.hpp>
#include "RLCard502.h"
using namespace std;

// ---------------------------------------------------------------------------
class LCardData
{
private:
	// датчик.измерение
	public:
	vector<vector<double> >LZone;
	vector<vector<double> >CZone;
	private:
	bool IsCrossStarted;
	bool IsLineStarted;
	RLCard502* lcard;

	vector<double>CVoltPercent;
	vector<double>LVoltPercent;
	TIniFile *ini;
	RLCard_parameters Parameters;

public:
	AnsiString LastError;

	LCardData(TIniFile* _ini);
	~LCardData();
	void LoadSettings(void);
	void StartCross(void);
	void StopCross(void);
	void StartLine(void);
	void StopLine(void);
	double GetValue(int _ch);

	inline vector<vector<double> > * GetPointLine()
	{
		return (&LZone);
	}

	inline vector<vector<double> > * GetPointCross()
	{
		return (&CZone);
	}
	void ClearCross(void);
	void ClearLine(void);
	bool Read(void);
};

extern LCardData* lcard;
// ---------------------------------------------------------------------------
#endif
