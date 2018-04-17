#ifndef RChannel_parametersH
#define RChannel_parametersH
//---------------------------------------------------------------------------
class RChannel_parameters
{
public:
	RChannel_parameters()
	{
		range = 0;
		logicalChannel = 0;
		collectedMode = 0;
	}
	// ! @brief Входной диапазон
	// ! @li 0 Диапазон +/-10V
	// ! @li 1 Диапазон +/-5V
	// ! @li 2 Диапазон +/-2V
	// ! @li 3 Диапазон +/-1V
	// ! @li 4 Диапазон +/-0.5V
	// ! @li 5 Диапазон +/-0.2V
	unsigned int range;
	// ! Номер логического канала 0-31
	unsigned int logicalChannel;
	// ! @brief Режим сбора
	// ! @li 0 Измерение напряжения относительно общей земли
	// ! @li 1 Дифференциальное измерение напряжения
	// ! @li 2 Измерение собственного нуля
	unsigned int collectedMode;
};
//---------------------------------------------------------------------------
#endif
