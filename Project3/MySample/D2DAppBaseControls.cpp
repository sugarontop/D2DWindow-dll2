#include "pch.h"
#include "D2DAppBaseControls.h"
#include "D2D1UI_1.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())




bool V6::CreateTextFormat( float height, LPCWSTR fontnm, int bold,  IDWriteTextFormat** pfmt )
{
	ComPTR<IDWriteFactory> wfactory;	

	auto weight = (bold==0 ? DWRITE_FONT_WEIGHT_REGULAR : (DWRITE_FONT_WEIGHT)bold);
	
	if ( S_OK == DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &wfactory))
		if ( S_OK == wfactory->CreateTextFormat(fontnm, NULL, weight,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, LOCALE, pfmt))
			return true;

	return false;
}