#pragma hdrstop
#include "RLCard_parameters.h"
#include "l502api.h"
#pragma package(smart_init)

void RLCard_parameters::LoadSettings(TIniFile* ini)
{
	AnsiString sect = "Type_" + ini->ReadString("Default", "TypeSize", "1");
	if (!ini->ValueExists(sect, "CrossSensors"))
		ini->WriteString(sect, "CrossSensors", "12");
	if (!ini->ValueExists(sect, "LineSensors"))
		ini->WriteString(sect, "LineSensors", "4");

	LSensors=ini->ReadInteger(sect,"LineSensors",4);
	CSensors=ini->ReadInteger(sect,"CrossSensors",12);
	OSensors=others_sensors_count;
	AnsiString name;
	int ldiameter=ini->ReadInteger(sect,"Diameter",1);

	channels.clear();
	for(int i=0;i<CSensors;i++)
	{
		RChannel_parameters chPar;

		sect="PP";
		name="Number";
		name+=i;
		chPar.logicalChannel=ini->ReadInteger(sect,name,1);

		sect="CollectedMode_PP_";
		sect+=ldiameter;
		name="Mode";
		name+=i;
		chPar.collectedMode=ini->ReadInteger(sect,name,1);

		sect="Range_PP_";
		sect+=ldiameter;
		name="Range";
		name+=i;
		chPar.range=ini->ReadInteger(sect,name,0);
		channels.push_back(chPar);
	}
	for(int i=0;i<LSensors;i++)
	{
		RChannel_parameters chPar;

		sect="PR";
		name="Number";
		name+=i;
		chPar.logicalChannel=ini->ReadInteger(sect,name,1);

		sect="CollectedMode_PR_";
		sect+=ldiameter;
		name="Mode";
		name+=i;
		chPar.collectedMode=ini->ReadInteger(sect,name,1);

		sect="Range_PR_";
		sect+=ldiameter;
		name="Range";
		name+=i;
		chPar.range=ini->ReadInteger(sect,name,0);
		channels.push_back(chPar);
	}
	others.clear();
	for(int i=0;i<others_sensors_count;i++)
	{
		RChannel_parameters ch;
		ch.range=0;
		ch.collectedMode=0;
		others.push_back(ch);
	}
	/*
	channelSolenoidsON = ini->ReadInteger("PP", "spIsSolenoidsON", 27);
	channelSolinoid1 = ini->ReadInteger("PP", "spSolenoid1", 28);
	channelSolinoid2 = ini->ReadInteger("PP", "spSolenoid2", 29);
	*/
	others[0].logicalChannel=ini->ReadInteger("PP", "spIsSolenoidsON", 27);

	others[1].logicalChannel=ini->ReadInteger("PP", "spSolenoid1", 28);

	others[2].logicalChannel=ini->ReadInteger("PP", "spSolenoid2", 29);

	/*
	others[3].logicalChannel=ini->ReadInteger("PR","voltageChannel",6);

	others[4].logicalChannel=ini->ReadInteger("OtherSettings","SensorSG",16);
	others[4].collectedMode=0;
	others[4].range=1; // плюс/минус 5В

	others[5].logicalChannel=ini->ReadInteger("OtherSettings","SensorCurr",17);
	others[5].collectedMode=0;
	others[5].range=1; // плюс/минус 5В

	others[6].logicalChannel=21;
	*/

	syncMode=ini->ReadInteger("LCard","SyncMode",0);
	syncStartMode=ini->ReadInteger("LCard","SyncStartMode",0);
	frequencyCollect=ini->ReadFloat("LCard","Friquency",1000.0);
	frequencyPerChannel=ini->ReadFloat("LCard","Rate",1000.0);
}


