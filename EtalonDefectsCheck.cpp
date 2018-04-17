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
	// �������� ���� �� ������� Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::FormShow(TObject *Sender)
{
	// ����������� ��������� ���� �� ������ � ��� �������
	LoadFormPos(this, ini);
	AnsiString Str = "�������� � �������� " + IntToStr(IDTube);
	TPr::pr(Str);
	// ����������� � ��
	QEtalons->Connection = DBS->Connection();
	// ������� ������
	mCheckedDefects->Clear();
	mCheckedDefects->Lines->Clear();
	EtalonCheck();
	// ������� ������ "���������" � ������ ����������� �����
	bSave->Enabled = !Singleton->FromFile;
}

// ---------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetIni(TIniFile* _ini)
{
	// ������ ���� � �����������
	ini = _ini;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// �������� ��������� ���� �� ������ � ��� �������
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetIDTube(int _IDTube)
{
	// ������ ID ����
	IDTube = _IDTube;
}

// ----------------------------------------------------------------------------
void TEtalonDefectsCheckForm::EtalonCheck()
{
	// if(Singleton->CrossResult->zones == 0)
	// {
	// TPr::pr("��� ����� ��� ������");
	// Application->MessageBoxW(L"��� ����� ��� ������!",
	// L"��������������!", MB_ICONWARNING);
	// Close();
	// return;
	// }
	// --������ �������� ����������������
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
    // ���������� ���������� �����������������
	AnsiString LastDef = ini->ReadString("Default","LastAuthorisation","");
	if (LastDef != "")
		cbSelectDefectoscoper->ItemIndex = cbSelectDefectoscoper->Items->IndexOf(LastDef);
	cbSelectDefectoscoperChange(this);
	// -----
	if (Singleton->CrossResult->zones == 0)
	{
		TPr::pr("��� ����� ��� ������");
		Application->MessageBoxW(L"��� ����� ��� ������!", L"��������������!",
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
	// ���� �� ������ �������� �������
	while (!QEtalons->Eof)
	{
		numOfDefects++;
		// ������ ������
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
		// ��������� ������
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
	Str = IntToStr(matchedDefects) + " �� " + IntToStr(numOfDefects);
	pResultNum->Caption = Str;
	if (matchedDefects == numOfDefects)
	{
		pResultBit->Caption = "���������";
		pResultBit->Color = clGreen;
	}
	else
	{
		pResultBit->Caption = "������";
		pResultBit->Color = clRed;
	}
	pResultNum->Color = pResultBit->Color;
	eDate->Text = DateTimeToStr((Date() + Time()));
	Str = "��������� ������ �������� �������: " + pResultBit->Caption + " " +
		pResultNum->Caption;
	TPr::pr(Str);

}

// ----------------------------------------------------------------------------
bool TEtalonDefectsCheckForm::DefectCheck(int _Module, int _DefType,
	int _DefPoint, int _DefRadius, bool _isTopBrackBorder,
	int _TopBorderPercent, bool _isBottomBrackBorder, int _BottomBorderPercent)
{
	// �������� ������:
	// ��� ������� �������
	// ��� ������� �������
	// ��������� �������� ������
	// ��������� ����� � ������
	// ��������� ������ �� ������������ ���� �������
	// ���� ��������, �� ���������� True
	// ����� ���������� False
	// ������ ������ ���������� ������
	int startPoint = _DefPoint - _DefRadius;
	int endPoint = _DefPoint + _DefRadius;

	int sensorMax = 0;
	if (_Module == 1) // ����������
			sensorMax = Singleton->CrossResult->sensors_a;
	else if (_Module == 2) // ����������
			sensorMax = Singleton->LinearResult->sensors_a;
	else if (_Module == 3) // ����������
			sensorMax = 1;
	else
	{
		TPr::pr("������: ���������� ������ �������");
		return false;
	}
	// ���������, ��� ������ ��������� � �������� �����
	int startZone = startPoint / 200;
	int endZone = endPoint / 200;
	switch (_Module)
	{
	case 1:
		if (Singleton->CrossResult->zones < endZone || startZone < 0)
		{
			TPr::pr("������ �� ����������� ������ �� ��������� �����");
			Application->MessageBoxW(L"������ ������� �� ����������� ������ �� ��������� �����!",
				L"��������������!", MB_ICONWARNING);
			return false;
		}
		break;
	case 2:
		if (Singleton->LinearResult->zones < endZone || startZone < 0)
		{
			TPr::pr("������ �� ����������� ������ �� ��������� �����");
			Application->MessageBoxW(L"������ ������� �� ����������� ������ �� ��������� �����!",
				L"��������������!", MB_ICONWARNING);
			return false;
		}
		break;
	case 3:
		if (Singleton->ThResult->zones < endZone || startZone < 0)
		{
			TPr::pr("������ �� ����������� �� ��������� �����");
			Application->MessageBoxW(L"������ ������� �� ����������� �� ��������� �����!",
				L"��������������!", MB_ICONWARNING);
			return false;
		}
		break;
	}

	// ��������� ���� ������
	vector<int>dirtyFind;
	for (int sens = 0; sens < sensorMax; sens++)
	{
		dirtyFind.push_back(0); // ���� ������ �������
		if (_TopBorderPercent != 0) // ���� ������� ������� �������
		{
			if (CheckDiap(_Module, sens, _isTopBrackBorder, _TopBorderPercent,
				startPoint, endPoint))
			{
				dirtyFind[sens] = 2; // ���� ������� �������
			}
			else
				dirtyFind[sens] = 1; // ���� ������� �������
		}
		if ((_BottomBorderPercent != 0) && (dirtyFind[sens] != 2))
			// ���� ������� ������ �������
		{
			if (CheckDiap(_Module, sens, _isBottomBrackBorder,
				_BottomBorderPercent, startPoint, endPoint))
			{
				dirtyFind[sens] = 1; // ����� ������� � ������ ���������
			}
			else
				dirtyFind[sens] = 0; // ���� ������ �������
		}
	}

	AnsiString Str = "DirtyFind = [";
	for (int i = 0; i < (int)dirtyFind.size(); i++)
	{
		Str += IntToStr(dirtyFind[i]);
	}
	Str += "]";
	TPr::pr(Str);

	// ��������� �� ������������ ���� �������
	bool Result = false;
	Result = CheckDefType(_Module, _DefType, dirtyFind);

	return Result;
}

// ----------------------------------------------------------------------------
void TEtalonDefectsCheckForm::SetData()
{
	// ����������
	CrossFilteredOut_Data = Singleton->CrossResult->GetFilteredOut_Data();
	CrossGain = Singleton->CrossResult->GetGain();
	CrossDeads = Singleton->CrossResult->GetDeads();
	// ����������
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
	if (_Module == 1) // ����������
	{
//		if (Singleton->CrossResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("������ �� ����������� ������ �� ��������� �����");
//			Application->MessageBoxW(L"������ ������� �� ��������� �����!",
//				L"��������������!", MB_ICONWARNING);
//			return false;
//		}
		if (Singleton->CrossResult->sensors_a < _sensor || _sensor < 0)
		{
			TPr::pr("������: ����������� ������ ������ ��� ������ � ��������");
			Application->MessageBoxW
				(L"������: ����������� ������ ������ ��� ������ � ��������",
				L"��������������!", MB_ICONWARNING);
			return false;
		}
		int startMeasure = (int)CrossFilteredOut_Data[startZone][0].size() *
			(_startPoint % 200) / 200;
		int endMeasure = (int)CrossFilteredOut_Data[endZone][0].size() *
			(_endPoint % 200) / 200;
		// ���� ��� ����� � ����� ����
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
		else // ���� � ������
		{
			// �������� ��������� ����
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
			// �������� ������������� ����
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
			// �������� �������� ����
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
	if (_Module == 2) // ����������
	{
//		if (Singleton->LinearResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("������ �� ����������� ������ �� ��������� �����");
//			Application->MessageBoxW(L"������ ������� �� ��������� �����!",
//				L"��������������!", MB_ICONWARNING);
//			return false;
//		}
		if (Singleton->LinearResult->sensors_a < _sensor || _sensor < 0)
		{
			TPr::pr("������: ����������� ������ ������ ��� ������ � ��������");
			Application->MessageBoxW
				(L"������: ����������� ������ ������ ��� ������ � ��������",
				L"��������������!", MB_ICONWARNING);
			return false;
		}
		int startMeasure = (int)LinearFilteredOut_Data[startZone][0].size() *
			(_startPoint % 200) / 200;
		int endMeasure = (int)LinearFilteredOut_Data[endZone][0].size() *
			(_endPoint % 200) / 200;
		// ���� ��� ����� � ����� ����
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
		else // ���� � ������
		{
			// �������� ��������� ����
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
			// �������� ������������� ����
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
			// �������� �������� ����
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
	if (_Module == 3) // ����������
	{
//		if (Singleton->ThResult->zones < endZone || startZone < 0)
//		{
//			TPr::pr("������ �� ����������� �� ��������� �����");
//			Application->MessageBoxW(L"������ ������� �� ��������� �����!",
//				L"��������������!", MB_ICONWARNING);
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
		a += "���������� ";
		switch (_DefType)
		{
		case 1:
			a += "�� 3 ��������";
			break;
		case 2:
			a += "��� �������";
			break;
		}
		break;
	case 2:
		a += "���������� ";
		switch (_DefType)
		{
		case 1:
			a += "��� �������";
			break;
		case 2:
			a += "1 � �����";
			break;
		}
        break;
	case 3:
		a += "���������� �������";
		break;
	}
	TPr::pr(a);
	if ((int)dirtyFind.size() == 0)
	{
		AnsiString Str = "������: ����� DirtyFind = " +
			IntToStr((int)dirtyFind.size());
		TPr::pr(Str);
		return false;
	}
	if (_Module == 1) // ����������
	{
		// �������� ������ �������� � ���������
		std::vector<int>sensDef;
		for (int i = 0; i < (int)dirtyFind.size(); i++)
		{
			if (dirtyFind[i] == 2)
			{
				TPr::pr(
					"������: ������ ����� �� ������� ������� �� ����� �� ��������");
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
		case 1: // �� 3 ��������
				// ����������� �� ���-�� �������� �������� ������
			switch (DefCount)
			{
			case 1:
				TPr::pr("�����: ������ ������ �� 1 ������� �����������");
				return true;
				break;
			case 2:
				if ((sensDef[1] - sensDef[0]) == 1 || (sensDef[1] - sensDef[0])
					== (int)dirtyFind.size() - 1)
				{
					TPr::pr(
						"�����: ������ ������ �� 2 �������� �������� �����������");
					return true;
				}
				else
				{
					TPr::pr(
						"������: ������ ������ �� 2 �� �������� �������� �����������");
					return false;
				}
				break;
			case 3:
				if (sensDef[0] == 0) // ���� ���� ����������� ������� �� �����
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
						"�����: ������ ������ �� 3 �������� �������� �����������");
					return true;
				}
				else
				{
					TPr::pr(
						"������: ������ ������ �� 3 �� �������� �������� �����������");
					return false;
				}
				break;
			default:
				TPr::pr(
					"������: ������ ������ �� ����� ��� 3 �������� ����������� ��� ������ �� ������");
				return false;
				break;
			}
			break;
		case 2: // ��� ������� � ������ �������
			if (DefCount == (int)dirtyFind.size())
			{
				TPr::pr("�����: ������ ������ �� ���� �������� �����������");
				return true;
			}
			else
			{
				TPr::pr(
					"������: ������ ������ �� �� ���� �������� ����������� ��� ������� �� �������");
				return false;
			}
			break;
		default:
			TPr::pr("������: �� ��������� ��� ������� �����������");
			return false;
		}
	}
	else if (_Module == 2) // ����������
	{
		switch (_DefType)
		{
		case 1: // ��� �������
			for (int i = 0; i < (int)dirtyFind.size(); i++)
			{
				if (dirtyFind[i] == 2)
				{
					TPr::pr(
						"������: ������ ����� �� ������� ������� �� ����� �� ��������");
					return false;
				}
				if (dirtyFind[i] != 1)
					// ���� ���� �� �� ����� ������� ��� ������� - ������
				{
					TPr::pr(
						"������: ������ ������ �� �� ���� �������� �����������");
					return false;
				}
			}
			TPr::pr("�����: ������ ������ �� ���� �������� �����������");
			return true; // ���� ��� ������� ����� ������, �� ������ ���������
			break;
		case 2: // 1 � ����� ��������
			{
				int sensFind = 0;
				for (int i = 0; i < (int)dirtyFind.size(); i++)
				{
					if (dirtyFind[i] == 2)
					{
						TPr::pr(
							"������: ������ ����� �� ������� ������� �� ����� �� ��������");
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
						"������: ������ �� ������ �� �� ����� ������� �����������");
					return false;
				}
				TPr::pr(
					"�����: ������ ������ �� 1 � ����� �������� �����������");
				return true;
				// ���� ���� �� 1 ������ ����� ������, �� ������ ���������
				break;
			}
		default:
			TPr::pr("������: �� ��������� ��� ������� �����������");
			return false;
		}
	}
	else if (_Module == 3) // ����������
	{
		switch (_DefType)
		{
		case 1: // �������
			if (dirtyFind[0] == 2)
			{
				TPr::pr("������: ������ ����� �� ������� �������");
				return false;
			}
			if (dirtyFind[0] != 1) // ���� ��� ������� - ������
			{
				TPr::pr("������: ������ ����������� �� ������");
				return false;
			}
			TPr::pr("�����: ������ ����������� ������");
			return true; // ���� ������� ����� ������, �� ������ ���������
			break;
		default:
			TPr::pr("������: �� ��������� ��� ������� �����������");
			return false;
		}
	}
	TPr::pr("������: �� ��������� ������ �������");
	return false;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::bCancelClick(TObject *Sender)
{
	// ������� ����
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonDefectsCheckForm::bSaveClick(TObject *Sender)
{
	// ��������� �����
	// ���� ������ ���������������
	TPasswordForm* f = new TPasswordForm(this);
	f->SetIni(ini);
	f->SetUserName(cbSelectDefectoscoper->Text);
	f->ShowModal();
	delete f;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		// ��������� ��������������� ��� ���������� �����������������
		ini->WriteString("Default","LastAuthorisation",cbSelectDefectoscoper->Text);
		CSelect S("Select top 1 * from dbo.Defectoscopers where name = '"
			+ cbSelectDefectoscoper->Text + "'");
		ini->WriteInteger("Default","LastAuthorisationID",S.AsInt("ID"));
		//��������� ������ ��� ������ �����
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
		// ������� ������ ����, ����� �������� � ���� ��������� ������
		try
		{
			FILE *file;
			file = fopen(FileName.c_str(), "wb");
			if (file == NULL)
			{
				AnsiString a =
					"TEtalonDefectsCheckForm::bSaveClick: �� ���� ������� ����: ";
				a += FileName;
				FATAL(a);
			}

//			fclose(file);
//			// ���������� ������
//			Singleton->CrossResult->SaveTubeToFile(FileName);
//			// ���������� ������
//			Singleton->LinearResult->SaveTubeToFile(FileName);
//			// ������������� (������ zone_data)
//			Singleton->ThResult->SaveTubeToFile(FileName);
//			//��������� ������ � �������� ��� ������������
//			ADCSettForm->SaveTubeSettingsToFile(FileName);
//			//�������� ��������� �������
//			SaveEtalonDefectsToFile(FileName);
			// ���������� ������
			Singleton->CrossResult->SaveTubeToFile(file);
			// ���������� ������
			Singleton->LinearResult->SaveTubeToFile(file);
			// ������������� (������ zone_data)
			Singleton->ThResult->SaveTubeToFile(file);
			//��������� ������ � �������� ��� ������������
			ADCSettForm->SaveTubeSettingsToFile(file);
			//�������� ��������� �������
			SaveEtalonDefectsToFile(file);
			fclose(file);
			//������� ���� �������
			UnicodeString unicName = (UnicodeString)FileName;
			int attr = GetFileAttributes(unicName.w_str());
			if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0)
				SetFileAttributes(unicName.w_str(), attr | FILE_ATTRIBUTE_HIDDEN);
		}
		catch (...)
		{
			Application->MessageBoxW(L"�� ������� ��������� ����", L"������",
				MB_ICONERROR | MB_OK);
		}
		// ��������� ������ � �������� � ��
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
			Str = "�� ������ ��������� ��������� �������� �������!";
			TPr::pr(Str);
			Application->MessageBoxW
				(L"�� ������ ��������� ��������� ��������!",
				L"��������������!", MB_ICONWARNING);
		}
		else
		{
			Str = "��������� ��������� �������� �������: " +
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
	// ��������� ���������
	AnsiString Str = "";
	switch (_module)
	{
	case 1:
		Str += "����������: ";
		break;
	case 2:
		Str += "����������: ";
		break;
	case 3:
		Str += "����������: ";
		break;
	default:
		return false;
		break;
	}
	Str += "����� " + IntToStr(_defPoint);
	if ((_defPoint % 200) == 0)
		Str += " � ����� " + IntToStr((_defPoint / 200)) + " � ";
	else Str += " � ���� ";
	Str += IntToStr((_defPoint / 200) + 1);

	if (_matched)
		Str += " �������.";
	else
		Str += " �� �������!";

	// ������� �� ������� ������
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
//		Application->MessageBoxW(L"�� ������� ������� ���� ��� ������",
//		L"������", MB_ICONERROR | MB_OK);

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
		//��������� ���������� ��������
		while (!QEtalons->Eof)
		{
			numOfDefects++;
			QEtalons->Next();
		}
	//	fwrite(&numOfDefects, sizeof(int), 1, file);
		fprintf(file,"%d ",numOfDefects);
		QEtalons->First();
		// ���� �� ������ �������� �������
		while (!QEtalons->Eof)
		{
			// ����� ������
	//		intPar = QEtalons->FieldByName("IDTube")->AsInteger;
	//		fwrite(&intPar, sizeof(int), 1, file);
			fprintf(file,"%d ",QEtalons->FieldByName("IDTube")->AsInteger);
			//������� ��� � ���������� �� ���� ����
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
		Application->MessageBoxW(L"�� ������� ��������� ������� �������", L"������",
			MB_ICONERROR | MB_OK);
	}
//	fclose(file);
}

