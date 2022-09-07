#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"

using namespace V6;

#pragma comment (lib, "MySample")

DLLEXPORT HANDLE MySample_CreateFreePainter( UIHandle parent );


void Sample2(UIHandle parent, LPCWSTR name, FRectF rc, int id, int nest)
{
	if ( 2 < nest ) 
		return;

	auto tab = D2DCreateTabControls(parent, rc, STAT_DEFAULT, L"tab1", -1 );

	D2DSendMessage(tab, WM_D2D_SET_TAB_POSITION, 0, 0);

	D2DSetText(tab, L"0:1st-empty");

	auto t1 = D2DGetTab(tab, 0);
	auto t1_1 = D2DCreateEmptyControls(t1, rc, STAT_DEFAULT, NONAME);
	D2DSendMessage(t1_1, WM_D2D_SET_BKMODE, 2, 0);

	// ------------------------------------------------------------------
	auto t2 = D2DAddNewTab(tab, L"2nd");
	auto t1_2 = D2DCreateEmptyControls(t2, rc, STAT_DEFAULT, NONAME);
	D2DSendMessage(t1_2, WM_D2D_SET_BKMODE, 2, 0);

	D2DSendMessage(tab, WM_D2D_SET_ACTIVE_CONTROL, 0, 1);

	// MySample dll
	MySample_CreateFreePainter(t1_2);

	// -------------------------------------------------------------------
	auto t3 = D2DAddNewTab(tab, L"3rd-empty");
	auto t3_1 = D2DCreateEmptyControls(t3, rc, STAT_DEFAULT, NONAME);
	D2DSendMessage(t3_1, WM_D2D_SET_BKMODE, 2, 0);

	auto tx1 = D2DCreateTextbox(t3_1, FRectF(100,100,FSizeF(100,26)), false, STAT_DEFAULT, L"calc");
	D2DSendMessage(tx1, WM_D2D_COMMAND_SET, (WPARAM)tx1.p, (LPARAM)L"type=money" );





	//Sample2(t3_1, L"test2", FRectF(0,0,FSizeF(900,900)), 0, ++nest);

}