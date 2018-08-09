// ---------------------------------------------------------------------------
#ifndef CBankH
#define CBankH
#include <SyncObjs.hpp>
#include <vector>
using namespace std;

// ---------------------------------------------------------------------------
class CBank
{
private:
	TCriticalSection* cs;
	//зона.датчик.измерение
public:
	vector<vector<vector<double> > >Source;
public:
	unsigned int zones;
private:
	unsigned int sensors;
public:
	unsigned int last;

public:
	CBank(int _max_zones, int _sensors);
	~CBank(void);
	void Clear(void);
	// датчик.измерение
	void AddZone(vector<vector<double> >* new_data);
	//зона.датчик.измерение
	int GetNextZone(vector<vector<vector<double> > > * _source);
};
#endif
