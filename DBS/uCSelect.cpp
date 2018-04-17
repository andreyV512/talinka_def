//------------------------------------------------------------------------------
#pragma hdrstop
#include "uCSelect.h"
#include "uFunctions.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------
CSelect::CSelect(CDBS* _DBS,AnsiString _SQL)
{
        Init(_DBS,_SQL);
}
//------------------------------------------------------------------------------
CSelect::CSelect(AnsiString _SQL)
{
        Init(DBS,_SQL);
}
//------------------------------------------------------------------------------
void CSelect::Init(CDBS* _DBS,AnsiString _SQL)
{
        RetMsg="Ok";
        Q=new TADOQuery(NULL);
        Q->Connection=_DBS->Connection();
        Q->SQL->Add(_SQL);
        try
        {
                Q->Active=true;
                Q->First();
        }
        catch(Exception &ex)
        {
                RetMsg="Îøèáêà: ";
                RetMsg+=_SQL;
                RetMsg+=" ";
                RetMsg+=ex.Message;
        }
        catch(...)
        {
                RetMsg="Îøèáêà: ";
                RetMsg+=_SQL;
        }
}
//------------------------------------------------------------------------------
CSelect::~CSelect()
{
        delete Q;
}
//------------------------------------------------------------------------------
int CSelect::Count(void)
{
        if(RetMsg!="Ok")
                return(0);
        return(Q->RecordCount);
};
//------------------------------------------------------------------------------
AnsiString CSelect::AsAnsiString(AnsiString _name)
{
        if(RetMsg!="Ok")
                return(NULL);
        return(Q->FieldByName(_name)->AsString);
};
//------------------------------------------------------------------------------
bool CSelect::AsBool(AnsiString _name)
{
        if(RetMsg!="Ok")
                return(false);
        return(Q->FieldByName(_name)->AsBoolean);
};
//------------------------------------------------------------------------------
int CSelect::AsInt(AnsiString _name)
{
        if(RetMsg!="Ok")
                return(0);
        return(Q->FieldByName(_name)->AsInteger);
};
//------------------------------------------------------------------------------
double CSelect::AsDouble(AnsiString _name)
{
        if(RetMsg!="Ok")
                return(0);
        return((Q->FieldByName(_name)->AsFloat));
};
//------------------------------------------------------------------------------
char CSelect::AsChar(AnsiString _name)
{
        AnsiString ret=AsAnsiString(_name);
        if(ret.Length()==0)
                return('-');
        return(*(ret.c_str()));
}
//------------------------------------------------------------------------------
void CSelect::Next(void)
{
        if(RetMsg!="Ok")
                return;
        Q->Next();
};
//------------------------------------------------------------------------------

