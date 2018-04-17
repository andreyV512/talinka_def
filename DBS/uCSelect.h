//------------------------------------------------------------------------------
#ifndef uCSelectH
#define uCSelectH
//------------------------------------------------------------------------------
#include "uCDBS.h"
//------------------------------------------------------------------------------
class CSelect
{
private:
        TADOQuery* Q;
        AnsiString RetMsg;
        void Init(CDBS* _DBS,AnsiString _SQL);
public:
        CSelect(CDBS* _DBS,AnsiString _SQL);
        CSelect(AnsiString _SQL);
        ~CSelect(void);
        inline AnsiString IsOk(void){return(RetMsg);};

        int Count(void);
        AnsiString AsAnsiString(AnsiString _name);
        bool AsBool(AnsiString _name);
        int AsInt(AnsiString _name);
        double AsDouble(AnsiString _name);
        char AsChar(AnsiString _name);
        void Next(void);
};
//------------------------------------------------------------------------------
#endif
