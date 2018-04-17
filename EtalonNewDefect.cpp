// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonNewDefect.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
//#include "EtalonBorderSet.h"
#include "Singleton.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonNewDefectForm *EtalonNewDefectForm;

// ---------------------------------------------------------------------------
__fastcall TEtalonNewDefectForm::TEtalonNewDefectForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormKeyPress(TObject *Sender,
	wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormActivate(TObject *Sender)
{
	// Подключимся к БД
	QEtalons->Connection = DBS->Connection();
}

// ---------------------------------------------------------------------------
void TEtalonNewDefectForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
	// Сбросим переменные
	PointEdit = false;
	PointNum = 0;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// Сбросим переменные
	PointEdit = false;
	PointNum = 0;
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::bSaveClick(TObject *Sender)
{
	//Проверим заполнены ли все поля
	if (eIDTube->Text != "" && eName->Text != "" && eTypeSize->Text != "" &&
		eDefPoint->Text != "" && eDefRadius->Text != "" &&
		eTopPercentage->Text != "" && eBottomPercentage->Text != "")
	{
		// Формируем запрос для SQL
		AnsiString Str = "";
		AnsiString SQL;
		if (PointEdit == false) //Новый дефект для СОП
		{
			SQL = "insert into dbo.EtalonTubeDefects (IDTube,Name,TypeSize,Module,DefType,";
			SQL += "DefPoint,DefRadius,isTopBrackBorder,TopBorderPercent,";
			SQL += "isBottomBrackBorder,BottomBorderPercent) values ('";
			SQL += eIDTube->Text;
			SQL += "','";
			SQL += eName->Text;
			SQL += "','";
			SQL += eTypeSize->Text;
			SQL += "','";
			SQL += IntToStr((cbModule->ItemIndex + 1));
			SQL += "','";
			SQL += IntToStr((cbDefType->ItemIndex + 1));
			SQL += "','";
			SQL += eDefPoint->Text;
			SQL += "','";
			SQL += eDefRadius->Text;
			SQL += "','";
			SQL += IntToStr(cbTopBorder->ItemIndex);
			SQL += "','";
			SQL += eTopPercentage->Text;
			SQL += "','";
			SQL += IntToStr(cbBottomBorder->ItemIndex);
			SQL += "','";
			SQL += eBottomPercentage->Text;
			SQL += "')";
		}
		else // Редактирование старого дефекта СОП
		{
			SQL = "UPDATE dbo.EtalonTubeDefects ";
			SQL += "SET IDTube = '";
			SQL += eIDTube->Text;
			SQL += "', Name = '";
			SQL += eName->Text;
			SQL += "', TypeSize = '";
			SQL += eTypeSize->Text;
			SQL += "', Module = '";
			SQL += IntToStr((cbModule->ItemIndex + 1));
			SQL += "', DefType = '";
			SQL += IntToStr((cbDefType->ItemIndex + 1));
			SQL += "', DefPoint = '";
			SQL += eDefPoint->Text;
			SQL += "', DefRadius = '";
			SQL += eDefRadius->Text;
			SQL += "', isTopBrackBorder = '";
			SQL += IntToStr(cbTopBorder->ItemIndex);
			SQL += "', TopBorderPercent = '";
			SQL += eTopPercentage->Text;
			SQL += "', isBottomBrackBorder = '";
			SQL += IntToStr(cbBottomBorder->ItemIndex);
			SQL += "', BottomBorderPercent = '";
			SQL += eBottomPercentage->Text;
			SQL += "' WHERE ID = ";
			SQL += IntToStr(PointNum);
		}
		// Выполняем запрос
		CExecSQL E(SQL);
		if (E.IsOk() != "Ok")
		{
			Str = "Не смогли сохранить данные эталона!";
			TPr::pr(Str);
			Application->MessageBoxW(L"Не смогли сохранить данные эталона!",
				L"Предупреждение!", MB_ICONWARNING);
		}
		else
		{
			Str = "Сохранили данные эталона";
			TPr::pr(Str);
			// Очищаем поля
			eIDTube->Text = "";
			eName->Text = "";
			eTypeSize->Text = "";
			eDefPoint->Text = "";
			eDefRadius->Text = "";
			eTopPercentage->Text = "0";
			eBottomPercentage->Text = "0";
			cbModule->ItemIndex = 0;
			cbModuleChange(this);
			cbDefType->ItemIndex = 0;
			cbTopBorder->ItemIndex = 1;
			cbBottomBorder->ItemIndex = 1;
			Close();
		}
	}
	else
		Application->MessageBoxW(L"Заполните все поля!", L"Предупреждение!",
		MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::bAbortClick(TObject *Sender)
{
	// Закроем окно
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonNewDefectForm::cbModuleChange(TObject *Sender)
{
	if (cbModule->ItemIndex == 0) // Поперечный
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("До 3 соседних");
		cbDefType->Items->Add("Все датчики");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
	}
	else if (cbModule->ItemIndex == 1) // Продольный
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("Все датчики");
		cbDefType->Items->Add("1 и более");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
	}
	else if (cbModule->ItemIndex == 2) // Толщиномер
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("Толщина");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = false;
		cbBottomBorder->Visible = false;
		lTopPercent->Caption = "мм";
		lBottomPercent->Caption = "мм";
	}
	else
		cbDefType->Items->Clear();
}

// ---------------------------------------------------------------------------
void TEtalonNewDefectForm::SetModuleType(int _DefModule, int _DefType)
{
    // Выбирает нужный модуль и тип дефекта из списков на форме (при редактировании)
	cbModule->ItemIndex = _DefModule;
	switch (_DefModule)
	{
	case 0:	//Поперечник
		cbDefType->Items->Clear();
		cbDefType->Items->Add("До 3 соседних");
		cbDefType->Items->Add("Все датчики");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
		break;
	case 1:	//Продольник
		cbDefType->Items->Clear();
		cbDefType->Items->Add("Все датчики");
		cbDefType->Items->Add("1 и более");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
		break;
	case 2:	//Толщиномер
		cbDefType->Items->Clear();
		cbDefType->Items->Add("Толщина");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = false;
		cbBottomBorder->Visible = false;
		lTopPercent->Caption = "мм";
		lBottomPercent->Caption = "мм";
		break;
	}
}
void __fastcall TEtalonNewDefectForm::bManualBorderSetClick(TObject *Sender)
{
	//расчет зон
	int zoneLength = Singleton->CrossResult->GetZoneLength();
	int startZone = (eDefPoint->Text.ToInt() - eDefRadius->Text.ToInt())/zoneLength;
//	if((eDefPoint->Text.ToInt() - eDefRadius->Text.ToInt())%zoneLength == 0)
//		startZone++;
	int lastZone = (eDefPoint->Text.ToInt() + eDefRadius->Text.ToInt())/zoneLength;
	if((eDefPoint->Text.ToInt() + eDefRadius->Text.ToInt())%zoneLength == 0)
		lastZone--;
	if (cbModule->ItemIndex == 0) // Поперечный
	{
		Singleton->CrossResult->EtalonBorderSet(startZone, lastZone, this, ini);
		//todo сохранить изменения
	}
	else if (cbModule->ItemIndex == 1) // Продольный
	{
		Singleton->LinearResult->EtalonBorderSet(startZone, lastZone, this, ini);
		//todo сохранить изменения
	}
	else if (cbModule->ItemIndex == 2) // Толщиномер
	{
		Application->MessageBoxW(L"Нельзя настроить в ручную!"
			,L"Предупреждение!", MB_ICONWARNING);
	}
	else
		cbDefType->Items->Clear();
}
//---------------------------------------------------------------------------

