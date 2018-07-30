#pragma hdrstop
#include "Series.hpp"
#include "Filters.h"
#include <IniFiles.hpp>
#include "Protocol.h"
#include "uFunctions.h"
#pragma package(smart_init)
// ------------------------------------------------------------------------------
Filters *LinearFilterIn;
Filters *LinearFilterOut;
Filters *CrossFilter;

// ------------------------------------------------------------------------------
Filters::Settings::Settings(AnsiString _type)
{
	type = _type;
}

// ------------------------------------------------------------------------------
void Filters::Settings::Load(TIniFile* _ini)
{
	isFilter = _ini->ReadBool("Filters", AnsiString("isFilter") + type, 0);
	CurrentType = (filterTypes)_ini->ReadInteger("Filters",
		AnsiString("FilterType") + type, 0);
	CurrentSubType = (filterSubTypes)_ini->ReadInteger("Filters",
		AnsiString("FilterSubType") + type, 0);
	order = _ini->ReadInteger("Filters", AnsiString("order") + type, 2);
	cutoffFrequency = _ini->ReadFloat("Filters", AnsiString("cutoffFrequency") +
		type, 300.0);
	widthFrequency = _ini->ReadFloat("Filters", AnsiString("widthFrequency") +
		type, 300.0);
	centerFrequency = _ini->ReadFloat("Filters", AnsiString("centerFrequency") +
		type, 300.0);
	rippleDb = _ini->ReadFloat("Filters", AnsiString("rippleDb") + type, 0.3);
	rolloff = _ini->ReadFloat("Filters", AnsiString("rolloff") + type, 0.3);
	sampleRate = _ini->ReadFloat("LCard", AnsiString("Rate"), 1);
}

// ------------------------------------------------------------------------------
void Filters::Settings::Save(TIniFile* _ini)
{
	_ini->WriteBool("Filters", AnsiString("isFilter") + type, isFilter);
	_ini->WriteInteger("Filters", AnsiString("FilterType") + type, CurrentType);
	_ini->WriteInteger("Filters", AnsiString("FilterSubType") + type,
		CurrentSubType);
	_ini->WriteInteger("Filters", AnsiString("order") + type, order);
	_ini->WriteFloat("Filters", AnsiString("cutoffFrequency") + type,
		cutoffFrequency);
	_ini->WriteFloat("Filters", AnsiString("widthFrequency") + type,
		widthFrequency);
	_ini->WriteFloat("Filters", AnsiString("centerFrequency") + type,
		centerFrequency);
	_ini->WriteFloat("Filters", AnsiString("rippleDb") + type, rippleDb);
	_ini->WriteFloat("Filters", AnsiString("rolloff") + type, rolloff);
}
// ------------------------------------------------------------------------------
void Filters::Settings::Save(FILE* _df)
{
//	fwrite(&isFilter,sizeof(isFilter),1,_df);
//	fwrite(&CurrentType,sizeof(CurrentType),1,_df);
//	fwrite(&CurrentSubType,sizeof(CurrentSubType),1,_df);
//	fwrite(&order,sizeof(order),1,_df);
//	fwrite(&cutoffFrequency,sizeof(cutoffFrequency),1,_df);
//	fwrite(&widthFrequency,sizeof(widthFrequency),1,_df);
//	fwrite(&centerFrequency,sizeof(centerFrequency),1,_df);
//	fwrite(&rippleDb,sizeof(rippleDb),1,_df);
//	fwrite(&rolloff,sizeof(rolloff),1,_df);
//	fwrite(&sampleRate,sizeof(sampleRate),1,_df);
	fprintf(_df,"%d ",isFilter);
	fprintf(_df,"%d ",CurrentType);
	fprintf(_df,"%d ",CurrentSubType);
	fprintf(_df,"%d ",order);
	fprintf(_df,"%lf ",cutoffFrequency);
	fprintf(_df,"%lf ",widthFrequency);
	fprintf(_df,"%lf ",centerFrequency);
	fprintf(_df,"%lf ",rippleDb);
	fprintf(_df,"%lf ",rolloff);
	fprintf(_df,"%lf ",sampleRate);
}
// ------------------------------------------------------------------------------
void Filters::Settings::Load(FILE* _df)
{
//	Fread(&isFilter,sizeof(isFilter),1,_df);
//	Fread(&CurrentType,sizeof(CurrentType),1,_df);
//	Fread(&CurrentSubType,sizeof(CurrentSubType),1,_df);
//	Fread(&order,sizeof(order),1,_df);
//	Fread(&cutoffFrequency,sizeof(cutoffFrequency),1,_df);
//	Fread(&widthFrequency,sizeof(widthFrequency),1,_df);
//	Fread(&centerFrequency,sizeof(centerFrequency),1,_df);
//	Fread(&rippleDb,sizeof(rippleDb),1,_df);
//	Fread(&rolloff,sizeof(rolloff),1,_df);
//	Fread(&sampleRate,sizeof(sampleRate),1,_df);
	double doubPar;
	fscanf(_df,"%d ", &isFilter);
	fscanf(_df,"%d ", &CurrentType);
	fscanf(_df,"%d ", &CurrentSubType);
	fscanf(_df,"%d ", &order);
	fscanf(_df,"%lf", &doubPar);
	cutoffFrequency = doubPar;
	fscanf(_df,"%lf", &doubPar);
	widthFrequency = doubPar;
	fscanf(_df,"%lf", &doubPar);
	centerFrequency = doubPar;
	fscanf(_df,"%lf", &doubPar);
	rippleDb = doubPar;
	fscanf(_df,"%lf", &doubPar);
	rolloff = doubPar;
	fscanf(_df,"%lf", &doubPar);
	sampleRate = doubPar;

}
// ------------------------------------------------------------------------------
//HINSTANCE Filters::dllInstance = NULL;
// ------------------------------------------------------------------------------
Filters::Filters(TIniFile* _ini, AnsiString _type) : settings(_type)
{
	ini = _ini;
	System::UnicodeString path = ExtractFilePath(Application->ExeName) + "../../Settings/filters.dll";
   //	if(!dllInstance)
	dllInstance = LoadLibrary(path.c_str());

	if (!dllInstance)
	{
		String strError = L"Не удалось загрузить библиотеку фильтров";
		MessageBox(NULL, strError.w_str(), L"Ошибка", MB_ICONERROR | MB_OK);
	}
	else
	{
		filter_chebyshev = (_ChebyshevI) GetProcAddress(dllInstance,
			"ChebyshevI");
		filter_butterworth = (_Butterworth) GetProcAddress(dllInstance,
			"Butterworth");
		filter_elliptic = (_Elliptic) GetProcAddress(dllInstance, "Elliptic");
		LoadSettings();
	}
}
// ------------------------------------------------------------------------------
void Filters::toFilter(double *data, int N)
{
	if(!settings.isFilter)
		return;
	switch (settings.CurrentType)
	{
	case 0:
		{
			filter_butterworth(data, N, settings.order, settings.sampleRate,
				settings.cutoffFrequency, settings.centerFrequency,
				settings.widthFrequency, settings.CurrentSubType);
			break;
		}
	case 1:
		{
			filter_chebyshev(data, N, settings.order, settings.sampleRate,
				settings.cutoffFrequency, settings.centerFrequency,
				settings.widthFrequency, settings.rippleDb,
				settings.CurrentSubType);
			break;
		}
	case 2:
		{
			filter_elliptic(data, N, settings.order, settings.sampleRate,
				settings.cutoffFrequency, settings.centerFrequency,
				settings.widthFrequency, settings.rippleDb, settings.rolloff,
				settings.CurrentSubType);
			break;
		}
	}
}
// ------------------------------------------------------------------------------
void Filters::LoadSettings()
{
	settings.Load(ini);
}
// ------------------------------------------------------------------------------
void Filters::Save(FILE* _df)
{
	settings.Save(_df);
}
// ------------------------------------------------------------------------------

