// ---------------------------------------------------------------------------
#ifndef RLCard502H
#define RLCard502H
#include "RLCard_parameters.h"
#include "l502api.h"
#pragma comment(lib, "l502api.lib")
#define LCard502_INIT_SIZE 2000
#define LCard502_ADD_PERCENT 100

// ---------------------------------------------------------------------------
class RLCard502
{
private:
	// ! Хендл платы
	t_l502_hnd handle;
	int raw_size;
	unsigned int* rawi;
	double* raw;
	bool IsStarted;
	// ! ТаймАут для сбора (с каким периодом будем скидывать данные из ацп в большой буфер,мс)
	int RECV_TOUT;
	int MainSensors;
	int OtherSensors;

	void SetRawSize(int _size);
	void LFATAL(AnsiString _msg, int _err);
	bool CheckError(int _err);
	void LoadMainSettings();

	RLCard_parameters* Parameters;

public:
	AnsiString LastError;

	RLCard502();
	~RLCard502(void);
	void Start(void);
	void Stop(void);
	double* Read(int* _size);
	double GetValue(int _ch);
	void LoadOtherSettings();

	inline void LoadSettings(RLCard_parameters* _Parameters)
	{
		Parameters = _Parameters;
	}
};
#endif
