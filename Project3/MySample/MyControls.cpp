
#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "D2DAppBaseControls.h"
#include "MyControls.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

#pragma comment (lib, "D2DUI_1")

static bool gef1(LPVOID captureobj, ID2D1DeviceContext* cxt)
{
	D2DAppBaseControls* p = (D2DAppBaseControls*)captureobj;
	return p->Draw(cxt);
}
static LRESULT gef2(LPVOID captureobj, AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	D2DAppBaseControls* p = (D2DAppBaseControls*)captureobj;
	return p->WndProc(b, message, wParam, lParam);
}

void D2DAppBaseControls::Create(UIHandle parent, LPCWSTR name, FRectF rc, int id)
{
	hndl_ = D2DCreateWhiteControls((LPVOID)this, gef1,gef2, parent, rc, STAT_DEFAULT, name, id);
}


bool D2DSampleControls::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	//if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(cxt);
		mat.PushTransform();
		mat_ = mat.Offset(rc_);
		{
			FPointF ptold;
			ComPTR<ID2D1SolidColorBrush> br, brpink;
			auto clr = ColorF(ColorF::Black);

			cxt->CreateSolidColorBrush(clr, &br);

			int k=0;
			for(auto& pt : ar_)
			{
				if ( 0 < k++ )
					cxt->DrawLine( pt, ptold, br );
				ptold = pt;
			}

			cxt->DrawRectangle(rc_.ZeroRect(), br );

			/*ComPTR<ID2D1SolidColorBrush> br, brpink;
			auto clr = ColorF(ColorF::Black);

			cxt->CreateSolidColorBrush(clr, &br);

			cxt->DrawRectangle(rc_.ZeroRect(), br);

			clr = ColorF(ColorF::LightPink);
			cxt->CreateSolidColorBrush(clr, &brpink);
			cxt->FillRectangle(rc_.ZeroRect(), brpink);


			ComPTR<IDWriteTextFormat> format;

			CreateTextFormat( 20, L"MS–¾’©", 0, &format);

			cxt->DrawText( str_.c_str(), str_.length(), format, rc_.ZeroRect(), br );

			D2DDefaultDraw(hndl_);*/
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

			auto h1 = D2DGetParent(hndl_);

			FRectF rc = D2DGetRect(h1);

			rc_ = rc.ZeroRect();


			

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
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp->pt);

			if ( rc_.ZeroPtInRect(pt))
			{
				D2DSetCapture(hndl_);

				ar_.clear();

				//pt.x -= rc_.left;
				//pt.y -= rc_.top;

				ar_.push_back(pt);

				r = 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			UIHandle h2 = D2DGetCapture();
			if ( h2.p == hndl_.p)
			{

				MouseParam* mp = (MouseParam*)lParam;
				auto pt = mat_.DPtoLP(mp->pt);

				//pt.x -= rc_.left;
				//pt.y -= rc_.top;

				ar_.push_back(pt);


				D2DRedraw(hndl_);

				r = 1;
			}

		}
		break;
		case WM_LBUTTONUP:
		{
			UIHandle h2 = D2DGetCapture();

			if ( h2.p == hndl_.p)
			{
				D2DReleaseCapture();
				r = 1;
			}

		}
		break;

	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}