#include "pch.h"
#include "D2DControls_with_Scrollbar.h"

using namespace V6;

#define  APP (D2DApp::GetInstance())

#define TEST_COLOR		D2RGB(200,200,200)

D2DControls_with_Scrollbar::D2DControls_with_Scrollbar()
{
	vscroll_x_ = 0;
	hscroll_x_ = 0;
	
	backcolor_ = TEST_COLOR;
}

void D2DControls_with_Scrollbar::SetRect(const FRectF& rc)
{
	rc_ = rc;

}
void D2DControls_with_Scrollbar::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	
	D2DRectFilter f(cxt, rc_);
	
	cxt.DFillRect(rc_,  backcolor_);

	mat.Offset(rc_);

	{
		mat.PushTransform();		
			mat.Offset(-sch_->LogicalOffset(), -scv_->LogicalOffset());
			InnerDraw(cxt);
		mat.PopTransform();


		// Draw scrollbar Vert
		mat.PushTransform();	
			mat.Offset(vscroll_x_, 0 );
			scv_->Draw2(cxt);
		mat.PopTransform();

		// Draw scrollbar Horz
		mat.PushTransform();	
			mat.Offset(hscroll_x_, rc_.Height()-BARW);
			sch_->Draw2(cxt);
		mat.PopTransform();
	}

	mat.PopTransform();
}


// Control����Child View�ƃX�N���[���o�[�̐ݒ�
void D2DControls_with_Scrollbar::SetViewMaxSize(FSizeF inner_view_sz, FSizeF scrollbar_sz)
{				
	_ASSERT((0 < inner_view_sz.width) && (0 < inner_view_sz.height));

	rc_.SetSize(inner_view_sz);

	vscroll_x_ = inner_view_sz.width-BARW;
	hscroll_x_ = 0;


	scv_->SetStat(STAT_DEFAULT);
	sch_->SetStat(STAT_DEFAULT);

	scv_->SetMaxSize(scrollbar_sz.height );
	sch_->SetMaxSize(scrollbar_sz.width );
	sch_->SetSize(inner_view_sz);
	scv_->SetSize(inner_view_sz);

	if (scrollbar_sz.width <= inner_view_sz.width)
		sch_->SetStat(0);
	if (scrollbar_sz.height <= inner_view_sz.height)
		scv_->SetStat(0);
	
}


std::wstring D2DControls_with_Scrollbar::GetTreeTyp(USHORT* typ)
{ 
	*typ=3; 
	return L"D2DControls_with_Scrollbar";
}
LRESULT D2DControls_with_Scrollbar::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	bool bl = true;

	switch( message )
	{
		case WM_SIZE:
		{			
			FSizeF veiw_sz(lParam);
			
			if (controls_.size() >= 2 && rc_.left == 0 && rc_.top == 0)
			{
				auto client = this->controls_[2];
				auto rc = client->GetRect();

				_ASSERT(rc.left == 0 && rc.top == 0);
				
				auto scroll_size = rc.Size();
				
				SetViewMaxSize(veiw_sz, scroll_size);

				

				client->WndProc(b, message, 1, (LPARAM)&rc);

				return 0;
			}
		}
		break;
		//case WM_SIZE:
		case WM_D2D_SET_SIZE:
		{
			if (controls_.size() == 3)
			{
				FSizeF veiw_sz(lParam);
				if (wParam == 0 || wParam == 1)
				{
					if (controls_.size() >= 2 && rc_.left == 0 && rc_.top == 0)
					{
						auto client = this->controls_[2];
						auto rc = client->GetRect();

						_ASSERT(rc.left == 0 && rc.top == 0);

						auto scroll_size = rc.Size();

						SetViewMaxSize(veiw_sz, scroll_size);



						client->WndProc(b, message, 1, (LPARAM)&rc);

						return 0;
					}					
				}	
				/*else if ( wParam == 3 )
				{
					auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child				

					SetViewMaxSize2(rc_.Size(), crc.Size());
					//SetViewMaxSize(crc.Size());
				}
				else if ( wParam == 4 )
				{
					auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child
					rc_.SetSize(crc.Size());

					SetViewMaxSize2(rc_.Size(), crc.Size());
					//SetViewMaxSize(crc.Size());
				}
				else if ( wParam == 5 )
				{
					

					rc_.SetSize( this->parent_control_->GetRect().Size());
					
					auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child


					SetViewMaxSize2(rc_.Size(), crc.Size());
					//SetViewMaxSize(crc.Size());
				}
				*/
			}

			return 0;
		}
		break;
		case WM_D2D_SET_SIZE_FROM_OUTER:
		{
			//if (BITFLG(STAT_AUTOFIT_CHILDWIN))
			{
				FSizeF sz = *(FSizeF*)lParam;

				rc_.SetSize(sz);

				if ( 2 < controls_.size() )
				{				
					this->controls_[2]->WndProc(b,WM_D2D_SET_SIZE_FROM_OUTER,wParam,(LPARAM)&sz);

					auto crc = this->controls_[2]->GetRect(); // 0,1 is scrollbar, 2 is child
					
					SetViewMaxSize( rc_.Size(), crc.Size());
				}
			}
			return 0;
		}
		break;		
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( !rc_.ZeroPtInRect(pt))
			{
				//bl = false;
			}

		}
		break;
		case WM_MOUSEMOVE:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( !rc_.ZeroPtInRect(pt))
			{
				//bl = false;
				int a = 0;
			}

		}
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);

			if ( !rc_.ZeroPtInRect(pt))
			{
				//bl = false;
			}

		}
		break;
		case WM_MOUSEWHEEL:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            //auto md = (rc_.ZeroRect().PtInRect(pt) ? 1 : 0);
			auto md = (rc_.PtInRect(pt) ? 1 : 0);

            if ( md == 1 )
            {
				r = InnerWndProc(b,message,wParam,lParam);

				if( r == 0)
				{
					float a = 0;
					if (mp->zDelta > 0)
						a = -10.0f;
					if (mp->zDelta < 0)
						 a= 10.0f;

					scv_->Offset(a);
				}
				r = 1;
            }
		}
		break;
		case WM_D2D_SET_COLOR:
		{
			if ( wParam == COLOR_IDX_BACK )
				backcolor_ = *(ColorF*)lParam;
			//else if ( wParam == 1 )
			//	forecolor_ = *(ColorF*)lParam;

			r = 1;
			

		}
		break;
		case WM_D2D_COMMAND_SET:
		{
			if ( (UINT_PTR)this == (UINT_PTR)wParam)
			{
				LPCWSTR cmd = (LPCWSTR)lParam;

				auto ar = SplitW(cmd,L"&");

				for(auto& it : ar)
				{
					auto ar2 = SplitW(it.c_str(), L"=");

					if ( ar2.size() == 2)
					{
						if (ar2[0] == L"mode")
						{
							//mode_ = _wtoi(ar2[1].c_str());
						}
						else if ( ar2[0] == L"enable")
						{
							int a = _wtoi(ar2[1].c_str());

							if (a ==0)
								stat_ &= ~STAT_ENABLE;
							else
								stat_ |= STAT_ENABLE;

						}
						else if ( ar2[0] == L"bkcolor")
						{
							DWORD clr = _wtoi(ar2[1].c_str());

							D2DColor k;
							k.FromInt(clr);

							//back_color_ = k;
						}
						else if ( ar2[0] == L"title")
						{
							LPCWSTR nm = ar2[1].c_str();
							
							auto parent = GetParentControls();
							parent->SendMesage(WM_D2D_SET_TEXT,(WPARAM)parent, (LPARAM)nm);



						}

					}
				}

				r = 1;
			}
		}
		break;
		
		

	}


	if ( r == 0 && bl )
	{
		r = InnerWndProc(b,message,wParam,lParam);
	}

	return r;

}
void D2DControls_with_Scrollbar::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc_width_scbar, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	rc_ = pacontrol->GetRect().ZeroRect();
		

	vscroll_x_ = 0;
	hscroll_x_ = 0;

	FRectF rc1(0,0,BARW,100); 
	auto scV = std::make_shared<D2DScrollbar>();
	scV->CreateControl(parent, this, rc1,0, NONAME );
	this->Add(scV);
	scv_ = scV;


	rc1.SetRect(0,0,100, BARW);
	auto scH = std::make_shared<D2DScrollbar>();
	scH->CreateControl(parent, this, rc1,0, NONAME );
	this->Add(scH);
	sch_ = scH;



	scV->SetMaxSize(rc_width_scbar.Height());
	scH->SetMaxSize(rc_width_scbar.Width());
}

D2DControls* D2DControls_with_Scrollbar::GetMainControls()
{
	_ASSERT( controls_.size() == 3);

	return dynamic_cast<D2DControls*>(controls_[2].get());
}

// /////////////////////////////////////////////////////////////////////////////////////////////////

void D2DControls_with_Move::Draw(D2DContext& cxt)
{
	D2DMatrix mat(*cxt);

	mat_ = mat.PushTransform();
	
	//D2DRectFilter f(cxt, rc_);

	FRectF rc = controls_[0]->GetRect();
	mat.Offset(rc_.left-rc.left, rc_.top-rc.top);
	
	controls_[0]->Draw(cxt);

	mat.PopTransform();
	
	//D2DControls::Draw(cxt);

	mat.PushTransform();
	

	mat.Offset(rc_);
	auto sz = rc_.Size();
	
	rc.SetRect(0,0,10,10); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(sz.width-10,0,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(0,sz.height-10,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	rc.SetRect(sz.width-10,sz.height-10,FSizeF(10,10)); cxt.DFillRect(rc, ColorF::Red);
	mat.PopTransform();

	
}
void D2DControls_with_Move::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

}
LRESULT D2DControls_with_Move::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;

	switch( message )
	{
		case WM_LBUTTONDOWN:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			auto pt = mat_.DPtoLP(pm.pt);
			if ( rc_.PtInRect(pt))
			{
				APP.SetCapture(this);

				r = 1;
			}
			else if ( target_ )
			{
				target_->SetNewParent(prv_controls_);

				target_->SetRect(rc_);
				
				target_ = nullptr;
				prv_controls_ = nullptr;

				

				r = 1;

				this->DestroyControl();

			}
		}
		break;
		case WM_MOUSEMOVE:
		{
			if ( APP.GetCapture() == this )
			{
				MouseParam& pm = *(MouseParam*)lParam;

				auto pt = mat_.DPtoLP(pm.pt);
				auto pt1 = mat_.DPtoLP(pm.ptprv);

				auto offx = pt.x-pt1.x;
				auto offy = pt.y-pt1.y;
				rc_.Offset(offx,offy);

				b.bRedraw = true;
				r = 1;

			}

		}
		break;
		case WM_LBUTTONUP:
		{
			MouseParam& pm = *(MouseParam*)lParam;
			if ( APP.GetCapture() == this )
			{
				APP.ReleaseCapture();


				r = 1;
			}
		}		
		break;
	}


	if ( r == 0 )
		r = D2DControls::DefWndProc(b,message,wParam,lParam);

	return r;

}