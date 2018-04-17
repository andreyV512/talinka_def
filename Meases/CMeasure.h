//---------------------------------------------------------------------------
#ifndef CMeasureH
#define CMeasureH
//---------------------------------------------------------------------------
class CMeasure
{
public:
	CMeasure(void);
	CMeasure(const CMeasure& _measure);
	CMeasure& operator=(const CMeasure& _measure);
	double v;
};
#endif
