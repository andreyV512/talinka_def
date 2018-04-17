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

	// ! ������ ���������� ����������� ������
	CCrossResult *CrossResult;
	// ! ������ ���������� ����������� ������
	CLinearResult* LinearResult;
	// ! ������ ���������� �����������
	ThicknessResult *ThResult;
	// ! �������� ���������
	SummaryResult *SumResult;

	// ! ������� ��������� ��������� �� ���� �������, ��������� ������ zone_data
	void ComputeZonesData();
	// ! ��������� ���� � �������� �������
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
