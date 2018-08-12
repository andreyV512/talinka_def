//---------------------------------------------------------------------------


#pragma hdrstop

#include "Save.h"
#include "Protocol.h"
#include "uFunctions.h"
#include "uCExecSQL.h"
#include "uCExecute.h"
#include "uCSelect.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "ADCSettings.h"
#include "Singleton.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
// ---------------------------------------------------------------------------
void SaveTubeToDB(TIniFile* _ini)
{
    TIniFile* ini = _ini;
	AnsiString SQL;
	AnsiString ret;

	CExecSQL* E;
	// Проверяем число записей
	CSelect S1("SELECT COUNT(TubeNum) as nn from dbo.TubesStat");
	if (S1.AsInt("nn") > 60000)
	{
		// Находим самую старую запись
		CSelect S2("Select top 1 * from dbo.TubesStat order by IDTube ASC");
		// Удаляем старые записи с этим номером

		SQL = "Delete from dbo.ZonesStat where TubeNum=";
		SQL += S2.AsInt("TubeNum");
		E = new CExecSQL(SQL);
		delete E;

		SQL = "Delete from dbo.TubesStat where TubeNum=";
		SQL += S2.AsInt("TubeNum");
		E = new CExecSQL(SQL);
		delete E;
	}

	// Ищем последний номер трубы
	CSelect S3("Select top 1 * from dbo.TubesStat order by IDTube Desc");
	// Выбираем новый номер трубы
	int TubeNum = 0;
	TubeNum = S3.AsInt("TubeNum");
	if (TubeNum == 0 || TubeNum == 200000)
		TubeNum = 1;
	else
		TubeNum++;

	// Удаляем старые записи с этим номером
	SQL = "Delete from dbo.ZonesStat where TubeNum=";
	SQL += TubeNum;
	E = new CExecSQL(SQL);
	delete E;

	SQL = "Delete from dbo.TubesStat where TubeNum=";
	SQL += TubeNum;
	E = new CExecSQL(SQL);
	ret = E->IsOk();
	delete E;

	// Вводим новую запись трубы
	SQL = "insert into dbo.TubesStat (TubeNum,DateTime,TypeSize,Result,SolidGroup,";
	SQL += "ThickBrackBorder,NumOfZones,Cut1,Cut2,Defectoscoper";
	//---
	SQL += ",C11Reas,C12Reas,C13Reas,C21Reas,C22Reas,C23Reas,Mod1Brack,Mod2Brack,Mod3Brack";
	//---
	SQL += ") values(";
	SQL += TubeNum;
	SQL += ",'";
	SQL += DateTimeToStr(Date() + Time());
	SQL += "','";
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	SQL += ini->ReadString("Default", "TypeSize", "1");
	SQL += "','";
	SQL += Singleton->SumResult->decision;
	SQL += "','";
	SQL += (char)Singleton->currentSolidGroup;//"?"; //todo вставить Группу прочности
	SQL += "',";
	AnsiString ss;
	double minThick = Singleton->ThResult->MinThick(Singleton->SumResult->cut1
		, Singleton->SumResult->cut2);
	if(minThick < 0 || minThick > 10.00)
		minThick = 10.00;
	ss.printf("%0.2f,", minThick);
	SQL += ss;
	SQL += Singleton->SumResult->zones;
	SQL += ",";
	SQL += Singleton->SumResult->cut1;
	SQL += ",";
	SQL += Singleton->SumResult->cut2;
	SQL += ",'";
	SQL += ini->ReadString("Default", "LastAuthorisation", "?");
	//---
	SQL += "',";
	if(Singleton->SumResult->cut1 >= 1)
	{
		SQL += CheckCutReason(Singleton->SumResult->cut1 - 1,1);  //Поправка на 1 зону
		SQL += ",";
		SQL += CheckCutReason(Singleton->SumResult->cut1 - 1,2);  //Поправка на 1 зону
		SQL += ",";
		SQL += CheckCutReason(Singleton->SumResult->cut1 - 1,3);  //Поправка на 1 зону
	}
	else SQL += "0,0,0";
	SQL += ",";
	if(Singleton->SumResult->cut2 >= 1)
	{
		SQL += CheckCutReason(Singleton->SumResult->cut2 + 1,1);
		SQL += ",";
		SQL += CheckCutReason(Singleton->SumResult->cut2 + 1,2);
		SQL += ",";
		SQL += CheckCutReason(Singleton->SumResult->cut2 + 1,3);
	}
	else SQL += "0,0,0";
	SQL += ",";
	SQL += CheckModBrack(1);
	SQL += ",";
	SQL += CheckModBrack(2);
	SQL += ",";
	SQL += CheckModBrack(3);
	//---
	SQL += ")";
	TPr::pr(SQL);
	E = new CExecSQL(SQL);
	ret = E->IsOk();
	delete E;
	if (ret != "Ok")
	{
		TPr::pr(AnsiString("SaveTubeToDB: ") + ret);
		wchar_t wstr[500];
		ret.WideChar(wstr,500);
		Application->MessageBoxW(wstr, L"Ошибка", MB_ICONERROR | MB_OK);
	}
	else
	{
		// Ищем номер новой трубы
		CSelect S4("Select top 1 * from dbo.TubesStat order by IDTube Desc");
		int IDTube = 0;
		IDTube = S4.AsInt("IDTube");
		if (IDTube > 0)
		{
			// Вводим новые записи по зонам
			// Поперечник
			SQL = "insert into dbo.ZonesStat (TubeID,TubeNum,Module,";
			SQL += "Zone1,Zone2,Zone3,Zone4,Zone5,Zone6,Zone7,Zone8,Zone9,Zone10,";
			SQL += "Zone11,Zone12,Zone13,Zone14,Zone15,Zone16,Zone17,Zone18,Zone19,Zone20,";
			SQL += "Zone21,Zone22,Zone23,Zone24,Zone25,Zone26,Zone27,Zone28,Zone29,Zone30,";
			SQL += "Zone31,Zone32,Zone33,Zone34,Zone35,Zone36,Zone37,Zone38,Zone39,Zone40,";
			SQL += "Zone41,Zone42,Zone43,Zone44,Zone45,Zone46,Zone47,Zone48,Zone49,Zone50,";
			SQL += "Zone51,Zone52,Zone53,Zone54,Zone55,Zone56,Zone57,Zone58,Zone59,Zone60) values(";
			SQL += IDTube;
			SQL += ",";
			SQL += TubeNum;
			SQL += ",'Поперечный'";
			int NumOfZones = Singleton->CrossResult->zones;
			int i;
			for (i = 0; i < NumOfZones; i++)
			{
				SQL += ",";
				SQL += Singleton->CrossResult->zone_data[i];
			}
			for (; i < 60; i++)
			{
				SQL += ",";
				SQL += 5; // Нет данных (за концом трубы)
			}
			SQL += ")";
			TPr::pr(SQL);
			E = new CExecSQL(SQL);
			ret = E->IsOk();
			delete E;
			if (ret != "Ok")
			{
				TPr::pr(AnsiString("SaveTubeToDB: ") + ret);
			}

			// Продольник
			NumOfZones = Singleton->LinearResult->zones;
			if (NumOfZones > 0)
			{
				SQL = "insert into dbo.ZonesStat (TubeID,TubeNum,Module,";
				SQL += "Zone1,Zone2,Zone3,Zone4,Zone5,Zone6,Zone7,Zone8,Zone9,Zone10,";
				SQL += "Zone11,Zone12,Zone13,Zone14,Zone15,Zone16,Zone17,Zone18,Zone19,Zone20,";
				SQL += "Zone21,Zone22,Zone23,Zone24,Zone25,Zone26,Zone27,Zone28,Zone29,Zone30,";
				SQL += "Zone31,Zone32,Zone33,Zone34,Zone35,Zone36,Zone37,Zone38,Zone39,Zone40,";
				SQL += "Zone41,Zone42,Zone43,Zone44,Zone45,Zone46,Zone47,Zone48,Zone49,Zone50,";
				SQL += "Zone51,Zone52,Zone53,Zone54,Zone55,Zone56,Zone57,Zone58,Zone59,Zone60) values(";
				SQL += IDTube;
				SQL += ",";
				SQL += TubeNum;
				SQL += ",'Продольный'";
				for (i = 0; i < NumOfZones; i++)
				{
					SQL += ",";
					SQL += Singleton->LinearResult->zone_data[i];
				}
				for (; i < 60; i++)
				{
					SQL += ",";
					SQL += 5; // Нет данных (за концом трубы)
				}
				SQL += ")";
				TPr::pr(SQL);
				E = new CExecSQL(SQL);
				ret = E->IsOk();
				delete E;
				if (ret != "Ok")
				{
					TPr::pr(AnsiString("SaveTubeToDB: ") + ret);
				}
			}
			// Толщиномер
			NumOfZones = Singleton->ThResult->zones;
			if (NumOfZones > 0)
			{
				SQL = "insert into dbo.ZonesStat (TubeID,TubeNum,Module,";
				SQL += "Zone1,Zone2,Zone3,Zone4,Zone5,Zone6,Zone7,Zone8,Zone9,Zone10,";
				SQL += "Zone11,Zone12,Zone13,Zone14,Zone15,Zone16,Zone17,Zone18,Zone19,Zone20,";
				SQL += "Zone21,Zone22,Zone23,Zone24,Zone25,Zone26,Zone27,Zone28,Zone29,Zone30,";
				SQL += "Zone31,Zone32,Zone33,Zone34,Zone35,Zone36,Zone37,Zone38,Zone39,Zone40,";
				SQL += "Zone41,Zone42,Zone43,Zone44,Zone45,Zone46,Zone47,Zone48,Zone49,Zone50,";
				SQL += "Zone51,Zone52,Zone53,Zone54,Zone55,Zone56,Zone57,Zone58,Zone59,Zone60) values(";
				SQL += IDTube;
				SQL += ",";
				SQL += TubeNum;
				SQL += ",'Толщиномер'";
				for (i = 0; i < NumOfZones; i++)
				{
					SQL += ",";
					SQL += Singleton->ThResult->LevelClass
						(Singleton->ThResult->zone_data[i]);
				}
				for (; i < 60; i++)
				{
					SQL += ",";
					SQL += 5; // Нет данных (за концом трубы)
				}
				SQL += ")";
				TPr::pr(SQL);
				E = new CExecSQL(SQL);
				ret = E->IsOk();
				delete E;
				if (ret != "Ok")
				{
					TPr::pr(AnsiString("SaveTubeToDB: ") + ret);
				}
			}
		}
	}

}
// ----------------------------------------------------------------------------
void SaveTubeToFile(TIniFile* _ini)
{
	TIniFile* ini = _ini;
	AnsiString DirPath = ini->ReadString("OtherSettings", "TubePath",
		"d:\RAG\Defectoscope\Tubes");
	//Проверяем число сохраненных труб и удаляем наиболее старую, если их больше максимума
	CheckNumberOfFilesInDirectory(ini, DirPath);

	//Формируем название для новой трубы
	AnsiString FileName = DirPath + "\\Tube_";
	time_t t;
	time(&t);
	tm ltm = *localtime(&t);
	AnsiString fn;
	FileName.cat_printf("%.2d%.2d%.2d_%.2d%.2d%.2d.dkb",
		ltm.tm_year - 100, ltm.tm_mon + 1, ltm.tm_mday, ltm.tm_hour,
		ltm.tm_min, ltm.tm_sec);
	TPr::pr(FileName);
	// Создаем пустой файл, чтобы записать в него собранные данные
	try
	{
		FILE* file = fopen(FileName.c_str(), "wb");
		if (file == NULL)
		{
			AnsiString a = "Save: не могу открыть файл для записи: ";
			a += FileName;
			FATAL(a);
		}
//		FILE *file;
//		file = fopen(FileName.c_str(), "a");
//		if (file == NULL)
//		{
//			AnsiString a =
//				"Save: не могу открыть файл для записи: ";
//			a += FileName;
//			FATAL(a);
//		}
//		fclose(file);
//		// Поперечные данные
//		Singleton->CrossResult->SaveTubeToFile(FileName);
//		// Продольные данные
//		Singleton->LinearResult->SaveTubeToFile(FileName);
//		// Толщинометрия (только zone_data)
//		Singleton->ThResult->SaveTubeToFile(FileName);
//		//Сохраняет пороги и усиления для Альметьевска
//		ADCSettForm->SaveTubeSettingsToFile(FileName);
		// Поперечные данные
		Singleton->CrossResult->SaveTubeToFile(file);
		// Продольные данные
		Singleton->LinearResult->SaveTubeToFile(file);
		// Толщинометрия (только zone_data)
		Singleton->ThResult->SaveTubeToFile(file);
		//Сохраняет пороги и усиления для Альметьевска
		ADCSettForm->SaveTubeSettingsToFile(file);
		fclose(file);
	}
	catch (...)
	{
		Application->MessageBoxW(L"Не удалось сохранить файл", L"Ошибка",
			MB_ICONERROR | MB_OK);
	}
}
//-----------------------------------------------------------------------------
void CheckNumberOfFilesInDirectory(TIniFile* _ini, AnsiString _directory)
{
	TIniFile* ini = _ini;
	HANDLE dir;
	WIN32_FIND_DATA file_data;
	wchar_t wdirect[256];
	AnsiString directory = _directory + "/*";
	directory.WideChar(wdirect,256);
	if ((dir = FindFirstFile(wdirect , &file_data))
		== INVALID_HANDLE_VALUE)
		return; /* Файлов не найдено */

	int count = 0; //число файлов в каталоге
	AnsiString oldestTubeName = ""; //имя самого старого файла
	char oldestTubeTime[26]; //время изменения самого старого файла
	char timebuf[26]; //время изменения текущего файла
	struct _stat buf; //данные текущего файла
	int result;       //результат чтения статистики файла
	AnsiString Proto; //текст для протокола

	do {
		AnsiString file_name = file_data.cFileName;
		AnsiString full_file_name = _directory + "/" + file_name;
		const bool is_directory =
			(file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[1] == '.')
			continue;

		if (is_directory)
			continue;

		count++;
		result = _stat( full_file_name.c_str(), &buf );
		if( result != 0 )
		{
			Proto = "Не можем прочесть данные по файлу с трубой: "
				+ full_file_name;
			TPr::pr(Proto);
			continue;
		}
		else
		{
			ctime_s(timebuf, 26, &buf.st_mtime);
			if(oldestTubeName == "") //Назначаем первую попавшуюся трубу самой старой
			{
				for(int i=0; i<26; i++)
					oldestTubeTime[i] = timebuf[i];
				oldestTubeName = full_file_name;
			}
			else
			{
				if(oldestTubeTime < timebuf) //обновляем при нахождении более старой
				{
					for(int i=0; i<26; i++)
						oldestTubeTime[i] = timebuf[i];
					oldestTubeName = full_file_name;
				}
			}
		}
	} while (FindNextFile(dir, &file_data));
	FindClose(dir);
	//проверяем итоговое число труб
	if (!ini->ValueExists("OtherSettings", "MaxCountSaveTubes"))
		ini->WriteInteger("OtherSettings", "MaxCountSaveTubes", 1000);
	int maxCount = ini->ReadInteger("OtherSettings","MaxCountSaveTubes", 1000);
	if (count >= maxCount)//если максимум превышен - удаляем самую старую
	{
		if(remove(oldestTubeName.c_str()))
		{
			Proto = "Не можем удалить файл: " + oldestTubeName;
			TPr::pr(Proto);
			return;
		}
	}
}
// --------------------------------------------------------------------------
int CheckCutReason(int _zone, int _module)
{
	// Определим дефект реза (можно использовать и просто
	// для определения дефектов в любой зоне)
	int zone = _zone - 1;
	if((Singleton->SumResult->zones < zone) || (zone < 0)) //ошибка
	{
		TPr::pr("Ошибка: Рез за пределами трубы");
		return 0;
	}
	switch(_module)
	{
		case 1:{
			if(Singleton->ThResult->zones > 0)
			{
				if(Singleton->ThResult->LevelClass(Singleton->ThResult->zone_data[zone]) == 0) return 1;
					else return 0;
			}
			else return 0;
		}
		case 2:{
			if(Singleton->CrossResult->zones > 0)
			{
				if(Singleton->CrossResult->zone_data[zone] == 0) return 1;
					else return 0;
			}
			else return 0;
		}
		case 3:{
			if(Singleton->LinearResult->zones > 0)
			{
				if(Singleton->LinearResult->zone_data[zone] == 0) return 1;
					else return 0;
			}
			else return 0;
		}
		default:{
			TPr::pr("Ошибка: Не определен модуль для проверки реза");
			return 0;
		}
    }
}
// --------------------------------------------------------------------------
int CheckModBrack(int _module)
{
	switch(_module)
	{
		case 1:{
			if(Singleton->ThResult->zones > 0)
			{
				for(int i = 0;i < Singleton->ThResult->zones; ++i)
				{
					if(Singleton->ThResult->LevelClass(Singleton->ThResult->zone_data[i]) == 0)
						return 1;//брак - достаточно
				}
				return 0;
			}
			else return 0;
		}
		case 2:{
			if(Singleton->CrossResult->zones > 0)
			{
				for(int i = 0;i < Singleton->CrossResult->zones; ++i)
				{
					if(Singleton->CrossResult->zone_data[i] == 0)
						return 1;//брак - достаточно
				}
				return 0;
			}
			else return 0;
		}
		case 3:{
			if(Singleton->LinearResult->zones > 0)
			{
				for(int i = 0;i < Singleton->LinearResult->zones; ++i)
				{
					if(Singleton->LinearResult->zone_data[i] == 0)
						return 1;//брак - достаточно
				}
				return 0;
			}
			else return 0;
		}
		default:{
			TPr::pr("Ошибка: Не определен модуль для проверки наличия дефектов");
			return 0;
		}
	}
}
// --------------------------------------------------------------------------
