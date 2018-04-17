// ---------------------------------------------------------------------------

#ifndef GlobalH
#define GlobalH

#include <Classes.hpp>
// ---------------------------------------------------------------------------
/*
enum SGSource
{
	sgsLCard=0,sgsCOMPort=1,sgsNone=-1
};
*/
// ! Класс глобальных переменных
class Globals
{
public:
	// ! Текущий диаметр (строго 60,73 или 89)
	static int current_diameter;
	// ! Имя и путь к файлу с настройками
	static String IniFileName;
	// ! Путь к файлу с порогами для гп с ms-dos
	static String PathFile_threshold_sg;
	// ! Номер трубы
//	static AnsiString tube_number;
	// ! Номер трубы
//	static bool tube_transit;

	// ! @deprecated устарело, нужно все переводить на векторы
	// ! @brief Макс кол-во датчиков, устарело наверное, нужно выкуривать
	static const int max_sensors=16;
	// ! @deprecated устарело, нужно все переводить на векторы
	// ! @brief Макс кол-во зон, устарело, выкуривать
	static const int max_zones=60;
	// ! Число датчиков продольного контроля в проекте
//	static const int LinSensors=4;
	// ! Максимальное число датчиков поперечного контроля в проекте
//	static const int Cross_sensors=12;
	// ! параметр частотника в котором хранится скорость врщения
	static const int defaultRotParameter=4;
private:

};

int Globals::current_diameter=2;
String Globals::PathFile_threshold_sg="..\\..\\Settings\\";
String Globals::IniFileName="..\\..\\Settings\\SettingsDefectoscope.ini";
//AnsiString Globals::tube_number="";
//bool Globals::tube_transit;

// -------------------------------------------------------------------------------------------
// ! Класс констант, относящихся к особенностям проекта
class SystemConst
{
public:
	// !	Прятать ли усиления каналов и отключение модулей
	static bool HideInGain;
	// ! Будем ли фильтровать сигнал поперечного цифровым фильтром
	static bool isCrossDigitalFilter;
	// ! Будем ли фильтровать сигнал продольного цифровым фильтром
	static bool isLinearDigitalFilterIn;
	static bool isLinearDigitalFilterOut;

	// ! Установим постоянные характеристики проекта
	static void SetSystemConstants();

};
bool SystemConst::isCrossDigitalFilter=true;
bool SystemConst::isLinearDigitalFilterIn=true;
bool SystemConst::isLinearDigitalFilterOut=true;
//bool SystemConst::ComWithASU=false;
bool SystemConst::HideInGain=false;

// ---------------------------------------------------------------------------
#endif
