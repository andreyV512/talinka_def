#ifndef RLCard_parametersH
#define RLCard_parametersH
#include <vector>
#include <l502api.h>
#include "RChannel_parameters.h"
#include <IniFiles.hpp>
using namespace std;

// ---------------------------------------------------------------------------
class RLCard_parameters
{
private:
public:
	// ! ТаймАут для сбора (с каким периодом будем скидывать данные из ацп в большой буфер,мс)
	int RECV_TOUT;
	// ! @brief Режим синхронизации
	// ! @li 0 Внутренний сигнал
	// ! @li 1 От внешнего мастера по разъему синхронизации
	// ! @li 2 По фронту сигнала DI_SYN1
	// ! @li 3 По фронту сигнала DI_SYN2
	// ! @li 6 По спаду сигнала DI_SYN1
	// ! @li 7 По спаду сигнала DI_SYN2
	uint32_t syncMode;
	// ! Источник запуска синхронного ввода/вывода см. режимы @link syncMode
	uint32_t syncStartMode;
	// ! Частота сбора (Гц.)
	double frequencyCollect;
	// ! Частота на канал (Гц.)
	double frequencyPerChannel;

	// ! Возвращает кол-во используемых каналов
	int getCountChannels()
	{
		return channels.size() + others.size();
	}

	// ! Индивидуальные настройки для каждого канала
	vector<RChannel_parameters>channels;
	// ! @brief Дополнительные каналы
	// ! @li 0 - Ток продольный соленоид
	// ! @li 1 - Напряжение продольный соленоид
	// ! @li 2 - Ток поперечный соленоид
	// ! @li 3 - Напряжение поперечный соленоид
	// ! @li 4 - Ток группы прочности
	// ! @li 5 - Напряжение группы прочности
	// ! @li 6 - Разделение по зонам
	vector<RChannel_parameters>others;

	int LSensors;
	int CSensors;
	int OSensors;
	static const int others_sensors_count = 3;
	// ! Конструктор
	RLCard_parameters()
	{
		others.resize(others_sensors_count);
	}
	void LoadSettings(TIniFile* _ini);

};
// ---------------------------------------------------------------------------
#endif
