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
		
			D2DInnerDraw(hndl_);

			stock_chart_.Draw(cxt);

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

			this->stock_chart_.SetSize(rc_.Size());

			auto hParent = D2DGetParent(hndl_);

			//if ( rc.IsEmpty())
			//{
			//	
			//	FRectF rc = D2DGetRect(hParent);
			//	//rc_ = rc.ZeroRect();
			//}

			float h = 23.0f;

			D2DCreateStatic(hndl_, FRectF(0,7,FSizeF(55,h)), STAT_DEFAULT, L"CD: ", NONAME);
			auto t1 = D2DCreateTextbox(hndl_, FRectF(63,5,FSizeF(100,h)), false, STAT_DEFAULT, L"cd");
			auto t2 = D2DCreateButton(hndl_, FRectF(170,5,FSizeF(100,h)), STAT_DEFAULT, L"b1", 11);

			D2DCreateStatic(hndl_, FRectF(350,7,FSizeF(55,h)), STAT_DEFAULT, L"INTV: ", NONAME);
			auto t3 = D2DCreateDropdownListbox(hndl_, FRectF(408,5,FSizeF(100,h)),  STAT_DEFAULT, L"intv");


			D2DSetText(t1, L"spy");
			D2DSetText(t2,L"download");

			D2DAddItem(t3, L"1d");
			D2DAddItem(t3, L"1wk");
			D2DSelectItem(t3,0);


			D2DGetDWriteFormat(hndl_, &wformat_);
			

			ComPTR<IDWriteFactory> wf;
			ComPTR<IDWriteTextFormat> wtextformat;

			if ( S_OK == (DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &wf)))
			{
				
				if (S_OK ==(wf->CreateTextFormat(L"MS –¾’©", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,10.0f, LOCALE, &wtextformat)))
					{
						stock_chart_.trim_textformat_ = wtextformat;

					}	
				
				wtextformat = nullptr;

				if (S_OK ==(wf->CreateTextFormat(L"MS –¾’©", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,20.0f, LOCALE, &wtextformat)))
					{
						stock_chart_.money_textformat_ = wtextformat;

					}		
			}


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
			if ( wParam == 110 )
			{
				D2DNMHDR* p = (D2DNMHDR*)lParam;

				FileDataProvider pv; // test—p


				auto cd = D2DGetText(cd_); 

				auto intv = D2DGetText(intv_);

				DataProviderInfo dpi;
				dpi.cd = cd;
				dpi.interval = intv;
				

				stock_chart_.Load(pv, dpi );

				r = 1;
			}
			else if ( wParam == 11 )
			{
				D2DNMHDR* p = (D2DNMHDR*)lParam;

				InetDataProvider* pv = new InetDataProvider();

				auto cd = D2DGetText(cd_); 
				auto intv = D2DGetText(intv_);

				DataProviderInfo* dpi = new DataProviderInfo();
				dpi->cd = cd;
				dpi->interval = intv;

				auto complete = [b]()
				{
					b.Redraw();
				};

				stock_chart_.LoadAsync(pv, dpi, complete);

				r = 1;
			}

		}
		break;
		case WM_MOUSEMOVE:
		{
			MouseParam* mp = (MouseParam*)lParam;
			auto pt = mat_.DPtoLP(mp->pt);

			if ( rc_.ZeroPtInRect(pt))
			{
				stock_chart_.MouseMove(pt);
				b.Redraw();
			}

		}
		break;
		case WM_D2D_APP_SETDEFAULT_CD :
		{
			if ( (UINT_PTR)wParam == (UINT_PTR)this )
			{
				LPCWSTR cd = (LPCWSTR)lParam;

				D2DSetText(cd_, cd);

				r = 1;
			}

		};
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}