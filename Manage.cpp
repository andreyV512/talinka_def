// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

// using Microsoft::ManagementConsole::Advanced;

#include "Manage.h"
#include "Global.h"
#include "Solenoid.h"
#include "Inverter.h"
#include "SignalListDef.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManageForm *ManageForm;
// ---------------------------------------------------------------------------
__fastcall TManageForm::TManageForm(TComponent* Owner)
	: TForm(Owner)
{
	// задаем имена для кнопок и состояний, картинки и цвета
	ManageForm->Refresh();
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::FormShow(TObject *Sender)
{
	// читаем цвета из файла
	TIniFile *ini=new TIniFile(Globals::IniFileName);

	on=(TColor)ini->ReadInteger("Color","ActiveIn",0);
	off=(TColor)ini->ReadInteger("Color","NotActiveIn",0);
	move=(TColor)ini->ReadInteger("Color","NotActiveOut",0);

	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
		workSpeed = ini->ReadInteger(sect, "WorkSpeed", 4);

	delete ini;

	TPr::pr("TManageForm::FormShow: getParameterSpeed");
  //	eFriquencyRot->Text=IntToStr(frConverter->getParameterSpeed(Globals::defaultRotParameter));
	TPr::pr("TManageForm::FormShow: getParameterSpeed ---");

  //	bRotation->Caption="Включить вращение";
	// включаем таймер, отслеживающий состояние входов и выходов
	butt_enabled=true;
	this->StatusBarBottom->Panels->Items[0]->Text="";
	this->StatusBarBottom->Refresh();
	Timer->Enabled=true;
	bCrossSolenoid->Caption="Включить магнитное поле";
  //	SLD->oLPCHPOW->Set(true);
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormClose(TObject *Sender,TCloseAction &Action)
{
	// выключаем таймер, отслеживающий состояние входов и выходов
	Timer->Enabled=false;
	// сбрасываем сигналы, если пользователь забыл об этом
	SLD->oCSOLPOW->Set(false);
   //	SLD->oLSOLPOW->Set(false);
   //	bLinearSolenoid->Caption="Включить магнитное поле";
   //	bCrossSolenoid->Caption="Включить магнитное поле";
   frConverter->stopRotation();
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::FormKeyPress(TObject *Sender,wchar_t &Key)
{
	if(Key==27)
		ManageForm->Close();
}
// ---------------------------------------------------------------------------
void __fastcall TManageForm::bLinearSolenoidClick(TObject *Sender)
{
/*
	if(!SLD->oLSOLPOW->Get())
	{
		SLD->oLSOLPOW->Set(true);
		bLinearSolenoid->Caption="Отключить магнитное поле";
	}
	else if(SLD->oLSOLPOW->Get())
	{
		SLD->oLSOLPOW->Set(false);
		bLinearSolenoid->Caption="Включить магнитное поле";
	}
	StatusBarBottom->Panels->Items[0]->Text="";
	*/
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bCrossSolenoidClick(TObject *Sender)
{
	if(!SLD->oCSOLPOW->Get())
	{
		bCrossSolenoid->Caption="Отключить магнитное поле";
		SLD->oCSOLPOW->Set(true);
	}
	else if(SLD->oCSOLPOW->Get())
	{
		bCrossSolenoid->Caption="Включить магнитное поле";
		SLD->oCSOLPOW->Set(false);
	}
	StatusBarBottom->Panels->Items[0]->Text="";
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::bRotationClick(TObject *Sender)
{
	if(SLD->iLPCHRUN->Get())
	{
		frConverter->stopRotation();
		/*
		if(!frConverter->stopRotation())
		{
			StatusBarBottom->Panels->Items[0]->Text="Не удалось остановить вращение";
			StatusBarBottom->Refresh();
		}
		else
		{
		  //	bRotation->Caption="Включить вращение";
			StatusBarBottom->Panels->Items[0]->Text="Остановили вращение";
			StatusBarBottom->Refresh();
		}
		*/
	}
	else
	{
		frConverter->setParameterSpeed(Globals::defaultRotParameter, workSpeed);
		frConverter->startRotation();
		/*
		if(!frConverter->setParameterSpeed(Globals::defaultRotParameter, workSpeed))
		{
			StatusBarBottom->Panels->Items[0]->Text="Не удалось выставить скорость";
			StatusBarBottom->Refresh();
			return;
		}

		if(!frConverter->startRotation())
			StatusBarBottom->Panels->Items[0]->Text="Не удалось включить вращение";
		else
		{
			StatusBarBottom->Panels->Items[0]->Text="Включили вращение";
		  //	bRotation->Caption="Выключить вращение";
		}
		 */
		//StatusBarBottom->Refresh();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::TimerTimer(TObject *Sender)
{
		if(CrossSolenoid->SolenoidOn())
		{
			 lbSolenoidON->Caption = "Магнитное поле включено";
		}
		else
		{
             lbSolenoidON->Caption = "Магнитное поле отключено";
		}
		double t;
		CrossSolenoid->Solenoid1U(t);
		t = int(10 * t) / 10.0;
		edSolenoid1->Text = FloatToStr(t);
		CrossSolenoid->Solenoid2U(t);
		t = int(10 * t) / 10.0;
		edSolenoid2->Text = FloatToStr(t);
/*
	if(SLD->oCSOLPOW->Get() && !CrossSolenoid->SolenoidOn())//OkResist())
	{
		SLD->oCSOLPOW->Set(false);
		bCrossSolenoid->Caption="Включить магнитное поле";
		StatusBarBottom->Panels->Items[0]->Text="Соленоиды поперечного перегреты";
		StatusBarBottom->Refresh();
	}
	*/
	/*
	if(SLD->oLSOLPOW->Get() && !LinearSolenoid->OkResist())
	{
		SLD->oLSOLPOW->Set(false);
		bLinearSolenoid->Caption="Включить магнитное поле";
		StatusBarBottom->Panels->Items[0]->Text="Соленоиды продольного перегреты";
		StatusBarBottom->Refresh();
	}
	if(SLD->oLSOLPOW->Get())
	{
//		TPr::pr("Расчет панели продольного");
		eLinearAmperage->Text=FloatToStrF(LinearSolenoid->getAmperage(),ffNumber,8,1);
		eLinearVoltage->Text=FloatToStrF(LinearSolenoid->getVoltage(),ffNumber,8,0);
		eLinearTemperature->Text=FloatToStrF(LinearSolenoid->getResist(),ffNumber,8,0);
	}
	else
	{
		eLinearAmperage->Text=" ";
		eLinearVoltage->Text=" ";
		eLinearTemperature->Text=" ";
	}
	*/
	/*
	if(SLD->oCSOLPOW->Get())
	{
		eCrossAmperage->Text=FloatToStrF(CrossSolenoid->getAmperage(),ffNumber,8,1);
		eCrossVoltage->Text=FloatToStrF(CrossSolenoid->getVoltage(),ffNumber,8,0);
		eCrossTemperature->Text=FloatToStrF(CrossSolenoid->getResist(),ffNumber,8,0);
	}
	else
	{
		eCrossAmperage->Text=" ";
		eCrossVoltage->Text=" ";
		eCrossTemperature->Text=" ";
	}
	*/
}
// ---------------------------------------------------------------------------

void __fastcall TManageForm::btRotationClick(TObject *Sender)
{
	if(1 == btRotation->Tag)
	{
		btRotation->Tag = 0;
		btRotation->Caption = "Вращать";
		frConverter->stopRotation();
		StatusBarBottom->Panels->Items[0]->Text="Стоп вращения";
		StatusBarBottom->Refresh();
	}
	else
	{

		int speed = 0;
		if(cbRL->Checked) speed = 1;
		if(cbRM->Checked) speed |= 2;
		if(cbRH->Checked) speed |= 4;
		if(!frConverter->setParameterSpeed(Globals::defaultRotParameter, speed))
		{
			StatusBarBottom->Panels->Items[0]->Text="Не удалось выставить скорость вращение";
			StatusBarBottom->Refresh();
			return;
		}
		else
		{
			StatusBarBottom->Panels->Items[0]->Text="";
			StatusBarBottom->Refresh();
		}
		if(!frConverter->startRotation())
		{
            StatusBarBottom->Panels->Items[0]->Text="Не удалось включить вращение";
			StatusBarBottom->Refresh();
			return;
		}
		else
		{
             StatusBarBottom->Panels->Items[0]->Text="";
			StatusBarBottom->Refresh();
		}
		btRotation->Tag = 1;
		btRotation->Caption = "Остановить";
	}
}
//---------------------------------------------------------------------------

