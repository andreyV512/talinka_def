// ---------------------------------------------------------------------------
#pragma hdrstop
#include "uFunctions.h"
#include <stdio.h>
#include <SysUtils.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)

void LoadFormPos(TForm* _form, TIniFile* _ini)
{
	AnsiString ident="win_";
	ident+=_form->Name;
	AnsiString s=_ini->ReadString("Wins",ident,"");
	if(s.Length()<7)
		return;
	int left;
	int width;
	int top;
	int height;
	if(sscanf(s.c_str(),"%d,%d,%d,%d",&left,&width,&top,&height)!=4)
		return;
	_form->Left=left;
	_form->Width=width;
	_form->Top=top;
	_form->Height=height;
}
void SaveFormPos(TForm* _form, TIniFile* _ini)
{
	AnsiString ident="win_";
	ident+=_form->Name;
	AnsiString s= _form->Left;
	s+=",";
	s+=_form->Width;
	s+=",";
	s+=_form->Top;
	s+=",";
	s+=_form->Height;
	_ini->WriteString("Wins",ident,s);
}
void FATAL(AnsiString _msg)
{
	WideString w=_msg;
	Application->MessageBoxW(w.c_bstr(), L"Аварийное завершение",MB_ICONERROR | MB_OK);
	throw Exception(L"Аварийное завершение");
}
void Fread(void* _buf, int _size, int _n, FILE* _df)
{
	if (fread(_buf, _size, _n, _df) != _n)
	{
		if (feof(_df))
		{
			fpos_t pos;
			fgetpos(_df, &pos);
			AnsiString a = "Fread: Неожиданный конец файла: ";
			a += pos;
			throw(Exception(a));
		}
		else
			throw(Exception("Fread: Неизвестная ошибка"));
	}
}
