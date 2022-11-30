#include "pch.h"
#include "D2D1UI_1.h"
#include "FD2DMyStockChart.h"
#include "chart/StockChart.h"
using namespace V6;
#define  APP (D2DApp::GetInstance())



class ShortBSTR
{
	public :
		ShortBSTR():bstr_(nullptr){};
		ShortBSTR(BSTR bs):bstr_(bs){};
		~ShortBSTR(){free();}

		void free()
		{
			if ( bstr_ )
				::SysFreeString(bstr_);
			bstr_ = nullptr;
		}

		std::wstring Str(){ return std::wstring(bstr_); }
		std::wstring UpperStr()
		{ 
			for(int i=0; i< (int)::SysStringLen(bstr_); i++)
			{
				if ( 'a' <= bstr_[i] && bstr_[i] <='z' )
					bstr_[i] -= ('a'-'A');
			}			
			return std::wstring(bstr_); 
		}

		ShortBSTR& operator = (BSTR bs)
		{
			free();
			bstr_ = bs;
			return *this;
		}
	protected :
		BSTR bstr_;


};


// FD2DMyStockChart //////////////////////////////////////////////////////////////////////////
bool FD2DMyStockChart::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	if ( BITFLG2( stat, STAT_VISIBLE))
	{
		D2DMatrix mat(cxt);

		mat.PushTransform();
		mat_ = mat.Offset(rc_);

		ComPTR<ID2D1SolidColorBrush> br, brgray;
		auto clr = ColorF(ColorF::LightGray);

		cxt->CreateSolidColorBrush(clr, &br);
		cxt->FillRectangle(rc_.ZeroRect(), br);
			
		if ( view_ )
			cxt->DrawBitmap(view_,rc_.ZeroRect());
		else
		{
			if ( CreateMemoryView( cxt, rc_.Size(), &view_))
				cxt->DrawBitmap(view_,rc_.ZeroRect());
			else			
				InnerDraw(cxt);
		}

		// button etc..
		D2DInnerDraw(hndl_);
	
		if ( stock_chart_->IsEmpty())
		{
			DrawPrimeChart(cxt);

		}
		else
			shots_.clear();

		mat.PopTransform();	
	}

	return false;
}
void FD2DMyStockChart::InnerDraw(ID2D1RenderTarget* cxt)
{
	stock_chart_->Draw(cxt);
}
void FD2DMyStockChart::DrawPrimeChart(ID2D1DeviceContext* cxt)
{
	FRectF rc(10,100, FSizeF(200,30));

	ComPTR<IDWriteTextFormat> textformat;

	D2DGetTextFormat(hndl_, &textformat);

	ComPTR<ID2D1SolidColorBrush> br, brgray;
	auto clr = ColorF(ColorF::Black);
	cxt->CreateSolidColorBrush(clr, &br);


	for(auto& it : prime_ )
	{
		WCHAR cb[256];
		StringCbPrintf(cb,256,L"%s %-8.1f", it.second.cd.c_str(), it.second.regularMarketPrice);
		
		cxt->DrawText(cb,(int)wcslen(cb), textformat, rc, br);

		rc.Offset(0,30);
	}

	int i=0;
	FRectF rcs[]={ FRectF(600,100,FSizeF(500,300)),FRectF(1200,100,FSizeF(500,300)),FRectF(600,500,FSizeF(500,300)),FRectF(1200,500,FSizeF(500,300))};
	if ( shots_.empty() )
	{		
		for(auto& it : prime_ )
		{			
			if ( i++ > 4 )
				break;

			auto cd = it.first;
			
			auto shot = std::make_shared<ChartShot>(stock_chart_.get()); 
			shot->Load(cd, hndl_);

			shots_.push_back(shot);
			
		}
	}

	i=0;
	for(auto& it : shots_)
	{
		FRectF rc = rcs[i++];
		it->Draw(rc, cxt);
	}


}
// --------------------------------------------------
bool FD2DMyStockChart::CreateMemoryView(ID2D1RenderTarget* target, FSizeF sz, ID2D1Bitmap** pview)
{	
	ComPTR<IWICBitmap> wicb_bmp;
	ComPTR<ID2D1RenderTarget> mem_cxt = D2DCreateSecondRenderTarget((UINT)sz.width,(UINT)sz.height, &wicb_bmp);

	if ( mem_cxt )
	{
		mem_cxt->BeginDraw();
		{
			D2D1_MATRIX_3X2_F mat = {};
			mat._11 = mat._22 = 1.0f; 
			mem_cxt->SetTransform(mat);
			mem_cxt->Clear(D2D1::ColorF(D2D1::ColorF::White));


			InnerDraw(mem_cxt);
		}
		mem_cxt->EndDraw();

		// 	convert IWICBitmap to ID2D1Bitmap
		auto hr = target->CreateBitmapFromWicBitmap(wicb_bmp, NULL, pview );

		// convert IWICBitmap to stream
		// WICBitmapSourceToStream(wicb_bmp, &sm_);

		return (S_OK==hr);
	}
	return false;
}

LRESULT FD2DMyStockChart::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{			
			hndl_ = *(UIHandle*)lParam;						
			FRectF rc = D2DGetRect(hndl_);


			rc_ = rc.ZeroRect();


			FSizeF sz = rc_.Size().Inflate(0,-TOPBAR_HEIGHT);

			

			auto hParent = D2DGetParent(hndl_);

			float h = 23.0f;

			D2DCreateStatic(hndl_, FRectF(0,7,FSizeF(55,h)), STAT_DEFAULT, L"CD: ", NONAME);
			auto t1 = D2DCreateTextbox(hndl_, FRectF(63,5,FSizeF(100,h)), false, STAT_DEFAULT, L"cd");
			auto t2 = D2DCreateButton(hndl_, FRectF(170,5,FSizeF(100,h)), STAT_DEFAULT, L"b1", 11);

			D2DCreateStatic(hndl_, FRectF(350,7,FSizeF(55,h)), STAT_DEFAULT, L"INTV: ", NONAME);
			auto t3 = D2DCreateDropdownListbox(hndl_, FRectF(408,5,FSizeF(100,h)),  STAT_DEFAULT, L"intv");


			D2DCreateStatic(hndl_, FRectF(520,7,FSizeF(155,h)), STAT_DEFAULT, L"DATE-VALUE: ", NONAME);
			now_value_ = D2DCreateTextbox(hndl_, FRectF(680,5,FSizeF(200,h)), false, STAT_DEFAULT, L"now");
			D2DReadOnly(now_value_, true);

			D2DSetText(t1, L"SPY");
			D2DSetText(t2,L"download");

			D2DAddItem(t3, L"1d");
			D2DAddItem(t3, L"1wk");
			D2DSelectItem(t3,0);


			D2DGetDWriteFormat(hndl_, &wformat_);
			

			ComPTR<IDWriteFactory> wf;
			ComPTR<IDWriteTextFormat> wtextformat;

			
			// CreateStockChart
			stock_chart_ = std::make_unique<StockChart>(FRectF(0,TOPBAR_HEIGHT,sz));

			if ( S_OK == (DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &wf)))
			{
				
				if (S_OK ==(wf->CreateTextFormat(L"MS –¾’©", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,10.0f, LOCALE, &wtextformat)))
					{
						stock_chart_->trim_textformat_ = wtextformat;

					}	
				
				wtextformat = nullptr;

				if (S_OK ==(wf->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,30.0f, LOCALE, &wtextformat)))
					{
						stock_chart_->money_textformat_ = wtextformat;

					}		
			}




			
			txtCD_ = t1;
			txtIntv_ = t3;



			/// 
			/*PrimeStockDataItem item = {};
			prime_[L"SPY"] = item;
			prime_[L"QQQ"] = item;
			prime_[L"VTI"] = item;
			prime_[L"XLE"] = item;
			
			PrimeStockDataLoad(prime_);*/


			r =1;
		}
		break;
		case WM_D2D_ON_DESTROY:
		{
			delete this;
			r= 1;
		}
		break;
		case WM_NOTIFY:
		{
			if ( wParam == 110 ) // test—p
			{
				D2DNMHDR* p = (D2DNMHDR*)lParam;

				FileDataProvider pv;


				ShortBSTR cd = D2DGetText(txtCD_); 

				ShortBSTR intv = D2DGetText(txtIntv_);

				DataProviderInfo dpi;
				dpi.cd = cd.UpperStr();
				dpi.interval = intv.Str();
				

				stock_chart_->Load(pv, dpi );

				D2DTabSendMessage(this->hndl_, WM_D2D_APP_ON_CHART_CHANGED, 0, (LPARAM)this);
				view_ = nullptr;

				r = 1;
			}
			else if ( wParam == 11 )
			{
				D2DNMHDR* p = (D2DNMHDR*)lParam;

				InetDataProvider* pv = new InetDataProvider();

				ShortBSTR cd = D2DGetText(txtCD_); 
				ShortBSTR intv = D2DGetText(txtIntv_);

				

				DataProviderInfo* dpi = new DataProviderInfo();
				dpi->cd = cd.UpperStr();
				dpi->interval = intv.Str();

				auto complete = [b, this]()
				{
					D2DTabSendMessage(this->hndl_, WM_D2D_APP_ON_CHART_CHANGED, 0, (LPARAM)this);
					
					view_ = nullptr;

					b.Redraw();


				};

				stock_chart_->LoadAsync(pv, dpi, complete);

				

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
				if ( stock_chart_->MouseMove(pt))
				{
					money m4;
					LPCWSTR date = stock_chart_->GetNowValue(&m4);


					WCHAR cb[256];
					StringCbPrintf(cb,256,L"%s - %-8.1f", date, m4);

					D2DSetText(now_value_, cb);

					b.Redraw();
				}
			}

		}
		break;
		case WM_D2D_APP_SETDEFAULT_CD :
		{
			if ( (UINT_PTR)wParam == (UINT_PTR)this )
			{
				LPCWSTR cd = (LPCWSTR)lParam;

				D2DSetText(txtCD_, cd);

				r = 1;
			}

		};
		break;
		case WM_KEYDOWN:
		{
			// wparam : VK_XXXX...
			// lparam : 

			if ( wParam == 0x41) // A key
			{
				ShowDialog(0, FRectF(100,100,FSizeF(500,300)));
				r = 1;
			}
			else if ( wParam == 0x42) // B key
			{
				ShowDialog(1, FRectF(100,100,FSizeF(300,500)));
				r = 1;
			}

		}
		break;
		case WM_D2D_APP_SET_CD:
		{
			LPCWSTR cd = (LPCWSTR)lParam;
			
			D2DSetText(txtCD_, cd); 

			r = 1;
		}
		break;
		case WM_D2D_RESOURCES_UPDATE:
		{
			if ( wParam == 0 )
				view_ = nullptr;
 		}
		break;

		case WM_CHAR:
		{
			int a =0;
			r = 1;
		}
		break;
		case WM_KEYUP:
		{
			r = 1;
		}
		break;		
		case WM_D2D_SET_SIZE:
		{
			/*int a = 0;

			FSizeF sz( rc_.Size().width, 800 );

			auto a1 = D2DGetParent(hndl_);
			
			D2DSendMessage(a1, WM_D2D_SET_SIZE_FROM_OUTER,0,(LPARAM)&sz);

			rc_.SetSize(sz);
			stock_chart_->SetSize(sz);
			view_ = nullptr;*/
		}
		break;
		case WM_D2D_SET_SIZE_FROM_OUTER:
		{
			FSizeF sz = *(FSizeF*)lParam;
			
			
			rc_.SetSize(sz);
			
			
			
			stock_chart_->SetSize(sz);

			view_ = nullptr;

		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}




// D2DMyStockDataView //////////////////////////////////////////////////////////////////////////

bool FD2DMyStockDataView::Draw(ID2D1DeviceContext* cxt)
{
	auto stat = D2DGetStat(Get());
	if ( BITFLG2( stat, STAT_VISIBLE))
	{	
		D2DMatrix mat(cxt);

		mat.PushTransform();
		mat_ = mat.Offset(rc_);

		ComPTR<ID2D1SolidColorBrush> br;
		auto clr = ColorF(ColorF::AliceBlue);
		cxt->CreateSolidColorBrush(clr, &br);
		cxt->FillRectangle(rc_.ZeroRect(), br);


		D2DInnerDraw(hndl_);

		mat.PopTransform();
	}
	return false;
}
LRESULT FD2DMyStockDataView::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{			
			hndl_ = *(UIHandle*)lParam;						
			FRectF rc = D2DGetRect(hndl_);
			rc_ = rc.ZeroRect();

			hgridview_ = D2DCreateDataGridView(hndl_, rc_, STAT_DEFAULT, NONAME);

			D2DDefControlProc(hndl_,b, WM_D2D_CREATE,  (WPARAM)0 ,(LPARAM)&hgridview_);
			r = 1;

		}
		break;
		case WM_D2D_ON_DESTROY:
		{
			delete this;
			r= 1;
		}
		break;
		case WM_D2D_APP_ON_CHART_CHANGED:
		{
			FD2DMyStockChart* chart = (FD2DMyStockChart*)lParam;

			StockChart& ch = *(chart->stock_chart_);

			auto cd = ch.cd_;

			auto ar = ch.xar_;


			auto conv = [](int row, int col, std::wstring s)->std::wstring{
			
				_ASSERT(row>0 && col>0 && col < 26);
				WCHAR colnm = L'A';

				WCHAR cb[64];
				StringCbPrintf(cb,_countof(cb),L"%c%d=%s&", (WCHAR)(colnm+col-1), row, s.c_str());
				return cb;
			};
			
			auto iconv = [](int row, int col, int d)->std::wstring{
			
				_ASSERT(row>0 && col>0 && col < 26);
				WCHAR colnm = L'A';

				WCHAR cb[64];
				StringCbPrintf(cb,_countof(cb),L"%c%d=%d&", (WCHAR)(colnm+col-1), row, d);
				return cb;
			};

			auto fconv = [](int row, int col, float d)->std::wstring{
			
				_ASSERT(row>0 && col>0 && col < 26);
				WCHAR colnm = L'A';

				WCHAR cb[64];
				StringCbPrintf(cb,_countof(cb),L"%c%d=%f&", (WCHAR)(colnm+col-1), row, d);
				return cb;
			};

			auto A2W = [](LPCSTR a)->std::wstring {
				int wlen = ::MultiByteToWideChar(CP_ACP,0,a,lstrlenA(a),nullptr,0);
				std::unique_ptr<WCHAR[]> wcb(new WCHAR[wlen]);
				::MultiByteToWideChar(CP_ACP,0,a,lstrlenA(a), wcb.get(), wlen);
				return std::wstring(wcb.get(),wlen);	
			};

			int row = 1;
			for(auto& it : ch.xar_)
			{
				std::wstring cmd;

				cmd  = conv(row, 1, cd );
				cmd += conv(row, 2, A2W(it.raw.date) );
				cmd += fconv(row, 3, it.raw.m1 );
				cmd += fconv(row, 4, it.raw.m2 );
				cmd += fconv(row, 5, it.raw.m3 );
				cmd += fconv(row, 6, it.raw.m4 );
				cmd += iconv(row, 7, it.raw.qnt );
				
				D2DSendMessage(hgridview_,WM_D2D_SET_GRIDVIEW_VALUE,(WPARAM)0,(LPARAM)cmd.c_str()); 

				row++;
			}

			D2DSendMessage(hgridview_,WM_D2D_SET_GRIDVIEW_VALUE,(WPARAM)1,(LPARAM)0); 

			r = 1;

		}
		break;
		case WM_D2D_SET_SIZE_FROM_OUTER:
		{
			FSizeF sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

			D2DSetRect(hgridview_, rc_);
		}
		break;
	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}

//////////////////////////////////////////////////////////////////////////////////

static const WCHAR* gcds_[]={L"SPY",L"QQQ",L"VTI",L"SPYG",L"SPYV",L"XLE",L"ARKK"};

class FD2DDialogTest: public FD2DAppBaseControls
{
	public :

		FD2DDialogTest(int typ):ret_(0),typ_(typ)
		{
			// "rc_" is setted in WM_D2D_CREATE
		}
		~FD2DDialogTest()
		{
			int a = 0;
		}

		virtual bool Draw(ID2D1DeviceContext* cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		FRectF rc_;
		std::wstring str_;
		D2DMat mat_;
		int ret_;
		int typ_;

	protected :
		bool Draw0(ID2D1DeviceContext* cxt);
		LRESULT WndProc0(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
		bool Draw1(ID2D1DeviceContext* cxt);
		LRESULT WndProc1(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam);
};

bool FD2DDialogTest::Draw(ID2D1DeviceContext* cxt)
{
	if ( typ_ == 1 )
		return Draw1(cxt);
		
	return Draw0(cxt);
}
LRESULT FD2DDialogTest::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( typ_ == 1 )
		return WndProc1(b,message,wParam,lParam);

	return WndProc0(b,message,wParam,lParam);
}


#pragma region TYP0

bool FD2DDialogTest::Draw0(ID2D1DeviceContext* cxt)
{
	
	//if (BITFLG(STAT_VISIBLE))
    {		
		ComPTR<ID2D1SolidColorBrush> back, back2;
		ComPTR<ID2D1SolidColorBrush> fore;

		(cxt)->CreateSolidColorBrush(D2RGB(0,0,0), &fore);	
		(cxt)->CreateSolidColorBrush(D2RGB(180,150,165), &back);
		(cxt)->CreateSolidColorBrush(D2RGB(200,200,200), &back2);

       // D2DRectFilter fil(cxt, rc_ );


	   FRectF rc(rc_);
		(cxt)->FillRectangle(rc,back);
		(cxt)->FillRectangle(rc.InflateRect(-3,-3),back2);

		D2DMatrix mat(cxt);
        mat_ = mat.PushTransform();

       
        mat.Offset(rc_);
	
		ComPTR<IDWriteTextFormat> textformat;

		D2DGetTextFormat(hndl_, &textformat);
		std::wstring s = L"Dialog control TEST";
		(cxt)->DrawText(s.c_str(),(UINT32)s.length(), textformat, FRectF(0,0,200,100), fore);


		D2DInnerDraw(hndl_);
		

		mat.PopTransform();
	}

	return false;
}

LRESULT FD2DDialogTest::WndProc0(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r=0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{	
			hndl_ = *(UIHandle*)lParam;	

			D2DSetCapture(hndl_); // <-- Dialog

			rc_ = D2DGetRect(hndl_);

			D2DCreateTextbox(hndl_, FRectF(100,100,FSizeF(200,26)), false, STAT_DEFAULT, L"test1");
			D2DCreateTextbox(hndl_, FRectF(100,150,FSizeF(200,26)), false, STAT_DEFAULT, L"test2");
			D2DCreateButton(hndl_, FRectF(100,200,FSizeF(100,26)), STAT_DEFAULT, L"OK", 102);
			D2DCreateButton(hndl_, FRectF(220,200,FSizeF(100,26)), STAT_DEFAULT, L"Cancel", 101);

			r = 1;
		}
		break;
		case WM_KEYDOWN:
		{
			if ( wParam == VK_ESCAPE )
			{
				D2DReleaseCapture();
				D2DDestroyControl(hndl_);
				ret_ = -1;
				r = 1;
			}
		}
		break;
		case WM_NOTIFY:
		{
			if ( wParam == 101 )
			{
				r = 1;

				D2DReleaseCapture();
				D2DDestroyControl(hndl_);

				ret_ = -1;
			}
			else if ( wParam == 102 )
			{
				r = 1;

				D2DReleaseCapture();
				D2DDestroyControl(hndl_);
				
				ret_ = 1;
			}
		}
		break;
	}

	return r;
}
#pragma endregion

#pragma region TYP1
bool FD2DDialogTest::Draw1(ID2D1DeviceContext* cxt)
{
    {		
		ComPTR<ID2D1SolidColorBrush> back, back2;
		ComPTR<ID2D1SolidColorBrush> fore;

		(cxt)->CreateSolidColorBrush(D2RGB(0,0,0), &fore);	
		(cxt)->CreateSolidColorBrush(D2RGB(180,150,165), &back);
		(cxt)->CreateSolidColorBrush(D2RGB(200,200,200), &back2);

	   FRectF rc(rc_);
		(cxt)->FillRectangle(rc,back);
		(cxt)->FillRectangle(rc.InflateRect(-3,-3),back2);

		D2DMatrix mat(cxt);
        mat_ = mat.PushTransform();
 
        mat.Offset(rc_);
		
	
		/*ComPTR<IDWriteTextFormat> textformat;
		D2DGetTextFormat(hndl_, &textformat);
		std::wstring s = L"Dialog control TEST";
		(cxt)->DrawText(s.c_str(),(UINT32)s.length(), textformat, FRectF(0,0,200,100), fore);*/


		D2DInnerDraw(hndl_);
		

		mat.PopTransform();
	}

	return false;
}

LRESULT FD2DDialogTest::WndProc1(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r=0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{	
			hndl_ = *(UIHandle*)lParam;	

			D2DSetCapture(hndl_); // <-- Dialog

			rc_ = D2DGetRect(hndl_);


			auto hls = D2DCreateListbox(hndl_,FRectF(10,10,FSizeF(200,450)), STAT_DEFAULT, NONAME,201);

			for(int i = 0; i <_countof(gcds_); i++)
				D2DAddItem(hls, gcds_[i]);

			D2DCreateButton(hndl_, FRectF(10,470,FSizeF(100,26)), STAT_DEFAULT, L"Cancel", 101);
			r = 1;
		}
		break;
		case WM_NOTIFY:
		{
			if ( wParam == 101 )
			{
				r = 1;

				D2DReleaseCapture();
				D2DDestroyControl(hndl_);

				ret_ = -1;
			}
			else if ( wParam == 102 )
			{
				r = 1;

				D2DReleaseCapture();
				D2DDestroyControl(hndl_);
				
				ret_ = 1;
			}
			else if ( wParam == 201 )
			{
				r = 1;
				D2DNMHDR& nmh = *(D2DNMHDR*)lParam;

				if ( nmh.code == EVENTID_SELECTCHANGED )
				{
					int idx = nmh.prm1;

					LPCWSTR cd = gcds_[idx];



					D2DSendMessage(D2DGetParent(hndl_), WM_D2D_APP_SET_CD,0, (LPARAM)cd);



				}

				
			}
		}
		break;
		case WM_KEYDOWN:
		{
			if ( wParam == VK_ESCAPE )
			{
				D2DReleaseCapture();
				D2DDestroyControl(hndl_);
				ret_ = -1;
				r = 1;
			}
		}
		break;
	}
	return r;
}

#pragma endregion

void FD2DMyStockChart::ShowDialog(int typ, FRectF rc)
{
	dlg_ = std::make_shared<FD2DDialogTest>(typ);
	dlg_->Create(hndl_,NONAME, rc, 0);
}



ChartShot::ChartShot(const StockChart* org)
{
	stock_chart_ = org->Clone();
}
void ChartShot::Load(std::wstring cd,UIHandle hdl)
{
	InetDataProvider* pdp = new InetDataProvider();
	DataProviderInfo* pdpi = new DataProviderInfo();

	pdpi->cd = cd;
	pdpi->info = nullptr;
	pdpi->interval = L"1d";


	auto comp = [hdl](){
		
		D2DRedraw(hdl);

	};
	
	stock_chart_->LoadAsync(pdp, pdpi, comp);

}

void ChartShot::Draw(FRectF rc, ID2D1RenderTarget* cxt)
{
	D2DMatrix mat(cxt);
    mat.PushTransform();       
    mat.Offset(rc);

	stock_chart_->SetSize(rc.Size());

	stock_chart_->Draw(cxt);

	mat.PopTransform();
}