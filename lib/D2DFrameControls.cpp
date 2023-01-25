 #include "pch.h"
 #include "D2DFrameControls.h"
 #include "D2DEmptyControls.h"
 #include "D2D1UI_1.h"
 using namespace V6;


 void D2DFrameControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, int rowcnt, int colcnt,DWORD stat, LPCWSTR name, int local_id)
{
	_ASSERT(0 < rowcnt && 0 < colcnt );
	
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;
	row_cnt_ = rowcnt;
	col_cnt_ = colcnt;

	FRectF rc1(0,0,rc.Size());

	for(int i =0; i < row_cnt_*col_cnt_;i++)
	{
		//auto cs = std::make_shared<D2DControls>();		
		auto cs = std::make_shared<D2DPlaceHolder>();

		WCHAR ch[2]={};
		ch[0] = ('A' + i);

		cs->CreateControl(parent, this, rc1, STAT_DEFAULT, ch );
		controls_.push_back(cs);
	}
	AutoResize(1);
}

void D2DFrameControls::AutoResize(int mode)
{	
	if ( mode == 1 )
	{	
		area_.clear();

		float h = rc_.Height() / row_cnt_;
		float w = rc_.Width() / col_cnt_;
		
		for(int i = 0; i < row_cnt_*col_cnt_; i++ )
			area_.push_back(FSizeF(w,h));
	}

	if ( mode == 0 )
	{




	}

}
D2DControls* D2DFrameControls::Get(int idx)
{
	_ASSERT( idx < controls_.size());
	return (D2DControls*)controls_[idx].get();
}

void D2DFrameControls::Set(D2DControls* newctrl, int idx)
{
	_ASSERT( idx < controls_.size());
	auto old = controls_[idx];

	old->DestroyControl();


	controls_[idx] = std::shared_ptr<D2DControl>(newctrl);
}


void D2DFrameControls::Draw(D2DContext& cxt)
{
	D2DRectFilter f(cxt, rc_);
	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc_);

	_ASSERT(controls_.size() == area_.size());

	auto it = controls_.begin();
	
	FSizeF sz;
	int i =0;
	for(auto y=0; y < row_cnt_; y++ )
	{
		mat.PushTransform();
		for(auto x =0; x < col_cnt_; x++ )			
		{
			sz  = area_[i++];

			{
				D2DRectFilter f2(cxt, FRectF(0,0,sz));			
				(*it)->Draw(cxt);
			}

			mat.Offset(sz.width, 0);
			it++;
		}
		mat.PopTransform();

		mat.Offset(0,sz.height);
	}

	mat.PopTransform();
}
LRESULT D2DFrameControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch(message)
	{
		case WM_SIZE:
		{
			auto cx = LOWORD(lParam);
			auto cy = HIWORD(lParam);

			if ( stat_ & STAT_AUTOFIT_TOP_WIN )
				rc_.SetRect(0,0,cx,cy);


			AutoResize(1);


			_ASSERT(controls_.size() == area_.size());

			int i = 0;
			for(auto& it : controls_)
			{
				auto sz = area_[i++];
				it->WndProc(b, WM_D2D_SET_SIZE_FROM_OUTER, 0, (LPARAM)&sz);
			}
		}
		break;
	}

	if ( r == 0 )
		return DefWndProc(b, message, wParam, lParam);


	return r;
}