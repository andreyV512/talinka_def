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
	// �������� ���� �� ������� Esc
	if (Key == 27)
		Close();
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormShow(TObject *Sender)
{
	// ����������� ��������� ���� �� ������ � ��� �������
	LoadFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormActivate(TObject *Sender)
{
	// ����������� � ��
	QEtalons->Connection = DBS->Connection();
}

// ---------------------------------------------------------------------------
void TEtalonNewDefectForm::SetIni(TIniFile* _ini)
{
	// ������ ���� � �����������
	ini = _ini;
	// ������� ����������
	PointEdit = false;
	PointNum = 0;
}

// ----------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::FormClose(TObject *Sender,
	TCloseAction &Action)
{
	// ������� ����������
	PointEdit = false;
	PointNum = 0;
	// �������� ��������� ���� �� ������ � ��� �������
	SaveFormPos(this, ini);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::bSaveClick(TObject *Sender)
{
	//�������� ��������� �� ��� ����
	if (eIDTube->Text != "" && eName->Text != "" && eTypeSize->Text != "" &&
		eDefPoint->Text != "" && eDefRadius->Text != "" &&
		eTopPercentage->Text != "" && eBottomPercentage->Text != "")
	{
		// ��������� ������ ��� SQL
		AnsiString Str = "";
		AnsiString SQL;
		if (PointEdit == false) //����� ������ ��� ���
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
		else // �������������� ������� ������� ���
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
		// ��������� ������
		CExecSQL E(SQL);
		if (E.IsOk() != "Ok")
		{
			Str = "�� ������ ��������� ������ �������!";
			TPr::pr(Str);
			Application->MessageBoxW(L"�� ������ ��������� ������ �������!",
				L"��������������!", MB_ICONWARNING);
		}
		else
		{
			Str = "��������� ������ �������";
			TPr::pr(Str);
			// ������� ����
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
		Application->MessageBoxW(L"��������� ��� ����!", L"��������������!",
		MB_ICONWARNING);
}

// ---------------------------------------------------------------------------
void __fastcall TEtalonNewDefectForm::bAbortClick(TObject *Sender)
{
	// ������� ����
	Close();
}
// ---------------------------------------------------------------------------

void __fastcall TEtalonNewDefectForm::cbModuleChange(TObject *Sender)
{
	if (cbModule->ItemIndex == 0) // ����������
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("�� 3 ��������");
		cbDefType->Items->Add("��� �������");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
	}
	else if (cbModule->ItemIndex == 1) // ����������
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("��� �������");
		cbDefType->Items->Add("1 � �����");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
	}
	else if (cbModule->ItemIndex == 2) // ����������
	{
		cbDefType->Items->Clear();
		cbDefType->Items->Add("�������");
		cbDefType->ItemIndex = 0;
		cbTopBorder->Visible = false;
		cbBottomBorder->Visible = false;
		lTopPercent->Caption = "��";
		lBottomPercent->Caption = "��";
	}
	else
		cbDefType->Items->Clear();
}

// ---------------------------------------------------------------------------
void TEtalonNewDefectForm::SetModuleType(int _DefModule, int _DefType)
{
    // �������� ������ ������ � ��� ������� �� ������� �� ����� (��� ��������������)
	cbModule->ItemIndex = _DefModule;
	switch (_DefModule)
	{
	case 0:	//����������
		cbDefType->Items->Clear();
		cbDefType->Items->Add("�� 3 ��������");
		cbDefType->Items->Add("��� �������");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
		break;
	case 1:	//����������
		cbDefType->Items->Clear();
		cbDefType->Items->Add("��� �������");
		cbDefType->Items->Add("1 � �����");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = true;
		cbBottomBorder->Visible = true;
		lTopPercent->Caption = "%";
		lBottomPercent->Caption = "%";
		break;
	case 2:	//����������
		cbDefType->Items->Clear();
		cbDefType->Items->Add("�������");
		cbDefType->ItemIndex = _DefType;
		cbTopBorder->Visible = false;
		cbBottomBorder->Visible = false;
		lTopPercent->Caption = "��";
		lBottomPercent->Caption = "��";
		break;
	}
}
void __fastcall TEtalonNewDefectForm::bManualBorderSetClick(TObject *Sender)
{
	//������ ���
	int zoneLength = Singleton->CrossResult->GetZoneLength();
	int startZone = (eDefPoint->Text.ToInt() - eDefRadius->Text.ToInt())/zoneLength;
//	if((eDefPoint->Text.ToInt() - eDefRadius->Text.ToInt())%zoneLength == 0)
//		startZone++;
	int lastZone = (eDefPoint->Text.ToInt() + eDefRadius->Text.ToInt())/zoneLength;
	if((eDefPoint->Text.ToInt() + eDefRadius->Text.ToInt())%zoneLength == 0)
		lastZone--;
	if (cbModule->ItemIndex == 0) // ����������
	{
		Singleton->CrossResult->EtalonBorderSet(startZone, lastZone, this, ini);
		//todo ��������� ���������
	}
	else if (cbModule->ItemIndex == 1) // ����������
	{
		Singleton->LinearResult->EtalonBorderSet(startZone, lastZone, this, ini);
		//todo ��������� ���������
	}
	else if (cbModule->ItemIndex == 2) // ����������
	{
		Application->MessageBoxW(L"������ ��������� � ������!"
			,L"��������������!", MB_ICONWARNING);
	}
	else
		cbDefType->Items->Clear();
}
//---------------------------------------------------------------------------

