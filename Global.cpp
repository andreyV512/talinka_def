#pragma hdrstop
#include "Global.h"
#include <IniFiles.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
void SystemConst::SetSystemConstants()
{

	// читаем параметры из файла
	TIniFile *ini=new TIniFile(Globals::IniFileName);

	isLinearDigitalFilterOut=ini->ReadBool("Filters","isFilterPrOut",1);
	isLinearDigitalFilterIn=ini->ReadBool("Filters","isFilterPrIn",1);
	isCrossDigitalFilter=ini->ReadBool("Filters","isFilterPp",1);
	delete ini;
}
// ---------------------------------------------------------------------------
