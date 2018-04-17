// ---------------------------------------------------------------------------
#ifndef uFunctionsH
#define uFunctionsH
#include <Forms.hpp>
#include <IniFiles.hpp>
#include <stdio.h>
// ---------------------------------------------------------------------------
void LoadFormPos(TForm* _form, TIniFile* _ini);
void SaveFormPos(TForm* _form, TIniFile* _ini);
void FATAL(AnsiString _msg);
void Fread(void* _buf, int _size, int _n, FILE* _df);
#endif
