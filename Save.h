//---------------------------------------------------------------------------

#ifndef SaveH
#define SaveH
#include <IniFiles.hpp>
//---------------------------------------------------------------------------
//Запись данных по трубе в БД для статистики
void SaveTubeToDB(TIniFile* _ini);
//Запись данных по трубе в файл
void SaveTubeToFile(TIniFile* _ini);
//Проверяем число сохраненных труб в папке и удаляем старейшую при превышении максимума
void CheckNumberOfFilesInDirectory(TIniFile* _ini, AnsiString _directory);
//Проверяет наличие брака по модулю в конкретной зоне
int CheckCutReason(int _zone, int _module);
//Проверяет обнаружен ли брак по модулю
int CheckModBrack(int _module);
#endif
