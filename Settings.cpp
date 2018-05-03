// --------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Settings.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#include "Protocol.h"
#include "uFunctions.h"
#include "uTFNewTypeSize.h"
#include "Password.h"
TSettingsForm *SettingsForm;
// ---------------------------------------------------------------------------
__fastcall TSettingsForm::TSettingsForm(TComponent* Owner,TIniFile* _ini) : TForm(Owner)
{
	ini=_ini;
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormCreate(TObject *Sender)
{
	names=new TStringList();
	KeyPreview=true;
	LoadFormPos(this,ini);

	need_loadsettings=false;
	// ищем типоразмеры в файле и добавляем их в ComboBox
	TStringList *sections=new TStringList();
	ini->ReadSections(sections);
	// добавляем в список элементов те элементы из ini файла, которые характеризуют типоразмер (префикс "Type_")
	for(int i=0;i<sections->Count;i++)
	{
		if(sections->Strings[i].SubString(1,5)=="Type_")
			cbTypeSize->AddItem(sections->Strings[i].SubString(6,20),NULL);
	}
	delete sections;
	typeSizeCur=ini->ReadString("Default","TypeSize","1");
	cbTypeSize->ItemIndex=cbTypeSize->Items->IndexOf(typeSizeCur);
	// создаем строки с заполненными полями:  "имя настройки" - "текущее значение"
	LoadTypeSize(typeSizeCur);
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormDestroy(TObject*Sender)
{
	delete names;
	SaveFormPos(this,ini);
}

// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::ValueListKeyPress(TObject*Sender,wchar_t &Key)
{
	// ловим нажатие на клавишу
	switch(Key)
	{
	case VK_F1:
		WinExec("hh Help.chm::Settings.htm",SW_RESTORE);
		break;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::bCreateTypeSizeClick(TObject*Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		FNewTypeSize=new TFNewTypeSize(this,ini);
		FNewTypeSize->ShowModal();
		AnsiString typeSizeNew=FNewTypeSize->typeSize;
		delete FNewTypeSize;
		if(typeSizeNew.Length()==0)
			return;
		for(int i=0;i<cbTypeSize->Items->Count;i++)
		{
			if(cbTypeSize->Items->Strings[i]==typeSizeNew)
			{
				StatusBarBottom->Panels->Items[0]->Text="Такой типоразмер уже есть";
				return;
			}
		}
		cbTypeSize->AddItem(typeSizeNew,NULL);
		cbTypeSize->ItemIndex=cbTypeSize->Items->IndexOf(typeSizeNew);
		SaveTypeSize(typeSizeNew);
		StatusBarBottom->Panels->Items[0]->Text="";
		//return;
	}
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::bDeleteTypeSizeClick(TObject*Sender)
{
	TPasswordForm* PasswordForm = new TPasswordForm(this);
	PasswordForm->SetIni(ini);
	PasswordForm->SetUserName("Master");
	PasswordForm->ShowModal();
	delete PasswordForm;
	if (ini->ReadBool("Default", "IsPasswordOk", false))
	{
		if(cbTypeSize->Text==typeSizeCur)
		{
			StatusBarBottom->Panels->Items[0]->Text="Нельзя удалить текущий типоразмер";
			return;
		}
		DeleteTypeSize(cbTypeSize->Text);
		int lindex=cbTypeSize->ItemIndex;
		cbTypeSize->DeleteSelected();
		if(cbTypeSize->Items->Count<=lindex)
			cbTypeSize->ItemIndex=lindex-1;
		else
			cbTypeSize->ItemIndex=lindex;
		LoadTypeSize(cbTypeSize->Text);
		StatusBarBottom->Panels->Items[0]->Text="";
	}
}
// ---------------------------------------------------------------------------
void TSettingsForm::LoadTypeSize(AnsiString _typeSize)
{
	// изменился типоразмер - нужно показать его настройки. удаляем все поля и заполняем заново
	ValueListEditor->Strings->Clear();
	names->Clear();
	String sect="Type_"+_typeSize;
	names->Add("Diameter");
	int pos;
	pos=ValueListEditor->InsertRow("Диаметр трубы, мм",
		ini->ReadInteger(sect,"Diameter",0),true);
	ValueListEditor->ItemProps[pos-1]->EditStyle = esPickList;
	  //	ValueListEditor->ItemProps[pos-1]->PickList->Add("48");
		ValueListEditor->ItemProps[pos-1]->PickList->Add("60");
		ValueListEditor->ItemProps[pos-1]->PickList->Add("73");
		ValueListEditor->ItemProps[pos-1]->PickList->Add("89");
  //		ValueListEditor->ItemProps[pos-1]->PickList->Add("114");

	names->Add("CrossBorder1");
	ValueListEditor->InsertRow("Поперечный порог брака",
		ini->ReadFloat(sect,"CrossBorder1",0.0),true);
	names->Add("CrossBorder2");
	ValueListEditor->InsertRow("Поперечный порог класса 2",
		ini->ReadFloat(sect,"CrossBorder2",0.0),true);

	names->Add("LinearBorder1");
	ValueListEditor->InsertRow("Продольный порог брака",
		ini->ReadFloat(sect,"LinearBorder1",0.0),true);
	names->Add("LinearBorder2");
	ValueListEditor->InsertRow("Продольный порог класса 2",
		ini->ReadFloat(sect,"LinearBorder2",0.0),true);

	names->Add("LinearBorder1Inner");
	ValueListEditor->InsertRow("Продольный порог брака внутренний",
		ini->ReadFloat(sect,"LinearBorder1Inner",0.0),true);
	names->Add("LinearBorder2Inner");
	ValueListEditor->InsertRow("Продольный порог класса 2 внутренний",
		ini->ReadFloat(sect,"LinearBorder2Inner",0.0),true);

	names->Add("ThicknessBorder1");
	ValueListEditor->InsertRow("Толщинометрия: порог брака",
		ini->ReadFloat(sect,"ThicknessBorder1",0.0),true);
	names->Add("ThicknessBorder2");
	ValueListEditor->InsertRow("Толщинометрия: порог класса 2",
		ini->ReadFloat(sect,"ThicknessBorder2",0.0),true);
	names->Add("Min_Good_Length");
	ValueListEditor->InsertRow("Минимальный годный участок",
		ini->ReadInteger(sect,"Min_Good_Length",0),true);
	names->Add("CrossDeadZoneStart");
	ValueListEditor->InsertRow("Мёртвая зона поперечного, начало",
		ini->ReadInteger(sect,"CrossDeadZoneStart",0),true);
	names->Add("CrossDeadZoneFinish");
	ValueListEditor->InsertRow("Мёртвая зона поперечного, конец",
		ini->ReadInteger(sect,"CrossDeadZoneFinish",0),true);
	names->Add("LinearDeadZoneStart");
	ValueListEditor->InsertRow("Мёртвая зона продольного, начало",
		ini->ReadInteger(sect,"LinearDeadZoneStart",0),true);
	names->Add("LinearDeadZoneFinish");
	ValueListEditor->InsertRow("Мёртвая зона продольного, конец",
		ini->ReadInteger(sect,"LinearDeadZoneFinish",0),true);
	names->Add("InSpeed");
	ValueListEditor->InsertRow("Скорость продольного модуля на входе",
		ini->ReadInteger(sect,"InSpeed",10),true);
	names->Add("WorkSpeed");
	ValueListEditor->InsertRow("Скорость продольного модуля в работе",
		ini->ReadInteger(sect,"WorkSpeed",35),true);
	names->Add("OutSpeed");
	ValueListEditor->InsertRow("Скорость продольного модуля на выходе",
		ini->ReadInteger(sect,"OutSpeed",20),true);
	names->Add("CrossSensors");
	pos=ValueListEditor->InsertRow("Количество датчиков поперечного",
		ini->ReadInteger(sect,"CrossSensors",12),true);
	ValueListEditor->ItemProps[pos-1]->EditStyle=esPickList;
	ValueListEditor->ItemProps[pos-1]->PickList->Add("8");
	ValueListEditor->ItemProps[pos-1]->PickList->Add("10");
	ValueListEditor->ItemProps[pos-1]->PickList->Add("12");
	ValueListEditor->ItemProps[pos-1]->ReadOnly=true;

	StatusBarBottom->Panels->Items[0]->Text="";
}
// ---------------------------------------------------------------------------
void TSettingsForm::SaveTypeSize(AnsiString _typeSize)
{
	String Sect="Type_"+_typeSize;
	// проверяем, все ли норм значения ввел юзер
	try
	{
		for(int i=0;i<names->Count;i++)
		{
			double tmp=ValueListEditor->Cells[1][i+1].ToDouble();
			ini->WriteFloat(Sect,names->Strings[i],tmp);
		}
	}
	catch(EConvertError &e)
	{
		e.Message="Fuck";
		StatusBarBottom->Panels->Items[0]->Text="Неправильное значение!";
	}
}
// ---------------------------------------------------------------------------
void TSettingsForm::DeleteTypeSize(AnsiString _typeSize)
{
	ini->EraseSection("Type_"+_typeSize);
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::cbTypeSizeSelect(TObject*Sender)
{
	LoadTypeSize(cbTypeSize->Text);
	StatusBarBottom->Panels->Items[0]->Text="";
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::ValueListEditorValidate(TObject*Sender,int ACol,
	int ARow, const UnicodeString KeyName, const UnicodeString KeyValue)
{
	validate();
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormCloseQuery(TObject*Sender,bool &CanClose)
{
	if(!validate())
		CanClose=false;
}
// ---------------------------------------------------------------------------
bool TSettingsForm::validate(void)
{
	AnsiString val_new=ValueListEditor->Cells[1][ValueListEditor->Row];
	ValueListEditor->RestoreCurrentRow();
	AnsiString val_old=ValueListEditor->Cells[1][ValueListEditor->Row];
	try
	{
		val_new.ToDouble();
	}
	catch(EConvertError &e)
	{
		e.Message="Fuck";
		StatusBarBottom->Panels->Items[0]->Text="Значение некорректно";
		return (false);
	}
	ValueListEditor->Cells[1][ValueListEditor->Row]=val_new;
	StatusBarBottom->Panels->Items[0]->Text="";
	ini->WriteString("Type_"+cbTypeSize->Text,
		names->Strings[ValueListEditor->Row-1],
		val_new);
	StatusBarBottom->Panels->Items[0]->Text="Значение изменено";
	if(cbTypeSize->Text==typeSizeCur)
		need_loadsettings=true;
	return (true);
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::FormResize(TObject*Sender)
{
	ValueListEditor->Width=ClientWidth-2*ValueListEditor->Left;
	ValueListEditor->Height=StatusBarBottom->Top-ValueListEditor->Left-ValueListEditor->Top;
}
// ---------------------------------------------------------------------------
void __fastcall TSettingsForm::ValueListEditorGetEditText(TObject *Sender, int ACol,
		  int ARow, UnicodeString &Value)
{
	StatusBarBottom->Panels->Items[0]->Text="";
}
//---------------------------------------------------------------------------

