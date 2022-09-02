#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "D2DAppBaseControls.h"
#include "MyControls.h"

using namespace V6;

DLLEXPORT HANDLE MySample_CreateFreePainter( UIHandle parent )
{
	auto obj =  new D2DSampleControls(); // auto delete : WM_D2D_DESTROY

	obj->Create(parent, L"hoi",  FRectF(0,0,0,0), 0);

	return nullptr;
}


