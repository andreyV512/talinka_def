#ifndef ThreadOnLineH
#define ThreadOnLineH
#include "CBank.h"
#include <IniFiles.hpp>

class ThreadOnLine : public TThread
{
private:
	int pauseWorkSpeed;
	int pauseStop;

	void __fastcall Execute();

	// ! Используем ли поперечный (всегда используем)
	bool Cross;
	// ! Используем ли продольный
	bool Linear;

	// ! текст для надписи в статусбаре в панели 1
	AnsiString stext1;
	// ! Текст для надписи в статусбаре в панели 2
	AnsiString stext2;

	// ! Выставляем исходное положение модулей перед работой
	UnicodeString PrepareForWork();
	// ! Финальные штуки (отключение сигналов и т.д.)
	void Finally();
	void ErrFinally(AnsiString _msg,bool* _Collect,bool* _result);

	// Скорости вращения вход, работа, выход
	// vector<int> speeds;
	int InSpeed;
	int WorkSpeed;
	int OutSpeed;

	// ! Поток обработки всей кучи информации
	bool OnlineCycle(void);

	int delay;
	CBank* BankCross;
	CBank* BankLine;
	UINT thread_msg;
	HWND main_handle;

	void Post(WPARAM _w,LPARAM _l=0);

	bool Collect;

	TCriticalSection* cs;

	void SetStext1(AnsiString _s);
	void SetStext2(AnsiString _s);

	TEvent* calc_event;

	TIniFile* ini;

	void pr(AnsiString _msg);
	vector <double>* SGbuffer;
	bool IsBrak;

public:
	__fastcall ThreadOnLine(bool CreateSuspended,bool _Linear,CBank*_BankCross,
		CBank*_BankLine,HWND _main_handle,UINT _thread_msg,TIniFile*_ini,vector <double> *_SGbuffer);
	__fastcall ~ThreadOnLine(void);
	static const WPARAM REDRAW=0;
	static const LPARAM REDRAW_CROSS=0;
	static const LPARAM REDRAW_LINE=1;
	static const WPARAM COMPUTE=1;
	static const WPARAM UPDATE_STATUS=3;
	static const WPARAM SGDRAW=4;
	static const WPARAM COMPLETE=5;
	static const WPARAM NEXT_TUBE=6;

	AnsiString GetStext1(void);
	AnsiString GetStext2(void);
	void SetCalc(bool _IsBrak);
};
// ---------------------------------------------------------------------------

#endif
