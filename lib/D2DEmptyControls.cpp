 #include "pch.h"
 #include "D2DEmptyControls.h"
 #include "D2D1UI_1.h"
 using namespace V6;

void D2DEmptyControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;
	bk_mode_ = 1;

}

static void DrawLattice(D2DContext& cxt, FSizeF sz, float box)
{
	D2DMatrix mat(*cxt);
	mat.PushTransform();
	ComPTR<ID2D1SolidColorBrush> br;
	(*cxt)->CreateSolidColorBrush(D2RGB(235,235,235), &br);

	int hcnt = sz.height / box * 2;
	int wcnt = sz.width / box * 2;

	FRectF brc(0,0,box,box);
	FRectF brc2 = brc.OffsetRect(box,box);

	for(int i =0; i < hcnt; i++)
	{
		mat.PushTransform();
		for(int j=0; j<wcnt; j++ )
		{
			(*cxt)->FillRectangle(brc, br);
			(*cxt)->FillRectangle(brc2, br);

			mat.Offset(box*2, 0);
		}
		mat.PopTransform();
		mat.Offset(0, box*2);
	}

	mat.PopTransform();
}

void D2DEmptyControls::Draw(D2DContext& cxt)
{
	D2DRectFilter f(cxt, rc_);

	cxt.DFillRect(rc_, ColorF::White);

	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc_);

	if ( bk_mode_ != 0 )
	{
		if (0 < bk_mode_ )
			DrawLattice(cxt, rc_.Size(), 25 );
		if (bk_mode_ == 2)
		{
			WCHAR cb[256];
			StringCbPrintf(cb, (size_t)_countof(cb), L"D2DEmptyControls this=%x\n w=%0.2f,h=%0.2f", this, rc_.Width(), rc_.Height() );

			UIHandle h={};
			h.p = this;

			D2DDrawText(h, FPointF(10,10), cb );


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

	}


	return DefWndProc(b, message, wParam, lParam);
}


