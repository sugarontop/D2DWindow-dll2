#include "pch.h"
#include "D2DWindow.h" 
#include "D2DTabControls.h"
#include "D2DSquarePaper.h"
#include "D2DControls_with_Scrollbar.h"
using namespace V6;

#define  APP (D2DApp::GetInstance())
#define range(first,x, last)	((int)first <= (int)x && x < last)


#define TAB_HEIGHT	26.0f
#define TAB_WIDTH	200.0f

#define TAB_TOP_BAR			0
#define TAB_BOTTOM_BAR		1


void D2DTabControls::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat2_ = mat.PushTransform();

	

	mat_ = mat.Offset(rc_);

	if ( tab_typ_ == TAB_TOP_BAR )
	{
		auto tab_height = DrawTab(cxt,tab_idx_);

		mat.Offset(0,tab_height);

		FSizeF sz(rc_.Size().width,rc_.Size().height-tab_height);

		controls_[tab_idx_]->Draw(cxt);
	}
	else if ( tab_typ_ == TAB_BOTTOM_BAR)
	{
		FSizeF sz(rc_.Size().width,rc_.Size().height-TAB_HEIGHT);

		mat.PushTransform();
		{
			D2DRectFilter f(cxt, FRectF(0,0,sz));

			controls_[tab_idx_]->Draw(cxt);

		}
		mat.PopTransform();

		mat_ = mat.Offset(0, sz.height);

		auto tab_height = DrawTab(cxt,tab_idx_);

		_ASSERT(TAB_HEIGHT == tab_height);
	}

	mat.PopTransform();

}

float D2DTabControls::DrawTab(D2DContext& cxt, USHORT tabidx)
{
	D2DMatrix mat(*cxt);
	mat.PushTransform();

	float tab_height = tabrects_[0].Height();

	USHORT k = 0;
	for(auto& it : tabrects_)
	{
		auto clr1 = DEFAULT_TAB_COLOR; 
		auto clr2 = ColorF::Black;

		if ( k == tabidx )
		{
			clr1 = DEFAULT_TAB_ACTIVE_COLOR;
			clr2 = ColorF::White;
		}

		cxt.DDrawRect(it, ColorF::Black, clr1);

		auto tab_name = this->controls_[k]->GetLocalName();
		cxt.DText(FPointF(it.left+6, it.top), tab_name.c_str(), clr2);

		k++;
	}


	mat.PopTransform();
	return tab_height;
}

D2DControl* D2DTabControls::GetControlFromIdx(USHORT idx)
{
	if ( idx < (USHORT)controls_.size())
		return this->controls_[idx].get();

	return nullptr;
}

LRESULT D2DTabControls::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_D2D_CREATE:

			r = 1;
		break;


		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;

			auto pt = mat2_.DPtoLP(pm.pt);

			auto nm = GetName();

			if ( rc_.PtInRect(pt))
			{
				int k = 0;
				auto old = tab_idx_;
				pt = mat_.DPtoLP(pm.pt);

				for(auto& it : tabrects_)
				{
					nm = controls_[k]->GetName();

					if ( it.PtInRect(pt))
					{
						tab_idx_ = k;
						r = 1;

						if ( old !=tab_idx_ )
							parent_window_->SendMessage(WM_D2D_TAB_CHANGE, (WPARAM)this, (LPARAM)tab_idx_);
							
						//APP.SetCapture(this);
						break;
					}
					k++;
				}
			}

		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				r = 1;
			}
		}
		break;
		case WM_D2D_SET_TAB_POSITION:
		{
			tab_typ_ = (lParam == 0 ? TAB_TOP_BAR : TAB_BOTTOM_BAR );

			r = 1;
		}
		break;
		case WM_D2D_SET_ACTIVE_CONTROL:
		{
			auto idx = (USHORT)lParam;

			if ( idx < controls_.size() )
			{
				tab_idx_ = idx;
				r = 1;
			}
			else
			{
				r = LRESULT_FAIL;
			}
		}
		break;
		case WM_D2D_SET_SIZE_FROM_CHILDWINDOW:
		{
			FSizeF sz = *(FSizeF*)lParam;
			rc_.SetSize(sz);

			sz.height -= tabrects_[0].Height();

			for(auto& it : controls_)
				it->WndProc(b,WM_D2D_SET_SIZE_FROM_CHILDWINDOW,wParam,(LPARAM)&sz);
			return 0;
		}
		break;
		case WM_D2D_SET_SIZE:
		{
			{					
				FRectF rc = *(FRectF*)lParam;
			
				if ( size_fix_ )
				{
					rc = rc_.ZeroRect();
					rc.top += tabrects_[0].Height();
				}
				else
				{
					rc_.SetWH(rc);
			
					rc.top += tabrects_[0].Height();
			
					rc = rc.ZeroRect();
				}
			
				for(auto& it : controls_)
					it->WndProc(b,WM_D2D_SET_SIZE,wParam,(LPARAM)&rc);

			}
			
			return 0;

		}
		break;
		case WM_D2D_TABCONTROL_TAB_NAME:
		{
			std::wstring s = (LPCWSTR)(LPARAM)lParam;

			r = 0;
		}
		break;
		case WM_D2D_SET_TEXT:
		{
			UINT idx = (UINT)wParam;

			if ( idx < controls_.size())
			{
				auto nm = GetName();
				nm += L"@";
				nm += (LPCWSTR)lParam;

				controls_[idx]->SetName(nm.c_str());
				r = 1;
			}
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			ColorF clr = *(ColorF*)lParam;

			if ( wParam == COLOR_IDX_BACK )
				back_ = clr;
			else if ( wParam == COLOR_IDX_FORE )
				fore_ = clr;
			//else if ( wParam == 2 )
			//	border_ = clr;

			r = 1;
		}
		break;

	}

	if ( r == 0 )
	{
		auto ctrls = dynamic_cast<D2DControls*>(controls_[tab_idx_].get());

		if ( ctrls )
			r = ctrls->InnerWndProc(b,message,wParam,lParam);
	}

	return r;
}

void D2DTabControls::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);


	if (rc.IsEmpty())
	{
		auto xrc = parent_control_->GetRect();
		rc_ = xrc.ZeroRect();

	}
	else
		rc_ = rc;


	tab_idx_ = 0;
	size_fix_ = false;
	tab_typ_ = TAB_TOP_BAR;

	back_ = ColorF::White;
	fore_ = ColorF::Black;

	for(int i = 0; i < 1; i++ )
	{
		WCHAR nm[64];
		wsprintf(nm,L"%s@%d", name_.c_str(), i);

		if (BITFLG(STAT_SIMPLE))
		{			
			auto page1 = std::make_shared<D2DControls>();
			page1->CreateControl(parent,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, nm );
			Add(page1);
		}
		else
		{
			auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
			page1->CreateControl(parent,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, nm );
			Add(page1);
		}
	}

	// 
	for(int i=0; i < 1; i++ )
	{
		FRectF rc(0,0,FSizeF(TAB_WIDTH,TAB_HEIGHT));
		rc.Offset(i*TAB_WIDTH,0);
		tabrects_.push_back(rc);

	}
}

static int ParseText1(LPCWSTR str, std::wstring* title)
{
	auto ar = SplitW(str, L":" );
	int ret = -1;
	if ( ar.size() == 2 )
	{
		ret = _wtoi(ar[0].c_str());
		*title = ar[1];
	}
	return ret;
}



void D2DTabControls::SetText(LPCWSTR str)
{
	std::wstring title;
	int idx = ParseText1(str, &title);
	
	if ( range(0, idx, controls_.size()))
	{
		std::wstring tabnm = name_;
		tabnm += L"@";
		tabnm += title;

		this->controls_[idx]->SetName(tabnm.c_str());
	}
}

D2DControls* D2DTabControls::AddNewTab(LPCWSTR tabName)
{
	D2DControls* ret = nullptr;

	std::wstring tabnm = name_;
	tabnm += L"@";
	tabnm += tabName;

	if (BITFLG(STAT_SIMPLE))
	{
		auto page1 = std::make_shared<D2DControls>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, tabnm.c_str() );
		Add(page1);
		ret = page1.get();
	}
	else
	{
		auto page1 = std::make_shared<D2DControls_with_Scrollbar>();
		page1->CreateControl(parent_window_,this, FRectF(0,0,rc_.Size()), STAT_DEFAULT, tabnm.c_str() );
		Add(page1);
		ret = page1.get();

	}



	auto i = tabrects_.size();
	FRectF rc(0,0,FSizeF(TAB_WIDTH,TAB_HEIGHT));
	rc.Offset(i*TAB_WIDTH,0);
	tabrects_.push_back(rc);

	return ret;
}
void D2DTabControls::DelTab(USHORT idx)
{
	
	auto p = GetControlFromIdx(idx);

	if ( p )
		p->DestroyControl();

}
std::wstring D2DTabControls::GetTreeTyp(USHORT* typ)
{ 
	*typ=3; 
	return L"D2DTabControls";
}



