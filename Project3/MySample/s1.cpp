#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "D2DAppBaseControls.h"
#include "D2DMyControls.h"
#include "D2DMyStockChart.h"

using namespace V6;

DLLEXPORT HANDLE MySample_CreateFreePainter( UIHandle parent )
{
	auto obj =  new D2DSampleControls(); // auto delete : WM_D2D_DESTROY

	obj->Create(parent, L"hoi",  FRectF(0,0,0,0), 0);

	return nullptr;
}


//DLLEXPORT HANDLE MySample_CreateChartBox( UIHandle parent, D2D1_RECT_F rc, LPCWSTR cd  )
//{
//	auto obj =  new D2DMyStockChart(rc.right-rc.left, rc.bottom-rc.top); // auto delete : WM_D2D_DESTROY
//		
//	obj->Create(parent, L"sample chart",  rc, 0);
//
//	
//
//	D2DSendMessage(parent, WM_D2D_APP_SETDEFAULT_CD, (WPARAM)obj, (LPARAM)cd);
//
//	return nullptr;
//}

DLLEXPORT HANDLE MySample_CreateChartBox( UIHandle parent, D2D1_RECT_F rc, LPCWSTR cd  )
{
	UIHandle tabcontrols = D2DCreateTabControls(parent, rc, STAT_DEFAULT, L"TAB1");
	D2DSendMessage(tabcontrols, WM_D2D_SET_TAB_POSITION, (WPARAM)0, (LPARAM)1);
	auto h1 = D2DGetTab(tabcontrols,0);

	// tab page : 1
	auto obj =  new D2DMyStockChart(rc.right-rc.left, rc.bottom-rc.top); // auto delete : WM_D2D_DESTROY		
	obj->Create(h1, L"sample chart",  rc, 0);
	D2DSendMessage(parent, WM_D2D_APP_SETDEFAULT_CD, (WPARAM)obj, (LPARAM)cd);	
	D2DSetText(tabcontrols, L"0:chart");


	// tab page : 2
	auto h2 = D2DAddNewTab(tabcontrols, L"data view");
	auto obj2 =  new D2DMyStockDataView(rc.right-rc.left, rc.bottom-rc.top); // auto delete : WM_D2D_DESTROY		
	obj2->Create(h2, L"data view",  rc, 0);

	return nullptr;
}