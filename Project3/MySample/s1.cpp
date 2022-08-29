#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "D2DAppBaseControls.h"
#include "MyControls.h"

using namespace V6;

DLLEXPORT HANDLE CreateFreePainter( UIHandle parent )
{
	
	auto obj =  new D2DSampleControls();
	

	FRectF rc(0,0,0,0);

	obj->Create(parent, L"hoi",  rc, 0);






	return nullptr;

}


