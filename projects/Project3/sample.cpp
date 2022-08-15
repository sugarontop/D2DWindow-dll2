#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"

#include "sample.h"


using namespace V6;


bool D2DSampleControls::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	//if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(cxt);
		mat.PushTransform();
		mat.Offset(rc_);
		{

			ComPTR<ID2D1SolidColorBrush> br, brpink;
			auto clr = ColorF(ColorF::Black);

			cxt->CreateSolidColorBrush(clr, &br);

			cxt->DrawRectangle(rc_.ZeroRect(), br);

			clr = ColorF(ColorF::LightPink);
			cxt->CreateSolidColorBrush(clr, &brpink);
			cxt->FillRectangle(rc_.ZeroRect(), brpink);


			ComPTR<IDWriteTextFormat> format;

			CreateTextFormat( 20, L"MS–¾’©", 0, &format);

			cxt->DrawText( str_.c_str(), str_.length(), format, rc_.ZeroRect(), br );

			D2DDefaultDraw(hndl_);
		}

		mat.PopTransform();
	}

	return false;



}
LRESULT D2DSampleControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{
			hndl_ = *(UIHandle*)lParam;						
			rc_ = D2DGetRect(hndl_);

			str_ = L"init";

			auto tx = D2DCreateTextbox(hndl_, FRectF(10,100,FSizeF(200,60)),true, STAT_DEFAULT,  L"TX1" );

			r =1;
		}
		break;
		case WM_D2D_DESTROY:
		{
			delete this;
			r= 1;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			str_ = L"click!!";

		}
		break;

	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}

void test(UIHandle parent, LPCWSTR name, FRectF rc, int id)
{
	auto obj =  new D2DSampleControls();
	
	obj->Create(parent, name,  rc, id);

}