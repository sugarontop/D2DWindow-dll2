#include "pch.h"
#include "appbase.h"
#include "D2D1UI_1.h"
#include "FD2DAppBaseControls.h"
#include "FD2DRL.h"

using namespace V6;

#pragma comment (lib, "D2DUI_1")

#define FONTHEIGHT 16

bool FD2DRLeControls::Draw(ID2D1DeviceContext* cxt)
{
	D2DMatrix mat(cxt);
	mat.PushTransform();
	mat_ = mat.Offset(rc_);

	ComPTR<ID2D1SolidColorBrush> br, brw;
	auto clr = ColorF(ColorF::Black);
	cxt->CreateSolidColorBrush(clr, &br);
	clr = ColorF(ColorF::White);
	cxt->CreateSolidColorBrush(clr, &brw);
	cxt->FillRectangle(FRectF(0,0,1000,1000), brw);



	float w = pitRect_.Size().width;
	float h = pitRect_.Size().height;
	int k = 0;

	for(int r=0; r < rowcnt_; r++)
	{
		mat.PushTransform();
		for(int c=0; c < colcnt_; c++)
		{
			cxt->DrawRectangle(pitRect_, br );

			FPointF pt(0,0);

			float* q = Q_ + 4*(rowcnt_*r + c);

			//for(int j=0; j < 4; j++)
			{
				DrawQvalue(cxt,br, pt, 0, q);
			}

			if ( r == 0 && c == 0)
			{
				cxt->DrawText(L"S", 1, textformat2_, FRectF(0,70,200,200), br );
			}
			else if ( r == rowcnt_-1 && c == colcnt_-1)
			{
				cxt->DrawText(L"G", 1, textformat2_, FRectF(0,70,200,200), br );
			}


			mat.Offset(w,0);
		}
		mat.PopTransform();
		mat.Offset(0,h);
	}
	mat.PopTransform();


	if ( bRedraw_ )
		D2DRedraw(hndl_);

	return false;
}
LRESULT FD2DRLeControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{	
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:
		{	
			rowcnt_ = colcnt_ = 0;
			hndl_ = *(UIHandle*)lParam;			
			
			ComPTR<IDWriteFactory> dwriteFactory;	

			if ( S_OK == (DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &dwriteFactory)))
			{
				if (S_OK ==(dwriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,FONTHEIGHT, LOCALE, &textformat_)))
					{
						r = 1;
					}
				if (S_OK ==(dwriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,FONTHEIGHT*2, LOCALE, &textformat2_)))
					{
						r = 1;
					}
			}			

			bRedraw_ = false;
			r = 1;
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
			INT_PTR id = (INT_PTR)(wParam);
			if ( id == 100 || id == 101 || id == 200)
			{
				if ( Eventhandler_ )
				{
					bRedraw_ = true;

					D2DNMHDR* d = (D2DNMHDR*)lParam;

					UIHandle h = d->sender;

					Eventhandler_(h, (INT_PTR)id);
				}
				r = 1;

			}


		}
		break;

		case WM_D2D_USER_FIRST+500:
		{
			bRedraw_ = false;
			r = 1;
		}
		break;

	}

	if ( r == 0 )
		r = D2DDefControlProc(hndl_,b,message,wParam,lParam);

	return r;
}

void FD2DRLeControls::DrawQvalue(ID2D1DeviceContext* cxt,ID2D1SolidColorBrush* br, FPointF pt, int s, float* Q4)
{
	FRectF rc(pt, FSizeF(200,26));

	WCHAR cb[256];

	WCHAR cc[] = {L'左',L'上',L'右',L'下'};
	WCHAR cc2[] = {L'☜',L'☝',L'☞',L'☟'};

	float fmax = -1;
	for(int i= 0; i <4; i++)
	{
		fmax = max(fmax, Q4[i]);
	}

	if ( fmax > 0 )
	{
		for(int i= 0; i <4; i++)
		{
			float xxx = Q4[i];

		
			if ( fmax == xxx )
				StringCbPrintf(cb,_countof(cb),L"%c:%f", cc2[i], Q4[i]);	
			else
				StringCbPrintf(cb,_countof(cb),L"%c:%f", cc[i], Q4[i]);	

			cxt->DrawText(cb, wcslen(cb), textformat_, rc, br );


			rc.Offset(0,FONTHEIGHT);
		}
	}
	else
	{
		LPCWSTR str = L"HOLE";
		cxt->DrawText(str, wcslen(str), textformat_, rc, br );
		

	}
}

void FD2DRLeControls::SetPitRect( FSizeF rcSize,float* Q, int rowcnt, int colcnt)
{
	pitRect_ = FRectF(0,0,rcSize);
	Q_ = Q;
	rowcnt_ = rowcnt;
	colcnt_ = colcnt;

}