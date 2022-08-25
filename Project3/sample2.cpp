#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"

//#include "sample2.h"


using namespace V6;


void Sample2(UIHandle parent, LPCWSTR name, FRectF rc1, int id)
{
	FRectF rc(0,0,1000,1000);

	auto tab = D2DCreateTabControls(parent, rc, STAT_DEFAULT, L"tab1", -1 );

	D2DSetText(tab, L"0:first");

	auto t1 = D2DGetTab(tab, 0);
	auto t2 = D2DAddNewTab(tab, L"second");

	//D2DSetColor(t1, ColorF::White, ColorF::Black, ColorF::Black );

	auto t1_1 = D2DCreateEmptyControls(t1, rc, STAT_DEFAULT, NONAME);

	D2DSendMessage(t1_1, WM_D2D_SET_BKMODE, 2, 0);

	

}