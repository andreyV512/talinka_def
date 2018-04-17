// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "EtalonDefectsCheck.h"
#include "Singleton.h"
#include "uFunctions.h"
#include "uCDBS.h"
#include "protocol.h"
#include "uCExecSQL.h"
#include "uCSelect.h"
#include "Global.h"
#include <algorithm>
#include "Password.h"
#include <time.h>
#include "ADCSettings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEtalonDefectsCheckForm *EtalonDefectsCheckForm;

// ---------------------------------------------------------------------------
__fastcall TEtalonDefectsCheckForm::TEtalonDefectsCheckForm(TComponent* Owner)
	: TForm(Owner)
{
	KeyPreview = true;
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::FormKeyPress(TObject *Sender,
	wchar_t &Key)
{
	// Закрытие окна по нажатию Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::FormShow(TObject *Sender)
{
	// Восстановим положение окна на экране и его размеры
	LoadFormPos(this, ini);
	AnsiString Str = "Сверимся с эталоном " + IntToStr(IDTube);
	TPr::pr(Str);
	// Подключимся к БД
	QEtalons->Connection = DBS->Connection();
	// Очистка списка
	mCheckedDefects->Clear();
	mCheckedDefects->Lines->Clear();
	EtalonCheck();
	// Закроем кнопку "сохранить" в случае загруженной трубы
	bSave->Enabled = !Singleton->FromFile;
}

// ---------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetIni(TIniFile* _ini)
{
	// Укажем файл с настройками
	ini = _ini;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// Сохраним положение окна на экране и его размеры
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetIDTube(int _IDTube)
{
	// Укажем ID СОПа
	IDTube = _IDTube;
}

// ----------------------------------------------------------------------------
void TEtalonDefectsCheckForm::EtalonCheck()
{
	// if(Singleton->CrossResult->zones == 0)
	// {
	// TPr::pr("Нет трубы для сверки");
	// Application->MessageBoxW(L"Нет трубы для сверки!",
	// L"Предупреждение!", MB_ICONWARNING);
	// Close();
	// return;
	// }
	// --Список активных дефектоскопистов
	cbSelectDefectoscoper->Items->Clear();
	QEtalons->Close();
	QEtalons->Active = false;
	QEtalons->SQL->Clear();
	AnsiString SQL;
	SQL = "SELECT * FROM Defectoscopers ";
	SQL += " where Defectoscopers.Active = 1";
	SQL += " order by Defectoscopers.ID";
	QEtalons->SQL->Add(SQL);
	QEtalons->ExecSQL();
	QEtalons->Active = true;
	QEtalons->First();
	while (!QEtalons->Eof)
	{
		cbSelectDefectoscoper->Items->Add
			(QEtalons->FieldByName("Name")->AsString);
		QEtalons->Next();
	}
	cbSelectDefectoscoper->ItemIndex = 0;
    // выставляем последнего авторизовавшегося
	AnsiString LastDef = ini->ReadString("Default","LastAuthorisation","");
	if (LastDef != "")
		cbSelectDefectoscoper->ItemIndex = cbSelectDefectoscoper->Items->IndexOf(LastDef);
	cbSelectDefectoscoperChange(this);
	// -----
	if (Singleton->CrossResult->zones == 0)
	{
		TPr::pr("Нет трубы для сверки");
		Application->MessageBoxW(L"Нет трубы для сверки!", L"Предупреждение!",
			MB_ICONWARNING);
		// Close();
		return;
	}
	// -----
	QEtalons->Close();
	QEtalons->Active = false;
	QEtalons->SQL->Clear();
	// AnsiString SQL;
	SQL = "SELECT * FROM EtalonTubeDefects ";
	SQL += "where EtalonTubeDefects.IDTube='" + IntToStr(IDTube) + "'";
	SQL += " order by EtalonTubeDefects.id";
	QEtalons->SQL->Add(SQL);
	QEtalons->ExecSQL();
	QEtalons->Active = true;

	SetData();
	AnsiString Str = "";
	numOfDefects = 0;
	matchedDefects = 0;
	QEtalons->First();
	eEtalonName->Text = QEtalons->FieldByName("Name")->AsString;
	eTypeSize->Text = QEtalons->FieldByName("TypeSize")->AsInteger;
	// Идем по списку дефектов эталона
	while (!QEtalons->Eof)
	{
		numOfDefects++;
		// Читаем данные
		int Module = QEtalons->FieldByName("Module")->AsInteger;
		int DefType = QEtalons->FieldByName("DefType")->AsInteger;
		int DefPoint = QEtalons->FieldByName("DefPoint")->AsInteger;
		int DefRadius = QEtalons->FieldByName("DefRadius")->AsInteger;
		bool isTopBrackBorder = QEtalons->FieldByName("isTopBrackBorder")
			->AsBoolean;
		int TopBorderPercent = QEtalons->FieldByName("TopBorderPercent")
			->AsInteger;
		bool isBottomBrackBorder = QEtalons->FieldByName("isBottomBrackBorder")
			->AsBoolean;
		int BottomBorderPercent = QEtalons->FieldByName("BottomBorderPercent")
			->AsInteger;
		// проверяем дефект
		if (DefectCheck(Module, DefType, DefPoint, DefRadius, isTopBrackBorder,
			TopBorderPercent, isBottomBrackBorder, BottomBorderPercent))
		{
			matchedDefects++;
			AddMemoResult(true, Module, DefPoint);
		}
		else
		{
			AddMemoResult(false, Module, DefPoint);
		}

		QEtalons->Next();
	}
	Str = IntToStr(matchedDefects) + " из " + IntToStr(numOfDefects);
	pResultNum->Caption = Str;
	if (matchedDefects == numOfDefects)
	{
		pResultBit->Caption = "Совпадает";
		pResultBit->Color = clGreen;
	}
	else
	{
		pResultBit->Caption = "Ошибка";
		pResultBit->Color = clRed;
	}
	pResultNum->Color = pResultBit->Color;
	eDate->Text = DateTimeToStr((Date() + Time()));
	Str = "Закончили расчет дефектов эталона: " + pResultBit->Caption + " " +
		pResultNum->Caption;
	TPr::pr(Str);

}

// ----------------------------------------------------------------------------
bool TEtalonDefectsCheckForm::DefectCheck(int _Module, int _DefType,
	int _DefPoint, int _DefRadius, bool _isTopBrackBorder,
	int _TopBorderPercent, bool _isBottomBrackBorder, int _BottomBorderPercent)
{
	// Алгоритм сверки:
	// Для каждого Дефекта
	// Для каждого датчика
	// проверяем диапозон поиска
	// Результат пишем в массив
	// Проверяем массив на соответствие типу дефекта
	// Если подходит, то возвращаем True
	// Иначе возвращаем False
	// список границ диапозонов поиска
	int startPoint = _DefPoint - _DefRadius;
	int endPoint = _DefPoint + _DefRadius;

	int sensorMax = 0;
	if (_Module == 1) // Поперечник
			sensorMax = Singleton->CrossResult->sensors_a;
	else if (_Module == 2) // Продольник
			sensorMax = Singleton->LinearResult->sensors_a;
	else if (_Module == 3) // Толщиномер
			sensorMax = 1;
	else
	{
		TPr::pr("Ошибка: Неизвестен модуль дефекта");
		return false;
	}
	// Проверяем, что дефект находится в пределах трубы
	int startZone = startPoint / 200;
	int endZone = endPoint / 200;
	switch (_Module)
	{
	case 1:
		if (Singleton->CrossResult->zones < endZone || startZone < 0)
		{
			TPr::pr("Дефект по поперечному модулю за пределами трубы");
			Application->MessageBoxW(L"Дефект эталона по поперечному модулю за пределами трубы!",
				L"Предупреждение!", MB_ICONWARNING);
			return false;
		}
		break;
	case 2:
		if (Singleton->LinearResult->zones < endZone || startZone < 0)
		{
			TPr::pr("Дефект по продольному модулю за пределами трубы");
			Application->MessageBoxW(L"Дефект эталона по продольному модулю за пределами трубы!",
				L"Предупреждение!", MB_ICONWARNING);
			return false;
		}
		break;
	case 3:
		if (Singleton->ThResult->zones < endZone || startZone < 0)
		{
			TPr::pr("Дефект по толщиномеру за пределами трубы");
			Application->MessageBoxW(L"Дефект эталона по толщиномеру за пределами трубы!",
				L"Предупреждение!", MB_ICONWARNING);
			return false;
		}
		break;
	}

	// Проверяем зону поиска
	vector<int>dirtyFind;
	for (int sens = 0; sens < sensorMax; sens++)
	{
		dirtyFind.push_back(0); // Ниже нижней границы
		if (_TopBorderPercent != 0) // Если указана верхняя граница
		{
			if (CheckDiap(_Module, sens, _isTopBrackBorder, _TopBorderPercent,
				startPoint, endPoint))
			{
				dirtyFind[sens] = 2; // Выше верхней границы
			}
			else
				dirtyFind[sens] = 1; // Ниже верхней границы
		}
		if ((_BottomBorderPercent != 0) && (dirtyFind[sens] != 2))
			// Если указана нижняя граница
		{
			if (CheckDiap(_Module, sens, _isBottomBrackBorder,
				_BottomBorderPercent, startPoint, endPoint))
			{
				dirtyFind[sens] = 1; // Между верхней и нижней границами
			}
			else
				dirtyFind[sens] = 0; // Ниже нижней границы
		}
	}

	AnsiString Str = "DirtyFind = [";
	for (int i = 0; i < (int)dirtyFind.size(); i++)
	{
		Str += IntToStr(dirtyFind[i]);
	}
	Str += "]";
	TPr::pr(Str);

	// Проверяем на соответствие типу дефекта
	bool Result = false;
	Result = CheckDefType(_Module, _DefType, dirtyFind);

	return Result;
}

// ----------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetData()
{
	// Поперечник
	CrossFilteredOut_Data = Singleton->CrossResult->GetFilteredOut_Data();
	CrossGain = Singleton->CrossResult->GetGain();
	CrossDeads = Singleton->CrossResult->GetDeads();
	// Продольник
	LinearFilteredOut_Data = Singleton->LinearResult->GetFilteredOut_Data();
	if (SystemConst::isLinearDigitalFilterIn)
		LinearFilteredIn_Data = Singleton->LinearResult->GetFilteredIn_Data();
	LinearGain = Singleton->LinearResult->GetGain();
	LinearDeads = Singleton->LinearResult->GetDeads();
}

// ----------------------------------------------------------------------------
bool TEtalonDefectsCheckForm::CheckDiap(int _Module, int _sensor,
	bool _isBrackBorder, int _borderPercent, int _startPoint, int _endPoint)
{
	int startZone = _startPoint / 200;
	int endZone = _endPoint / 200;
	double borderCoefficient = 100.0 / _borderPercent;
	if (_Module == 1) // Поперечник
	{
//		if (Singleton->CrossResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("Дефект по поперечному модулю за пределами трубы");
//			Application->MessageBoxW(L"Дефект эталона за пределами трубы!",
//				L"Предупреждение!", MB_ICONWARNING);
//			return false;
//		}
		if (Singleton->CrossResult->sensors_a < _sensor || _sensor < 0)
		{
			TPr::pr("Ошибка: Неправильно указан датчик для сверки с эталоном");
			Application->MessageBoxW
				(L"Ошибка: Неправильно указан датчик для сверки с эталоном",
				L"Предупреждение!", MB_ICONWARNING);
			return false;
		}
		int startMeasure = (int)CrossFilteredOut_Data[startZone][0].size() *
			(_startPoint % 200) / 200;
		int endMeasure = (int)CrossFilteredOut_Data[endZone][0].size() *
			(_endPoint % 200) / 200;
		// Если обе точки в одной зоне
		if (startZone == endZone)
		{
			for (int j = startMeasure; j < endMeasure; j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[startZone][_sensor][j]
						* CrossGain[_sensor] * borderCoefficient,
						false) // CrossDeads[startZone][j])
						== Singleton->CrossResult->Brack)
						return true;
				}
				else
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[startZone][_sensor][j]
						* CrossGain[_sensor] * borderCoefficient,
						false) // CrossDeads[startZone][j])
						!= Singleton->CrossResult->Good)
						return true;
				}
			}
		}
		else // если в разных
		{
			// проверим начальную зону
			for (int j = startMeasure;
			j < (int)CrossFilteredOut_Data[startZone][_sensor].size(); j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[startZone][_sensor][j]
						* CrossGain[_sensor] * borderCoefficient,
						false) // CrossDeads[startZone][j])
						== Singleton->CrossResult->Brack)
						return true;
				}
				else
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[startZone][_sensor][j]
						* CrossGain[_sensor] * borderCoefficient,
						false) // CrossDeads[startZone][j])
						!= Singleton->CrossResult->Good)
						return true;
				}
			}
			// проверим промежуточные зоны
			int zone = startZone + 1;
			while (zone < endZone)
			{
				for (int j = 0;
				j < (int)CrossFilteredOut_Data[zone][_sensor].size(); j++)
				{
					if (_isBrackBorder)
					{
						if (Singleton->CrossResult->ZoneColor
							(CrossFilteredOut_Data[zone][_sensor][j] * CrossGain
							[_sensor] * borderCoefficient,
							false) // CrossDeads[zone][j])
							== Singleton->CrossResult->Brack)
							return true;
					}
					else
					{
						if (Singleton->CrossResult->ZoneColor
							(CrossFilteredOut_Data[zone][_sensor][j] * CrossGain
							[_sensor] * borderCoefficient,
							false) // CrossDeads[zone][j])
							!= Singleton->CrossResult->Good)
							return true;
					}
				}
				zone++;
			}
			// Проверим конечную зону
			for (int j = 0; j < endMeasure; j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[endZone][_sensor][j] * CrossGain
						[_sensor] * borderCoefficient,
						false) // CrossDeads[endZone][j])
						== Singleton->CrossResult->Brack)
						return true;
				}
				else
				{
					if (Singleton->CrossResult->ZoneColor
						(CrossFilteredOut_Data[endZone][_sensor][j] * CrossGain
						[_sensor] * borderCoefficient,
						false) // CrossDeads[endZone][j])
						!= Singleton->CrossResult->Good)
						return true;
				}
			}
		}
	}
	if (_Module == 2) // Продольник
	{
//		if (Singleton->LinearResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("Дефект по продольному модулю за пределами трубы");
//			Application->MessageBoxW(L"Дефект эталона за пределами трубы!",
//				L"Предупреждение!", MB_ICONWARNING);
//			return false;
//		}
		if (Singleton->LinearResult->sensors_a < _sensor || _sensor < 0)
		{
			TPr::pr("Ошибка: Неправильно указан датчик для сверки с эталоном");
			Application->MessageBoxW
				(L"Ошибка: Неправильно указан датчик для сверки с эталоном",
				L"Предупреждение!", MB_ICONWARNING);
			return false;
		}
		int startMeasure = (int)LinearFilteredOut_Data[startZone][0].size() *
			(_startPoint % 200) / 200;
		int endMeasure = (int)LinearFilteredOut_Data[endZone][0].size() *
			(_endPoint % 200) / 200;
		// Если обе точки в одной зоне
		if (startZone == endZone)
		{
			for (int j = startMeasure; j < endMeasure; j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[startZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[startZone][j])
						== Singleton->LinearResult->Brack)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[startZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[startZone][j])
							== Singleton->LinearResult->Brack)
							return true;
				}
				else
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[startZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[startZone][j])
						!= Singleton->LinearResult->Good)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[startZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[startZone][j])
							!= Singleton->LinearResult->Good)
							return true;
				}
			}
		}
		else // если в разных
		{
			// проверим начальную зону
			for (int j = startMeasure;
			j < (int)LinearFilteredOut_Data[startZone][_sensor].size(); j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[startZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[startZone][j])
						== Singleton->LinearResult->Brack)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[startZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[startZone][j])
							== Singleton->LinearResult->Brack)
							return true;
				}
				else
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[startZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[startZone][j])
						!= Singleton->LinearResult->Good)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[startZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[startZone][j])
							!= Singleton->LinearResult->Good)
							return true;
				}
			}
			// проверим промежуточные зоны
			int zone = startZone + 1;
			while (zone < endZone)
			{
				for (int j = 0;
				j < (int)LinearFilteredOut_Data[zone][_sensor].size(); j++)
				{
					if (_isBrackBorder)
					{
						if (Singleton->LinearResult->ZoneColor
							(LinearFilteredOut_Data[zone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[zone][j])
							== Singleton->LinearResult->Brack)
							return true;
						if (SystemConst::isLinearDigitalFilterIn)
							if (Singleton->LinearResult->ZoneColorIn
								(LinearFilteredIn_Data[zone][_sensor][j]
								* LinearGain[_sensor] * borderCoefficient,
								false) // LinearDeads[zone][j])
								== Singleton->LinearResult->Brack)
								return true;
					}
					else
					{
						if (Singleton->LinearResult->ZoneColor
							(LinearFilteredOut_Data[startZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[startZone][j])
							!= Singleton->LinearResult->Good)
							return true;
						if (SystemConst::isLinearDigitalFilterIn)
							if (Singleton->LinearResult->ZoneColorIn
								(LinearFilteredIn_Data[startZone][_sensor][j]
								* LinearGain[_sensor] * borderCoefficient,
								false) // LinearDeads[startZone][j])
								!= Singleton->LinearResult->Good)
								return true;
					}
				}
				zone++;
			}
			// Проверим конечную зону
			for (int j = 0; j < endMeasure; j++)
			{
				if (_isBrackBorder)
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[endZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[endZone][j])
						== Singleton->LinearResult->Brack)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[endZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[endZone][j])
							== Singleton->LinearResult->Brack)
							return true;
				}
				else
				{
					if (Singleton->LinearResult->ZoneColor
						(LinearFilteredOut_Data[endZone][_sensor][j]
						* LinearGain[_sensor] * borderCoefficient,
						false) // LinearDeads[endZone][j])
						!= Singleton->LinearResult->Good)
						return true;
					if (SystemConst::isLinearDigitalFilterIn)
						if (Singleton->LinearResult->ZoneColorIn
							(LinearFilteredIn_Data[endZone][_sensor][j]
							* LinearGain[_sensor] * borderCoefficient,
							false) // LinearDeads[endZone][j])
							!= Singleton->LinearResult->Good)
							return true;
				}
			}
		}
	}
	if (_Module == 3) // Толщиномер
	{
//		if (Singleton->ThResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("Дефект по толщиномеру за пределами трубы");
//			Application->MessageBoxW(L"Дефект эталона за пределами трубы!",
//				L"Предупреждение!", MB_ICONWARNING);
//			return false;
//		}
		// if (startZone>0) startZone--;
		// if (endZone < Singleton->ThResult->zones) endZone++;
		for (int j = startZone; j < endZone + 1; j++)
		{
			if ((Singleton->ThResult->zone_data[j] * 10) > _borderPercent)
				return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
bool TEtalonDefectsCheckForm::CheckDefType(int _Module, int _DefType,
	vector<int>dirtyFind)
{
	AnsiString a;
	a.printf("CheckDefType: M=%d, T=%d ", _Module, _DefType);
	switch (_Module)
	{
	case 1:
		a += "Поперечный ";
		switch (_DefType)
		{
		case 1:
			a += "До 3 соседних";
			break;
		case 2:
			a += "Все датчики";
			break;
		}
		break;
	case 2:
		a += "Продольный ";
		switch (_DefType)
		{
		case 1:
			a += "Все датчики";
			break;
		case 2:
			a += "1 и более";
			break;
		}
        break;
	case 3:
		a += "Толщиномер Толщина";
		break;
	}
	TPr::pr(a);
	if ((int)dirtyFind.size() == 0)
	{
		AnsiString Str = "Ошибка: Длина DirtyFind = " +
			IntToStr((int)dirtyFind.size());
		TPr::pr(Str);
		return false;
	}
	if (_Module == 1) // Поперечник
	{
		// составим список датчиков с дефектами
		std::vector<int>sensDef;
		for (int i = 0; i < (int)dirtyFind.size(); i++)
		{
			if (dirtyFind[i] == 2)
			{
				TPr::pr(
					"Ошибка: Дефект вышел за верхнюю границу на одном из датчиков");
				return false;
			}
			else if (dirtyFind[i] == 1)
			{
				sensDef.push_back(i);
			}
		}
		int DefCount = sensDef.size();
		switch (_DefType)
		{
		case 1: // до 3 соседних
				// зависимость от кол-ва датчиков нашедших дефект
			switch (DefCount)
			{
			case 1:
				TPr::pr("Успех: Дефект найден на 1 датчике поперечного");
				return true;
				break;
			case 2:
				if ((sensDef[1] - sensDef[0]) == 1 || (sensDef[1] - sensDef[0])
					== (int)dirtyFind.size() - 1)
				{
					TPr::pr(
						"Успех: Дефект найден на 2 соседних датчиках поперечного");
					return true;
				}
				else
				{
					TPr::pr(
						"Ошибка: Дефект найден на 2 не соседних датчиках поперечного");
					return false;
				}
				break;
			case 3:
				if (sensDef[0] == 0) // если есть вероятность дефекта на стыке
				{
					for (int k = 0; k < 3; k++)
					{
						sensDef[k] += 2;
						sensDef[k] = sensDef[k] % dirtyFind.size();
					}
					std::sort(sensDef.begin(), sensDef.end());
				}
				if ((sensDef[0] + 1 == sensDef[1]) && (sensDef[1] +
					1 == sensDef[2]))
				{
					TPr::pr(
						"Успех: Дефект найден на 3 соседних датчиках поперечного");
					return true;
				}
				else
				{
					TPr::pr(
						"Ошибка: Дефект найден на 3 не соседних датчиках поперечного");
					return false;
				}
				break;
			default:
				TPr::pr(
					"Ошибка: Дефект найден на более чем 3 датчиках поперечного или вообще не найден");
				return false;
				break;
			}
			break;
		case 2: // все датчики в нужном пределе
			if (DefCount == (int)dirtyFind.size())
			{
				TPr::pr("Успех: Дефект найден на всех датчиках поперечного");
				return true;
			}
			else
			{
				TPr::pr(
					"Ошибка: Дефект найден не на всех датчиках поперечного или выходит за границы");
				return false;
			}
			break;
		default:
			TPr::pr("Ошибка: Не определен тип дефекта поперечного");
			return false;
		}
	}
	else if (_Module == 2) // Продольник
	{
		switch (_DefType)
		{
		case 1: // Все датчики
			for (int i = 0; i < (int)dirtyFind.size(); i++)
			{
				if (dirtyFind[i] == 2)
				{
					TPr::pr(
						"Ошибка: Дефект вышел за верхнюю границу на одном из датчиков");
					return false;
				}
				if (dirtyFind[i] != 1)
					// Если хотя бы на одном датчике нет дефекта - ошибка
				{
					TPr::pr(
						"Ошибка: Дефект найден не на всех датчиках продольного");
					return false;
				}
			}
			TPr::pr("Успех: Дефект найден на всех датчиках продольного");
			return true; // Если все датчики видят дефект, то дефект совпадает
			break;
		case 2: // 1 и более датчиков
			{
				int sensFind = 0;
				for (int i = 0; i < (int)dirtyFind.size(); i++)
				{
					if (dirtyFind[i] == 2)
					{
						TPr::pr(
							"Ошибка: Дефект вышел за верхнюю границу на одном из датчиков");
						return false;
					}
					else if (dirtyFind[i] == 1)
					{
						sensFind++;
					}
				}
				if (sensFind == 0)
				{
					TPr::pr(
						"Ошибка: Дефект не найден ни на одном датчике продольного");
					return false;
				}
				TPr::pr(
					"Успех: Дефект найден на 1 и более датчиках продольного");
				return true;
				// Если хотя бы 1 датчик видит дефект, то дефект совпадает
				break;
			}
		default:
			TPr::pr("Ошибка: Не определен тип дефекта продольного");
			return false;
		}
	}
	else if (_Module == 3) // Толщиномер
	{
		switch (_DefType)
		{
		case 1: // Толщина
			if (dirtyFind[0] == 2)
			{
				TPr::pr("Ошибка: Дефект вышел за верхнюю границу");
				return false;
			}
			if (dirtyFind[0] != 1) // Если нет дефекта - ошибка
			{
				TPr::pr("Ошибка: Дефект толщиномера не найден");
				return false;
			}
			TPr::pr("Успех: Дефект толщиномера найден");
			return true; // Если датчики видят дефект, то дефект совпадает
			break;
		default:
			TPr::pr("Ошибка: Не определен тип дефекта толщиномера");
			return false;
		}
	}
	TPr::pr("Ошибка: Не определен модуль дефекта");
	return false;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::bCancelClick(TObject *Sender)
{
	// Закроем окно
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::bSaveClick(TObject *Sender)
{
	// сохраняем трубу
	// Ввод пароля дефектоскописта
	TPasswordForm* f = new TPasswordForm(this);
	f->SetIni(ini);
	f->SetUserName(cbSelectDefectoscoper->Text);
	f->ShowModal();
	delete f;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// Сохраняем дефектоскописта как последнего авторизовавшегося
		ini->WriteString("Default","LastAuthorisation",cbSelectDefectoscoper->Text);
		CSelect S("Select top 1 * from dbo.Defectoscopers where name = '"
			+ cbSelectDefectoscoper->Text + "'");
		ini->WriteInteger("Default","LastAuthorisationID",S.AsInt("ID"));
		//Формируем полное имя нового файла
		if (!ini->ValueExists("OtherSettings", "SOPPath"))
			ini->WriteString("OtherSettings", "SOPPath",
			"d:\\RAG\\Defectoscope_502\\SOPChecks");
		AnsiString path = ini->ReadString("OtherSettings", "SOPPath",
			"d:\\RAG\\Defectoscope_502\\SOPChecks");
		AnsiString FileName = path + "\\SOPCheck_";
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
			FILE *file;
			file = fopen(FileName.c_str(), "wb");
			if (file == NULL)
			{
				AnsiString a =
					"TEtalonDefectsCheckForm::bSaveClick: не могу открыть файл: ";
				a += FileName;
				FATAL(a);
			}

//			fclose(file);
//			// Поперечные данные
//			Singleton->CrossResult->SaveTubeToFile(FileName);
//			// Продольные данные
//			Singleton->LinearResult->SaveTubeToFile(FileName);
//			// Толщинометрия (только zone_data)
//			Singleton->ThResult->SaveTubeToFile(FileName);
//			//Сохраняет пороги и усиления для Альметьевска
//			ADCSettForm->SaveTubeSettingsToFile(FileName);
//			//Сохраним настройки эталона
//			SaveEtalonDefectsToFile(FileName);
			// Поперечные данные
			Singleton->CrossResult->SaveTubeToFile(file);
			// Продольные данные
			Singleton->LinearResult->SaveTubeToFile(file);
			// Толщинометрия (только zone_data)
			Singleton->ThResult->SaveTubeToFile(file);
			//Сохраняет пороги и усиления для Альметьевска
			ADCSettForm->SaveTubeSettingsToFile(file);
			//Сохраним настройки эталона
			SaveEtalonDefectsToFile(file);
			fclose(file);
			//Сделаем файл скрытым
			UnicodeString unicName = (UnicodeString)FileName;
			int attr = GetFileAttributes(unicName.w_str());
			if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
				SetFileAttributes(unicName.w_str(), attr | FILE_ATTRIBUTE_HIDDEN);
		}
		catch (...)
		{
			Application->MessageBoxW(L"Не удалось сохранить файл", L"Ошибка",
				MB_ICONERROR | MB_OK);
		}
		// сохраняем данные о проверке в БД
		AnsiString Str = "";
		AnsiString SQL;
		SQL = "insert into dbo.EtalonCheckResult (Date,Name,TypeSize,Result,MatchedDefects,NumOfDefects,Defectoscoper,SaveAddress) values ('";
		SQL += eDate->Text;
		SQL += "','";
		SQL += eEtalonName->Text;
		SQL += "','";
		SQL += eTypeSize->Text;
		SQL += "',";
		SQL += "'" + pResultBit->Caption + "'";
		SQL += ",";
		SQL += IntToStr(matchedDefects);
		SQL += ",";
		SQL += IntToStr(numOfDefects);
		SQL += ",'";
		SQL += cbSelectDefectoscoper->Text;
		SQL += "','";
		SQL += AnsiString(FileName);
		SQL += "')";

		CExecSQL E(SQL);
		if (E.IsOk() != "Ok")
		{
			Str = "Не смогли сохранить результат проверки эталона!";
			TPr::pr(Str);
			Application->MessageBoxW
				(L"Не смогли сохранить результат проверки!",
				L"Предупреждение!", MB_ICONWARNING);
		}
		else
		{
			Str = "Сохранили результат проверки эталона: " +
				pResultBit->Caption;
			TPr::pr(Str);
			Close();
		}
	}
}

// ---------------------------------------------------------------------------
bool TEtalonDefectsCheckForm::AddMemoResult(bool _matched, int _module,
	int _defPoint)
{
	// Формируем сообщение
	AnsiString Str = "";
	switch (_module)
	{
	case 1:
		Str += "Поперечный: ";
		break;
	case 2:
		Str += "Продольный: ";
		break;
	case 3:
		Str += "Толщиномер: ";
		break;
	default:
		return false;
		break;
	}
	Str += "точка " + IntToStr(_defPoint);
	if ((_defPoint % 200) == 0)
		Str += " в зонах " + IntToStr((_defPoint / 200)) + " и ";
	else Str += " в зоне ";
	Str += IntToStr((_defPoint / 200) + 1);

	if (_matched)
		Str += " совпала.";
	else
		Str += " не совпала!";

	// Выводим на боковую панель
	mCheckedDefects->Lines->Add(Str);
	return true;
}
// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::cbSelectDefectoscoperChange(TObject *Sender)
{
	if(cbSelectDefectoscoper->Text == "Master")
		bSave->Enabled = true;
	else bSave->Enabled = !Singleton->FromFile;
}
//---------------------------------------------------------------------------
//void TEtalonDefectsCheckForm::SaveEtalonDefectsToFile(UnicodeString FileName)
void TEtalonDefectsCheckForm::SaveEtalonDefectsToFile(FILE *file)
{
//	FILE *file;
//	file = fopen(AnsiString(FileName).c_str(), "a");
//	if (file == NULL)
//		Application->MessageBoxW(L"Не удалось открыть файл для записи",
//		L"Ошибка", MB_ICONERROR | MB_OK);

//	int intPar = 0;
//	AnsiString strPar = "";
//	double doubPar = 0.0;
//	bool boolPar = false;
//	TDateTime dtPar;
	try
	{
		int size = 0;

		QEtalons->Close();
		QEtalons->Active = false;
		QEtalons->SQL->Clear();
		AnsiString SQL = "";
		SQL = "SELECT * FROM dbo.EtalonDefect ";
		SQL += "WHERE EtalonDefect.ID = '" + IntToStr(IDTube) + "'";
		SQL += " ORDER by EtalonDefect.ID";
		QEtalons->SQL->Add(SQL);
		QEtalons->ExecSQL();
		QEtalons->Active = true;
		QEtalons->First();
	//	dtPar = QEtalons->FieldByName("Date")->AsDateTime;
	//	fwrite(&dtPar, sizeof(dtPar), 1, file);
	//	strPar = QEtalons->FieldByName("Name")->AsAnsiString;
	//	size = strPar.Length();
	//	fwrite(&size, sizeof(size), 1, file);
	//	fwrite(strPar.c_str(), size, 1, file);
	//	strPar = QEtalons->FieldByName("TypeSize")->AsAnsiString;
	//	size = strPar.Length();
	//	fwrite(&size, sizeof(size), 1, file);
	//	fwrite(strPar.c_str(), size, 1, file);
		fprintf(file, "\n");
		AnsiString strPar = DateTimeToStr(QEtalons->FieldByName("Date")->AsDateTime);
		for (int i = 1; i < strPar.Length(); i++)
		{
			if(strPar[i]==' ') strPar[i]='_';
		}
		fprintf(file, "%s ", strPar);
		fprintf(file, "\n");
		fprintf(file, "%s ", QEtalons->FieldByName("Name")->AsAnsiString);
		fprintf(file, "\n");
		fprintf(file, "%s ", QEtalons->FieldByName("TypeSize")->AsAnsiString);
		fprintf(file, "\n");

		QEtalons->Close();
		QEtalons->Active = false;
		QEtalons->SQL->Clear();
	//	AnsiString SQL = "";
		SQL = "SELECT * FROM EtalonTubeDefects ";
		SQL += "where EtalonTubeDefects.IDTube='" + IntToStr(IDTube) + "'";
		SQL += " order by EtalonTubeDefects.id";
		QEtalons->SQL->Add(SQL);
		QEtalons->ExecSQL();
		QEtalons->Active = true;
		QEtalons->First();
		int numOfDefects = 0;
		//посчитаем количество дефектов
		while (!QEtalons->Eof)
		{
			numOfDefects++;
			QEtalons->Next();
		}
	//	fwrite(&numOfDefects, sizeof(int), 1, file);
		fprintf(file,"%d ",numOfDefects);
		QEtalons->First();
		// Идем по списку дефектов эталона
		while (!QEtalons->Eof)
		{
			// пишем данные
	//		intPar = QEtalons->FieldByName("IDTube")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("IDTube")->AsInteger);
			//возьмем имя и типоразмер из инфы выше
//	//		strPar = QEtalons->FieldByName("TypeSize")->AsAnsiString;
//	//		size = strPar.Length();
//	//		fwrite(&size, sizeof(size), 1, file);
//	//		fwrite(strPar.c_str(), size, 1, file);
//			fprintf(file,"%s ",QEtalons->FieldByName("TypeSize")->AsAnsiString);
//			fprintf(file,"\n");
//	//		strPar = QEtalons->FieldByName("Name")->AsAnsiString;
//	//		size = strPar.Length();
//	//		fwrite(&size, sizeof(size), 1, file);
//	//		fwrite(strPar.c_str(), size, 1, file);
//			fprintf(file,"%s ",QEtalons->FieldByName("Name")->AsAnsiString);
//			fprintf(file,"\n");
	//		intPar = QEtalons->FieldByName("Module")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("Module")->AsInteger);
	//		intPar = QEtalons->FieldByName("DefType")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("DefType")->AsInteger);
	//		intPar = QEtalons->FieldByName("DefPoint")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("DefPoint")->AsInteger);
	//		intPar = QEtalons->FieldByName("DefRadius")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("DefRadius")->AsInteger);
	//		boolPar = QEtalons->FieldByName("isTopBrackBorder")->AsBoolean;
	//		fwrite(&boolPar, sizeof(boolPar), 1, file);
			fprintf(file,"%d ",(int)QEtalons->FieldByName("isTopBrackBorder")->AsBoolean);
	//		intPar = QEtalons->FieldByName("TopBorderPercent")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("TopBorderPercent")->AsInteger);
	//		boolPar = QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean;
	//		fwrite(&boolPar, sizeof(boolPar), 1, file);
			fprintf(file,"%d ",(int)QEtalons->FieldByName("isBottomBrackBorder")->AsBoolean);
	//		intPar = QEtalons->FieldByName("BottomBorderPercent")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("BottomBorderPercent")->AsInteger);
			QEtalons->Next();
		}
	}
	catch (...)
	{
		Application->MessageBoxW(L"Не удалось сохранить дефекты эталона", L"Ошибка",
			MB_ICONERROR | MB_OK);
	}
//	fclose(file);
}

