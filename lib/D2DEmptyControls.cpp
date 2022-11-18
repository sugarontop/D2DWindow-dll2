 #include "pch.h"
 #include "D2DEmptyControls.h"
 #include "D2D1UI_1.h"
 using namespace V6;

 bool CreateTextFormat( float height, LPCWSTR fontnm, int bold,  IDWriteTextFormat** pfmt )
{
	ComPTR<IDWriteFactory> wfactory;	

	auto weight = (bold==0 ? DWRITE_FONT_WEIGHT_REGULAR : (DWRITE_FONT_WEIGHT)bold);
	
	if ( S_OK == DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &wfactory))
		if ( S_OK == wfactory->CreateTextFormat(fontnm, NULL, weight,
			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, height, LOCALE, pfmt))
			return true;

	return false;
}


void D2DEmptyControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;
	bk_mode_ = 1;

}

static void DrawLattice(ID2D1RenderTarget* pcxt, FSizeF sz, float box)
{
	D2DMatrix mat(pcxt);
	mat.PushTransform();
	ComPTR<ID2D1SolidColorBrush> br;
	(pcxt)->CreateSolidColorBrush(D2RGB(235,235,235), &br);

	int hcnt = sz.height / box * 2;
	int wcnt = sz.width / box * 2;

	FRectF brc(0,0,box,box);
	FRectF brc2 = brc.OffsetRect(box,box);

	for(int i =0; i < hcnt; i++)
	{
		mat.PushTransform();
		for(int j=0; j<wcnt; j++ )
		{
			(pcxt)->FillRectangle(brc, br);
			(pcxt)->FillRectangle(brc2, br);

			mat.Offset(box*2, 0);
		}
		mat.PopTransform();
		mat.Offset(0, box*2);
	}

	mat.PopTransform();
}
void test(D2DContext& cxt);

#define RATTICE 25

bool CreateMemoryView(D2DContext& cxt, FSizeF sz, ID2D1Bitmap** out)
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


			DrawLattice(mem_cxt, sz, RATTICE );
		}
		mem_cxt->EndDraw();

		// 	convert IWICBitmap to ID2D1Bitmap				
		auto hr = (*cxt)->CreateBitmapFromWicBitmap(wicb_bmp, NULL, out );

		return (hr == S_OK);
	}
	return false;
}
void D2DEmptyControls::Draw(D2DContext& cxt)
{
	D2DRectFilter f(cxt, rc_);
	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc_);

	if ( bk_mode_ != 0 )
	{
		if (0 < bk_mode_ && bk_mode_ != 3 )
		{
			cxt.DFillRect(rc_, ColorF::White);

			const FSizeF sz(RATTICE*20,RATTICE*20);
			if ( !LatticeBitmap_ )
				CreateMemoryView( cxt, sz, &LatticeBitmap_);					

			mat.PushTransform();
			for(int ix=0; ix < rc_.Size().width / sz.width; ix++)
			{
				mat.PushTransform();
				for(int iy=0; iy < rc_.Size().height / sz.height; iy++)
				{
					(*cxt)->DrawBitmap(LatticeBitmap_, FRectF(0,0,sz) );
					mat.Offset(0,sz.height);
				}
				mat.PopTransform();

				mat.Offset(sz.width,0);
			}
			mat.PopTransform();

		}
		if (bk_mode_ == 2 || bk_mode_ == 4 )
		{
			WCHAR cb[256];
			StringCbPrintf(cb, (size_t)_countof(cb), L"D2DEmptyControls this=%x\n w=%0.2f,h=%0.2f", this, rc_.Width(), rc_.Height() );

			UIHandle h={};
			h.p = this;

			D2DDrawText(h, FPointF(10,10), cb );


			if ( bk_mode_ == 4 )
			{
				test(cxt);
			}
		}
		else if ( bk_mode_ == 3 )
		{
			ComPTR<IDWriteTextFormat> textformat;
			ComPTR<ID2D1SolidColorBrush> brgray;

			if ( CreateTextFormat( 11.0f, L"Arial", 800,  &textformat ))
			{
				auto s = text_.c_str(); 
				FRectF rctext(0,1,FSizeF(50,20));
				auto clr = D2RGBA(100,100,100,100); 
				(*cxt)->CreateSolidColorBrush(clr, &brgray);
				
				// this is joke
				static int xoff1=-rc_.Width()*2;
				static DWORD last=0;

				if ( last + 80 < ::GetTickCount() )
				{
					xoff1 = ( rc_.Width() < xoff1 ? -rc_.Width()*2 : xoff1+10 );
					last = ::GetTickCount();
				}


				int xoff = xoff1;

				for(int y= 0; y < rc_.Height(); y+=24)
				{
					for(int x= xoff; x < rc_.Width(); x+=50)
						(*cxt)->DrawText(s, wcslen(s), textformat, FRectF(x,y,FSizeF(100,20)), brgray );
					xoff+=2;
				}
				
				cxt.Redraw();
			}

			auto rc = rc_.ZeroRect().Inflate(-2,-2);
			(*cxt)->DrawRectangle(rc, brgray );


			


		}
		
	}

	InnerDraw(cxt);

	mat.PopTransform();
}
LRESULT D2DEmptyControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch(message)
	{
		case WM_D2D_SET_BKMODE:
		{
			bk_mode_ = (int)wParam;
		}
		break;
		case WM_D2D_SET_TEXT:
		{
			text_ = (LPCWSTR)lParam;

		}
		break;
		case WM_D2D_RESOURCES_UPDATE:
		{
			if ( wParam == 0 )
				LatticeBitmap_ = nullptr;
 		}
		break;

	}


	return DefWndProc(b, message, wParam, lParam);
}


static void test(D2DContext& cxt)
{
	auto money = L"1,000,000";

	ComPTR<IDWriteTextFormat> textformat;

	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_REGULAR;

	cxt.tsf_wfactory_->CreateTextFormat(L"Arial", NULL, 
		weight,DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,20, LOCALE, & textformat);

	cxt.SetTextAlign(textformat, 2);

	ComPTR<IDWriteTextLayout> layout;				
	cxt.CreateTextLayout2(money, wcslen(money), FSizeF(500,20),  textformat, &layout);

	(*cxt)->DrawTextLayout(FPointF(0,200), layout, cxt.black_, D2D1_DRAW_TEXT_OPTIONS_NONE);

	(*cxt)->DrawRectangle(FRectF(0,200,FSizeF(500,20)), cxt.black_ );


}