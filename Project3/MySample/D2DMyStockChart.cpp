#include "pch.h"
#include "D2D1UI_1.h"
#include "D2DMyStockChart.h"
#include "chart/StockChart.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())

bool D2DMyStockChart::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	//if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(cxt);
		mat.PushTransform();
		mat_ = mat.Offset(rc_);
		{
			ComPTR<ID2D1SolidColorBrush> br, brgray;
			auto clr = ColorF(ColorF::LightGray);

			cxt->CreateSolidColorBrush(clr, &br);
			cxt->FillRectangle(rc_.ZeroRect(), br);


			//br = nullptr;
			//clr = ColorF(ColorF::Black);
			//cxt->CreateSolidColorBrush(clr, &br);
			//cxt->DrawText(str_.c_str(), (UINT32)str_.length(), wformat_, rc_, br );

			
			D2DInnerDraw(hndl_);

			st.Draw(cxt);

		}

		mat.PopTransform();
	}

	return false;
}
LRESULT D2DMyStockChart::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{			
			hndl_ = *(UIHandle*)lParam;						
			rc_ = D2DGetRect(hndl_);

			auto hParent = D2DGetParent(hndl_);

			if ( rc_.IsEmpty())
			{
				
				FRectF rc = D2DGetRect(hParent);
				rc_ = rc.ZeroRect();
			}

			float h = 23.0f;

			D2DCreateStatic(hndl_, FRectF(0,7,FSizeF(55,h)), STAT_DEFAULT, L"CD: ", NONAME);
			auto t1 = D2DCreateTextbox(hndl_, FRectF(63,5,FSizeF(100,h)), false, STAT_DEFAULT, L"cd");
			auto t2 = D2DCreateButton(hndl_, FRectF(170,5,FSizeF(100,h)), STAT_DEFAULT, L"b1", 10);

			D2DCreateStatic(hndl_, FRectF(350,7,FSizeF(55,h)), STAT_DEFAULT, L"INTV: ", NONAME);
			auto t3 = D2DCreateDropdownListbox(hndl_, FRectF(408,5,FSizeF(100,h)),  STAT_DEFAULT, L"intv");


			D2DSetText(t1, L"spy");
			D2DSetText(t2,L"download");

			D2DAddItem(t3, L"1d");
			D2DAddItem(t3, L"1wk");
			D2DSelectItem(t3,0);


			D2DGetDWriteFormat(hndl_, &wformat_);
			

			cd_ = t1;
			intv_ = t3;

			r =1;
		}
		break;
		case WM_D2D_DESTROY:
		{
			delete this;
			r= 1;
		}
		break;
		case WM_NOTIFY:
		{
			if ( wParam == 10 )
			{
				D2DNMHDR* p = (D2DNMHDR*)lParam;

				str_ = L"Click!";

				InetDataProvider pv;
				//FileDataProvider pv;


				auto cd = D2DGetText(cd_); 

				auto intv = D2DGetText(intv_);

				DataProviderInfo dpi;
				dpi.cd = cd;
				dpi.interval = intv;
				

				st.Load(pv, dpi );

				r = 1;
			}

		}
		break;

	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}