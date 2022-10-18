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


DLLEXPORT HANDLE MySample_CreateChartBox( UIHandle parent )
{
	auto obj =  new D2DMyStockChart(1000,600); // auto delete : WM_D2D_DESTROY

	obj->Create(parent, L"sample chart",  FRectF(0,0,FSizeF(1100,800)), 0);

	return nullptr;
}