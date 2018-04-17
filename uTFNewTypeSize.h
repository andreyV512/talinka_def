//---------------------------------------------------------------------------

#ifndef uTFNewTypeSizeH
#define uTFNewTypeSizeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "IniFiles.hpp"
//---------------------------------------------------------------------------
class TFNewTypeSize : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TButton *Button1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	TIniFile *ini;					// ini файл с настройками типоразмеров
public:		// User declarations
	__fastcall TFNewTypeSize(TComponent* Owner,TIniFile* _ini);
	AnsiString typeSize;
};
//---------------------------------------------------------------------------
extern PACKAGE TFNewTypeSize *FNewTypeSize;
//---------------------------------------------------------------------------
#endif
