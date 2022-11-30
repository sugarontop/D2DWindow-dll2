#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "FD2DAppBaseControls.h"
#include "FD2DMyControls.h"
#include "FD2DMyStockChart.h"
#include "FD2DFrame.h"
using namespace V6;

DLLEXPORT HANDLE MySample_CreateFreePainter( UIHandle parent )
{
	auto obj =  new FD2DSampleControls();

	obj->Create(parent, L"hoi",  FRectF(0,0,0,0), 0);

	return nullptr;
}

DLLEXPORT HANDLE MySample_CreateChartBox( UIHandle parent, D2D1_RECT_F rc, LPCWSTR cd  )
{
	//================= bottom tab page ======================================================	
	UIHandle tabcontrols = D2DCreateTabControls(parent, rc, STAT_DEFAULT|CREATE_SIMPLE, L"TAB1");
	D2DSendMessage(tabcontrols, WM_D2D_SET_TAB_POSITION, (WPARAM)0, (LPARAM)1);
	auto h1 = D2DGetTab(tabcontrols,0);

	D2D1_RECT_F rc1;
	D2DGetClientRect(h1, &rc1);



	//================= bottom tab page : 1 ======================================================
	auto obj =  new FD2DMyStockChart();

	FRectF rc2(rc1);
	rc2.bottom -= 200;

	obj->Create(h1, L"sample chart",  rc2, 0);
	D2DSendMessage(parent, WM_D2D_APP_SETDEFAULT_CD, (WPARAM)obj, (LPARAM)cd);	
	D2DSetText(tabcontrols, L"0:chart");

	//================= bottom tab page : 2 ======================================================
	auto h2 = D2DAddNewTab(tabcontrols, L"data view");
	auto obj2 =  new FD2DMyStockDataView();
	obj2->Create(h2, L"data view",  rc1, 0);


	// tab page : 3
	//auto h3 = D2DAddNewTab(tabcontrols, L"short chart");
	//auto obj3 =  new FD2DMyStockChart(rc1.right-rc1.left, rc1.bottom-rc1.top); // auto delete : WM_D2D_DESTROY		
	//obj3->Create(h3, L"sample chart",  rc, 0);
	//D2DSendMessage(parent, WM_D2D_APP_SETDEFAULT_CD, (WPARAM)obj3, (LPARAM)cd);	
	//D2DSetText(tabcontrols, L"2:chart");

	//auto Gedan = D2DCreateButton(h1, FRectF(0,800,1000,1000), STAT_DEFAULT, NONAME, 1000);

	return 0;
	 
	FD2DFrame* fd = new FD2DFrame();	
	
	fd->Add(LASTRC,LASTRC, h1);
	//fd->Add(LASTRC,LASTRC, Gedan );

	auto func = [](LPVOID capture, UINT message, WPARAM wParam, LPARAM lParam)->LRESULT
	{
		switch(message)
		{
			case WM_D2D_DESTROYEX:
			{
				FD2DFrame* a = (FD2DFrame*)capture;
				delete a;
			}
			break;
			case WM_SIZE:
			{
				auto cx = LOWORD(lParam);
				auto cy = HIWORD(lParam);

				FD2DFrame* a = (FD2DFrame*)capture;

				a->SizeArrange(cx, cy);



			}
			break;
		}
		return 0;
	};

	D2DExportWndHandler(tabcontrols, func, fd);

	return nullptr;
}