#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "FD2DAppBaseControls.h"
#include "FD2DMyControls.h"

using namespace V6;

#pragma comment (lib, "D2DUI_1")

bool FD2DSampleControls::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	//if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(cxt);
		mat.PushTransform();
		mat_ = mat.Offset(rc_);
		{
			FPointF ptold;
			ComPTR<ID2D1SolidColorBrush> br, brgray;
			auto clr = ColorF(ColorF::Black);

			cxt->CreateSolidColorBrush(clr, &br);

			int k=0, last_idx=0;
			for(auto& d : ar_)
			{
				if ( last_idx != d.idx )
				{
					ptold = d.pt;
					last_idx = d.idx;
					k=0;
				}

				if ( 0 < k++ )
				{
					cxt->DrawLine( d.pt, ptold, br );

					last_idx = d.idx;
				}



				ptold = d.pt;
			}



			
			ComPTR<IDWriteTextFormat> pfmt;
			
			if (CreateTextFormat( 80.0f, L"Arial", 800,  &pfmt ))
			{
				auto s = L"FreePaint";
				FRectF rctext(50,100,FSizeF(500,500));
				clr = D2RGBA(100,100,100,100); 
				cxt->CreateSolidColorBrush(clr, &brgray);
				cxt->DrawText(s, (UINT32)wcslen(s), pfmt, rctext, brgray );
			}

			cxt->DrawRectangle(rc_.ZeroRect(), br );

		}

		mat.PopTransform();
	}

	return false;



}
LRESULT FD2DSampleControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
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

			dot_idx_ = 0;
			

			r =1;
		}
		break;
		case WM_D2D_ON_DESTROY:
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

				
				dot d;
				d.idx = ++dot_idx_;
				d.pt = pt;

				ar_.push_back(d);

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

				dot d;
				d.idx = dot_idx_;
				d.pt = pt;

				ar_.push_back(d);


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
		case WM_RBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp->pt);

			if ( rc_.ZeroPtInRect(pt))
			{			
				ar_.clear();
				dot_idx_ = 0;
				D2DReleaseCapture();
				r = 1;
			}
		}
		break;
		case WM_D2D_SET_SIZE_FROM_CHILDWINDOW:
		{
			int a = 0;

			FSizeF& sz = *(FSizeF*)lParam;


		}
		break;

	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}