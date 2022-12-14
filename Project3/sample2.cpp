#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"

using namespace V6;

#pragma comment (lib, "MySample")

DLLEXPORT HANDLE MySample_CreateFreePainter( UIHandle parent );
DLLEXPORT HANDLE MySample_CreateChartBox( UIHandle parent, D2D1_RECT_F rc, LPCWSTR default_cd );


UIHandle gtx1;
DWORD MyTest(UIHandle sender, LPCWSTR eventName, void* param)
{
	BSTR text = nullptr;
	D2DSendMessage(sender, WM_D2D_GET_SELECTED_STRING,0,(LPARAM)&text);


	if ( text )
	{
		D2DSetFont(gtx1, text, 12, false,0 );

		::SysFreeString(text);
	}


	return 0;
}


void Sample2(UIHandle parent, LPCWSTR name, FRectF rc, int id, int nest)
{
	if ( 2 < nest ) 
		return;

	// 1 TAG-------------------------------------------------------------------
	auto tab = D2DCreateTabControls(parent, rc, STAT_DEFAULT|STAT_AUTOFIT_CHILDWIN|STAT_AUTOFIT_CHILDWIN, L"tab1", -1 );

	

	D2DSendMessage(tab, WM_D2D_SET_TAB_POSITION, 0, 0);

	D2DSetText(tab, L"0:chart");

	auto t1 = D2DGetTab(tab, 0);

	float xoff = 0;

	MySample_CreateChartBox(t1, FRectF(xoff,0,FSizeF(rc.Size().width-xoff, 800)), L"SPY");


	// 2 TAG------------------------------------------------------------------
	auto t2 = D2DAddNewTab(tab, L"2nd");
	auto t1_2 = D2DCreateEmptyControls(t2, rc, STAT_DEFAULT|STAT_AUTOFIT_CHILDWIN, NONAME);
	D2DSendMessage(t1_2, WM_D2D_SET_BKMODE, 2, 0);

	D2DSendMessage(tab, WM_D2D_SET_ACTIVE_CONTROL, 0, 1);

	// MySample dll
	MySample_CreateFreePainter(t1_2);

	// 3 TAG-------------------------------------------------------------------
	auto t3 = D2DAddNewTab(tab, L"3rd-empty");

	auto t31 = D2DCreateFreameControls(t3, rc, 2,2, STAT_DEFAULT|STAT_AUTOFIT_TOP_WIN, NONAME, 1 );

	auto t31a = D2DGetControls(t31, 0);
	auto t31b = D2DGetControls(t31, 3);	

	auto tt = { t31a, t31b };
	
	for(auto t : tt )
	{
		auto t3_1 = D2DCreateEmptyControls(t, rc, STAT_DEFAULT|STAT_AUTOFIT_CHILDWIN, NONAME);
		D2DSendMessage(t3_1, WM_D2D_SET_BKMODE, 2, 0);

		auto tx1 = D2DCreateTextbox(t3_1, FRectF(100,100,FSizeF(200,26)), false, STAT_DEFAULT, L"calc");
		D2DSendMessage(tx1, WM_D2D_SET_TEXT, (WPARAM)0, (LPARAM)L"SEGA MEGA DRIVE" );

		gtx1 = tx1;

		auto ls1 = D2DCreateListbox(t3_1, FRectF(100,150,FSizeF(200, 100)), STAT_DEFAULT, L"ls1");
		D2DSendMessage(ls1, WM_D2D_LISTBOX_ADD_ITEM, (WPARAM)0, (LPARAM)L"MS ?S?V?b?N" );
		D2DSendMessage(ls1, WM_D2D_LISTBOX_ADD_ITEM, (WPARAM)0, (LPARAM)L"MS ????" );
		D2DSendMessage(ls1, WM_D2D_LISTBOX_ADD_ITEM, (WPARAM)0, (LPARAM)L"Arial" );
		D2DSendMessage(ls1, WM_D2D_LISTBOX_ADD_ITEM, (WPARAM)0, (LPARAM)L"Times New Roman" );

		D2DEventHandler(ls1, MyTest);

	}
	
//MySample_CreateChartBox(t3_1, FRectF(500,0,FSizeF(1000,500)), L"SPY");
	

}