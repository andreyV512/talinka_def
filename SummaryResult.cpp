#pragma hdrstop
#include "SummaryResult.h"
#include "Global.h"
#include <Series.hpp>
#include "Protocol.h"
#pragma package(smart_init)

SummaryResult::SummaryResult(int _sensors, TIniFile* _ini)
	: ResultBase((short)_sensors, _ini)
{
	SummaryResult::zones = 0;
	cut1 = cut2 = 0;

	decision = "";
}

// ---------------------------------------------------------------------------
SummaryResult::~SummaryResult()
{
	SummaryResult::zone_data.~DynamicArray();
}

// ---------------------------------------------------------------------------
TColor SummaryResult::ZoneColor(double meas)
{
	// возвращает цвет зоны в зависимости от суммарного результата в этой зоне
	if (meas == 1)
		return Good;
	else if (meas == 0)
		return Brack;
	else
		return Class2;
}

// ---------------------------------------------------------------------------
void SummaryResult::PutResultOnChart(void)
{
	// рисует результат по зонам на чарте С;
	for (int j = 0; j <= Globals::max_zones; j++)
	{
		if (j < zones)
			FRM->Chart1->Series[0]->AddXY(j, 1, "", ClassColor(zone_data[j]));
		else
			FRM->Chart1->Series[0]->AddXY(j, 0, "", clWhite);
	}
}

// ---------------------------------------------------------------------------
void SummaryResult::MakeDecision()
{
	// принимает решение по трубе, заполняет значения резов
	bool flag = false;
	// сигнализирует, что текущий участок будет помечен 2-ым классом, если превысит минимальную годную длину
	bool class2 = false; // участок точно будет помечен 2-ым классом
	short temp = 0; // длина текущего участка в зонах
	short tcut1 = 0; // временный первый рез
	cut1 = cut2 = 0; // резов по умолчанию нет
	decision = ""; // решение не принято
	if (min_good_length > 0)
	{
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i] == 2)
				temp++;
			else if (zone_data[i] == 0)
			{
				if (temp >= min_good_length)
				{
					cut2 = (short) i;
					cut1 = (short) i - temp;
					class2 = flag;
				}
				temp = 0;
				tcut1 = i + 1;
				flag = false;
			}
			else if (zone_data[i] == 1)
			{
				flag = true;
				temp++;
			}
		}

		if (temp < min_good_length && cut2 == 0 && cut1 == 0)
			decision = "Брак";
		else if (temp >= min_good_length)
		{
			if (flag)
				decision = "Класс 2";
			else
				decision = "Годно";
			cut1 = tcut1;
			cut2 = 0;
		}
		else if (cut1 > 0 || cut2 > 0)
		{
			if (class2)
				decision = "Класс 2";
			else
				decision = "Годно";
		}

		// ебаный пыть-ях блеать
		if (end_cut > 0 && decision != "Брак")
		{
			if (cut2 == 0)
				cut2 = zones;
			int f1 = (end_cut - cut1 > 0) ? (end_cut - cut1) : (0);
			int f2 = (end_cut - zones + cut2 > 0) ? (end_cut - zones + cut2) : (0);
			if (cut2 - cut1 - f1 - f2 < min_good_length)
				decision = "Брак";
		}
		// Примочка для Казахстана
		if (cut1 > 0)
			cut1++;
	}
	// работа по зеленке - любой брак - сразу косяк
	else
	{
		bool brack = false;
		for (int i = 0; i < zones; i++)
		{
			if (zone_data[i] == 0)
			{
				brack = true;
				break;
			}
			else if (zone_data[i] == 1)
				class2 = true;
		}
		if (brack)
			decision = "Брак";
		else if (class2)
			decision = "Класс 2";
		else
			decision = "Годно";
	}
}
void SummaryResult::MakeDecision2()
{

}

// ---------------------------------------------------------------------------
void SummaryResult::Bind(TFRSum* _FRM)
{
	FRM = _FRM;
}
