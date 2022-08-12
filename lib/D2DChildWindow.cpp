#include "pch.h"
#include "D2DChildWindow.h"

using namespace V6;


#define  APP (D2DApp::GetInstance())

#define TITLEBAR_HEIGHT 26.0f

D2DChildWindow::D2DChildWindow()
{
	mode_ = 0;
	title_bar_mode_ = 0;
	window_mode_ = 0;
	title_ = L"no title";

	colors_[0] = D2RGB(170,170,170);
	colors_[1] = D2RGB(200,200,200);
}
void D2DChildWindow::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol,rc, stat, name, local_id);
	mode_ = 0;
	title_bar_mode_ = 0;
	window_mode_ = 0;
	title_ = L"no title";

	colors_[0] = D2RGB(170,170,170);
	colors_[1] = D2RGB(200,200,200);
}

void D2DChildWindow::Add(std::shared_ptr<D2DControl> p)
{
	D2DControls::Add(p);

	
}

void D2DChildWindow::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	auto& nm = this->name_;

	mat_ = mat.PushTransform();

//D2DRectFilter f(cxt, rc_);

	if ( window_mode_ == 1 )
		mini_window_->Draw(cxt);
	else
	{
		cxt.DDrawRect(rc_, ColorF::Black, ColorF::White);

		mat.Offset(rc_);
	
		DrawTitlebar(cxt);

		mat.Offset(0,TITLEBAR_HEIGHT);
		
		for(auto& it : controls_ )
			it->Draw(cxt);
	}
	mat.PopTransform();
}
LRESULT D2DChildWindow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT h = 0;
	bool bl = true;

	bool finished = false;

	if ( mini_window_ )
	{
		h = mini_window_->WndProc(b,message,wParam,lParam);
		return h;
	}

	switch( message )
	{
		case WM_D2D_CREATE:
			h = 1;
		break;

		case WM_SIZE:
		case WM_D2D_SET_SIZE:
		{
			auto sz = rc_.Size();
			sz.height -= TITLEBAR_HEIGHT;  // TITLEBAR_HEIGHT:26

			//DefWndProc(b,WM_D2D_SET_SIZE_FROM_CHILDWINDOW,0,(LPARAM)&sz);

			InnerWndProc(b,WM_D2D_SET_SIZE_FROM_CHILDWINDOW,0,(LPARAM)&sz);


			bl = false;

		}
		break;


		case WM_LBUTTONDBLCLK:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			if ( !rc_.PtInRect(pt))
			{
				bl = false;
				
			}

		}
		break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			auto old = title_bar_mode_;
			

			if ( rc_.PtInRect(pt) || mode_ == 1)
			{
				if ( (rc_.top < pt.y && pt.y < rc_.top+TITLEBAR_HEIGHT) || mode_ ==1 )
				{
					h = TitleBarProc(b,message,pm);
				}
			}
			else
				bl = false;


			if ( old != title_bar_mode_ )							
				b.bRedraw = true;
			
		}
		break;
		case WM_MOUSEMOVE:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			auto old = title_bar_mode_;

			h = TitleBarProc(b,message,pm);


			if ( old != title_bar_mode_ )							
			{
				b.bRedraw = true;
				h = 1;
			}

			
		}
		break;
		case WM_D2D_SET_TEXT:
		{
			title_ = (LPCWSTR)lParam;

			h = 1;
		}
		break;

	}

	if ( h==0 && bl )
	{
		h = InnerWndProc(b,message,wParam,lParam);
	}

	if (h == 0)
	{
		// window���Ȃ瑼��message���񂳂Ȃ�
		switch(message)
		{
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MOUSEMOVE:
			case WM_LBUTTONDBLCLK:
			case WM_MOUSEHWHEEL:
			//case WM_MOUSEWHEEL:

				MouseParam& pm = *(MouseParam*)lParam;
				auto pt = mat_.DPtoLP(pm.pt);
				if ( rc_.PtInRect(pt))
					h = 1;
			break;
		}
	}
	
	return h;
}

LRESULT D2DChildWindow::TitleBarProc(AppBase& b, UINT message, MouseParam& pm)
{
	LRESULT h = 0;
	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			if ( title_bar_mode_ == 0)
			{
				APP.SetCapture(this);
				mode_ = 1;

				
				GetParentControls()->SetFirstControl(this);

			}
			else if ( title_bar_mode_ > 0)
			{
				//APP.SetCapture(this);
				mode_ = 0;
			}

			h = 1;
		}
		break;
		case WM_MOUSEMOVE:
		{
			auto pt = mat_.DPtoLP(pm.pt);
			auto old = title_bar_mode_;

			if ( APP.IsCapture(this) && mode_ == 1)
			{								
				auto pt2 = mat_.DPtoLP(pm.ptprv);

				rc_.Offset(pt.x-pt2.x, pt.y-pt2.y);

				b.bRedraw = true;

				h = 1;
			}
			else
			{
				mode_ = 0;
				FRectF rc2(rc_.right-45,rc_.top,rc_.right,rc_.top+27);
				
				title_bar_mode_ = 0;
				if ( rc2.PtInRect(pt))
				{
					title_bar_mode_ = 1; h=1; // close
				}
				else
				{
					rc2.Offset(-45,0);
					if ( rc2.PtInRect(pt)){
						title_bar_mode_ = 2; h=1; // maximum size
					}
					else
					{
						rc2.Offset(-45,0);
						if ( rc2.PtInRect(pt)) {
							title_bar_mode_ = 3; h=1; // minimum size
						}
						else
							title_bar_mode_ = 0;
					}
				}
			}

			
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				mode_ = 0;
			}

			if (mode_ == 0)
			{
				if ( title_bar_mode_ == 1 )
				{
					DestroyControl();
				}
				else if ( title_bar_mode_ == 2 )
				{

				}
				else if ( title_bar_mode_ == 3 )
				{
					window_mode_ = 1;

					mini_window_ = std::make_shared<MinimumWindow>(this,FRectF(rc_.left,rc_.bottom, FSizeF(50,50)));
				}
			}


			h = 1;
		}
		break;
	}

	return h;
}





 

 static void DrawTitlebar2(D2DContext& cxt, const FRectF& rc,ColorF titlebar, LPCWSTR title, int tmd)
{
	FRectF rcbar = rc.ZeroRect(); 
	cxt.DFillRect(rcbar, titlebar);

	//(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);

	D2DMatrix mat(*cxt);
	mat.PushTransform();
	mat.Offset(rc.Width()-45, 0); // width:45, three buttons


	FRectF rc2(0,0,45,27);
	if ( tmd == 1 )
		cxt.DFillRect(rc2, D2RGB(232,17,35)); // DESTROY

	(*cxt)->DrawLine(FPointF(11+5,11), FPointF(21+5,21), cxt.black_);
	(*cxt)->DrawLine(FPointF(21+5,11), FPointF(11+5,21), cxt.black_);



	mat.Offset(-45, 0);

	if ( tmd == 2 )
		cxt.DFillRect(rc2, D2RGB(229,229,229)); // LARGE

		
	(*cxt)->FillRectangle(FRectF(11,11,21,21), cxt.transparent_);
	(*cxt)->DrawRectangle(FRectF(11,11,21,21), cxt.black_);

	mat.Offset(-45, 0);

	if ( tmd == 3 )
		cxt.DFillRect(rc2, D2RGB(229,229,229)); // CLOSE

	(*cxt)->DrawRectangle(FRectF(11,15,21,15), cxt.black_);

	mat.PopTransform();

	//(*cxt)->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);


	rcbar.Offset(2,0);
	(*cxt)->DrawText(title, wcslen(title), cxt.textformat_, rcbar, cxt.black_);

}

void D2DChildWindow::DrawTitlebar(D2DContext& cxt)
{
	DrawTitlebar2(cxt, FRectF(0,0,rc_.Width(),TITLEBAR_HEIGHT),colors_[CLR::TITLEBAR], title_.c_str(), title_bar_mode_ );
	

}
std::wstring D2DChildWindow::GetTreeTyp(USHORT* typ)
{ 
	*typ=3; 
	return L"D2DChildWidow";
}



void MinimumWindow::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);
	mat_ = mat.PushTransform();
	
	cxt.DFillRect(rc_, ColorF::LightGray); 

	mat.PopTransform();

}
LRESULT MinimumWindow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch(message)
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( rc_.PtInRect(pt))
			{
				APP.SetCapture(parent_);
				r = 1;
			}
		}
		break;
		case WM_MOUSEMOVE:
		{		
			if ( APP.IsCapture(parent_))
			{
				MouseParam& pm = *(MouseParam*)lParam;
				auto pt = mat_.DPtoLP(pm.pt);
				auto pt2 = mat_.DPtoLP(pm.ptprv);

				rc_.Offset( pt.x-pt2.x, pt.y-pt2.y );
				r = 1;
				b.bRedraw = true;
			}
			
		}
		break;
		case WM_LBUTTONUP:
		{
			if ( APP.IsCapture(parent_))
			{
				APP.ReleaseCapture();

				r = 1;
			}
		}
		break;
		case WM_LBUTTONDBLCLK:
		{			
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			if ( rc_.PtInRect(pt))
			{
				if ( APP.IsCapture(parent_))
					APP.ReleaseCapture();


				parent_->window_mode_ = 0;
				parent_->mini_window_ = nullptr;

				

				r = 1;
			}
		}
		break;
	}

	return r;
}