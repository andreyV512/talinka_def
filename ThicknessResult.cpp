#pragma hdrstop
#include "ThicknessResult.h"
#include "Global.h"
#include <Series.hpp>
#include "Protocol.h"
#include "uFunctions.h"
#pragma package(smart_init)
ThicknessResult::ThicknessResult(int _sensors,TIniFile* _ini) : ResultBase(_sensors,_ini)
{
	zones=0;
	borders.set_length(2);
	LoadSettings0();
	FRM=NULL;
}

// ----------------------------------------------------------------------------
ThicknessResult::~ThicknessResult()
{
	ThicknessResult::zone_data.~DynamicArray();
}
// ----------------------------------------------------------------------------
TColor ThicknessResult::ZoneColor(double meas)
{
	// возвращает цвет зоны в зависимости от толщины в этой зоне
	if(meas==10.0)
		return Dead;
	else if(meas>ThicknessResult::borders[borders.get_high()])
		return Good;
	else if(meas<=ThicknessResult::borders[0])
		return Brack;
	else
		return Class2;
}
// ----------------------------------------------------------------------------
bool ThicknessResult::IsDefect(double _meas)
{
	if(_meas==10.0)
		return(false);
	else if(_meas>ThicknessResult::borders[borders.get_high()])
		return(false);
	else if(_meas<=ThicknessResult::borders[0])
		return(true);
	else
		return(true);
}

// ----------------------------------------------------------------------------
void ThicknessResult::PutResultOnChart(void)
{
	if(FRM==NULL)
		return;
	FRM->Clear();
	int defects=0;
	for(int j=0;j<Globals::max_zones;j++)
	{
		if(j<zones)
		{
//			AnsiString a;
//			a="Thick: [";
//			a+=j;
//			a+="]=";
//			a+=zone_data[j];
//			TPr::pr(a);
			FRM->Chart1->Series[0]->AddXY(j,zone_data[j],"",
			ZoneColor(zone_data[j]));
			if(IsDefect(zone_data[j]))
				defects++;
		}
		else
			FRM->Chart1->Series[0]->Add(0,"",clWhite);
	}
	FRM->SetDefects(defects);
}

// ------------------------------------------------------------------------------
//void ThicknessResult::SaveTubeToFile(UnicodeString FileName)
void ThicknessResult::SaveTubeToFile(FILE *file)
{
//	FILE *file;
//	file=fopen(AnsiString(FileName).c_str(),"a");
//	if(file==NULL)
//		Application->MessageBoxW(L"Не удалось открыть файл для записи",
//		L"Ошибка",MB_ICONERROR|MB_OK);

	fprintf(file,"\n");
	fprintf(file,"%d ",zones);
	for(int i=0;i<zones;i++)
		fprintf(file,"%f ",zone_data[i]);
//	fclose(file);
}

// ------------------------------------------------------------------------------
void ThicknessResult::LoadTubeFromFile(FILE *file)
{
	fscanf(file,"%d ", &zones);
	for(int i=0;i<zones;i++)
	{
		double ppp;
		// fscanf(file,"%lf", &zone_data[i]);
		fscanf(file,"%lf", &ppp);
		zone_data[i]=ppp;
	}
}
// ------------------------------------------------------------------------------
double ThicknessResult::LevelClass(double _level)
{
	if(_level==10.0)
		return -1;
	else if(_level>ThicknessResult::borders[borders.get_high()])
		return 2;
	else if(_level<=ThicknessResult::borders[0])
		return 0;
	else
		return 1;

}
void ThicknessResult::LoadSettings0(void)
{
	AnsiString sect="Type_"+ini->ReadString("Default","TypeSize","1");

	double bor;

	borders.set_length(1);
	borders[0]=ini->ReadFloat(sect,"ThicknessBorder1",0.0);
	bor=ini->ReadFloat(sect,"ThicknessBorder2",0.0);
	if(bor!=0)
	{
		borders.set_length(2);
		borders[1]=bor;
	}
}
// ---------------------------------------------------------------------------
void ThicknessResult::LoadSettings(void)
{
	LoadSettings0();
}
// ---------------------------------------------------------------------------
void ThicknessResult::Bind(TFRThick* _FRM)
{
	FRM = _FRM;
}
// ---------------------------------------------------------------------------
double ThicknessResult::MinThick(int _cut0,int _cut1)
{
	int start=0;
	int stop=zones;
	if(_cut0>0)
		start=_cut0;
	if(_cut1>0)
		stop=_cut1-1;
	double ret=0;
	bool first=true;
	for(int i=start;i<stop;i++)
	{
		if(first)
		{
			ret=zone_data[i];
			first=false;
		}
		else
		{
			if(ret>zone_data[i])
				ret=zone_data[i];
		}
	}
	return(ret);
}
// ---------------------------------------------------------------------------

