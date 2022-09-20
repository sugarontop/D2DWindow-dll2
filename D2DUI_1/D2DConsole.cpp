#include "pch.h"
#include "D2DConsole.h"
#include "D2DColor.h"
#include "D2DMessage.h"


using namespace V6;
using namespace TOOL;

#define  APP (D2DApp::GetInstance())

#define LINE_HEIGHT 26
#define TEMP_HEIGHT 100

D2DConsole::D2DConsole()
{

}

void D2DConsole::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol, rc, stat, name, local_id );

	this->rc_ = rc;


	FRectF rcscb(0,0,FSizeF(20, rc.Height()));
	vbar_ = std::make_shared<D2DScrollbar>();
	vbar_->CreateControl(parent, this, rcscb, STAT_DEFAULT, NONAME);
	vbar_->SetSize(rc_.Size());
	vbar_->SetMaxSize(rc_.Size().height);


	this->back_ = ColorF::Black;
	this->fore_ = ColorF::White;
	lastpos_ = 0;
	offbar_y_ = 0;
	scstat_ =0;
	scbarThumbHeight_ = 0;


	FRectF rctext(0,0, FSizeF(rc.Width(), LINE_HEIGHT ));
	auto one = std::make_shared<D2DLineInput>();
	one->CreateControl(parent, this, rctext, STAT_DEFAULT, NONAME );
	this->Add(one);


	input_ = std::make_shared<D2DTextbox>();
	input_->CreateControl(parent,this, IBridgeTSFInterface::SINGLELINE, rctext, STAT_DEFAULT, NONAME);
	this->Add(input_);

	input_->SetBackColor(back_);
	input_->SetForeColor(fore_);
	input_->SetLink(one.get());

	lastpos_ += rctext.Height();

}
float D2DConsole::sc_dataHeight()
{
	return lastpos_;
}
float D2DConsole::sc_barThumbHeight()
{
	return scbarThumbHeight_;
}

float D2DConsole::sc_barTotalHeight()
{
	return rc_.Height();
}

void D2DConsole::sc_barSetBottom()
{
	float overflow = max(0, sc_dataHeight() - sc_barTotalHeight());
	scbarThumbHeight_ = sc_barTotalHeight() - overflow;


	offbar_y_ = sc_dataHeight();
	if ( offbar_y_ + sc_barThumbHeight() > rc_.Height())
		offbar_y_ = rc_.Height()- sc_barThumbHeight();


}
static FPointF ptold;
LRESULT D2DConsole::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	switch(message)
	{
		case WM_D2D_SET_ACTIVE_CONTROL:
		{
			if ( wParam == 1 )
			{
				lastpos_ += TEMP_HEIGHT;

				std::wstringstream sm;
				input_->GetText(&sm);
				D2DLineInput* p = (D2DLineInput*)input_->GetLink();

				p->str_ = sm.str();


				FRectF rctext(0,lastpos_, FSizeF(rc_.Width(), LINE_HEIGHT ));
				auto one = std::make_shared<D2DLineInput>();
				one->CreateControl(parent_window_, this, rctext, STAT_DEFAULT, NONAME );
				this->Add(one);

				input_->SetRect(rctext);
				input_->SetStat(STAT_VISIBLE|STAT_ENABLE);
				input_->SetLink(one.get());

				lastpos_ += LINE_HEIGHT;

				vbar_->SetMaxSize(lastpos_);



				sc_barSetBottom();

				
				r = 1;
			}
		}
		break;
		case WM_D2D_ON_TEXTBOX_RETURN:
		{
			if ( (D2DControl*)wParam == static_cast<D2DControl*>(input_.get()) )
			{
				D2DLineInput* p = (D2DLineInput*)input_->GetLink();

				p->OnCommand(2);
				//OnCommand()
				r = 1;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
			FRectF rcz = rc_.ZeroRect();		
			
            if (rcz.PtInRect(pt))
            {
                //ptold = pt;

                if ( rcz.right - BARW < pt.x && pt.x < rcz.right)
                {
                    APP.SetCapture(this);
					ptold = pt;
                    scstat_ = 3;
					r = 1;
                }
				
			}
		}
		break;
		case WM_LBUTTONUP:
        {            
            scstat_ = 0;

			if ( APP.IsCapture(this))
			{
				APP.ReleaseCapture();
				r = 1;
			}

            break;
        }
		break;
		case WM_MOUSEMOVE:
        {
            MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1: 0 );

           /* if (mouse_stat_ != md )
            {
                mouse_stat_ = md;
                b.bRedraw = true;

            }*/

            //if ( md == 1 )
            {
                /*auto y = pt.y + offbar_y_ * scbai_;
                int idx = (int)(y / rowheight);

                if (rc_.ZeroRect().PtInRect(pt))
                {
                    b.bRedraw = true;
                    if ( idx != float_idx_  && -1 < idx  && InnerRect(rc_).ZeroRect().PtInRect(pt))
                    {
                        float_idx_ = idx;
                    }
                }*/

				if ( APP.IsCapture(this))
				{
					if ( scstat_ == 3)
					{						
						sc_MouseMove(pt);
						b.bRedraw = true;
						
					}

					r = 1;
				}

                
            }
        }
        break;
		case WM_MOUSEWHEEL:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.ZeroRect().PtInRect(pt) ? 1 : 0);
			//auto md = (rc_.PtInRect(pt) ? 1 : 0);

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

					//vbar_->Offset(a);
					offbar_y_ = max(0,min(sc_dataHeight(), offbar_y_+a ));


					if ( offbar_y_ + sc_barThumbHeight() > rc_.Height())
						offbar_y_ = rc_.Height()- sc_barThumbHeight();


				}
				r = 1;
            }
		}
		break;
	}

	if ( r == 0 )
		r = InnerWndProc(b,message,wParam,lParam); //D2DControls::DefWndProc(b,message,wParam,lParam);



	return r;
}



bool D2DConsole::sc_MouseMove(FPointF& pt)
{
    offbar_y_ = max(0, offbar_y_ + (pt.y - ptold.y));

    offbar_y_ = min(sc_barTotalHeight()-sc_barThumbHeight()+BARW, offbar_y_);

    ptold = pt;

    return false;
}
#define _min_thum_height 24.0f

void D2DConsole::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		D2DRectFilter f(cxt, rc_);

		auto back = mk_color(back_);
		(*cxt)->FillRectangle(rc_, back);

		mat_ = mat.Offset(rc_.left, rc_.top); // - offbar_y_); //

		mat.PushTransform();
		mat.Offset(0,-offbar_y_);
		this->InnerDraw(cxt);
		mat.PopTransform();

		mat.PushTransform();
		{
			mat.Offset(0.0f, offbar_y_);
			//mat.Offset(0, rc_.top - offbar_y_); // 左上を(0,0)として表示

			float overflow = max(0, sc_dataHeight() - sc_barTotalHeight());
			scbarThumbHeight_ = sc_barTotalHeight() - overflow;
			const float min_thum_height = _min_thum_height;
			float scbai_ = 1.0f;
			if (scbarThumbHeight_ < min_thum_height)
			{
				scbarThumbHeight_ = min_thum_height;
				scbai_ = (sc_dataHeight() - sc_barTotalHeight()) / (sc_barTotalHeight() - scbarThumbHeight_); 
			}

			FRectF scbar(rc_.Size().width - BARW, 0, rc_.Size().width, scbarThumbHeight_);
			cxt.DFillRect(scbar, D2RGB(225,5,5));
		}
		mat.PopTransform();


		//mat_ = mat.Offset(rc_.left, rc_.top - offbar_y_); // 左上を(0,0)として表示
		
		//auto rc = rc_.ZeroRect();
	
		//(*cxt)->DrawRectangle(rc, cxt.black_);
		
		


		//mat.Offset(rc_.Width()-BARW, 0);
		//vbar_->Draw2(cxt);

		mat.PopTransform();

	}
}

// --------------------------------------------------------------------------------------------------------------------------------------

D2DLineInput::D2DLineInput()
{

}
void D2DLineInput::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol, rc, stat, name, local_id );

	this->rc_ = rc;
	str_ = L"...";
}
LRESULT D2DLineInput::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	switch( message )
	{
		case WM_KEYDOWN:
		{
			bool bShift   = ((GetKeyState(VK_SHIFT)& 0x80) != 0);    
			bool bControl = ((GetKeyState(VK_CONTROL)& 0x80) != 0);    
		}
		break;
	}

	if ( r == 0 )
		r = InnerWndProc(b,message,wParam,lParam);
	return r;
}
void D2DLineInput::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // 左上を(0,0)として表示
		
		auto rc = rc_.ZeroRect();

		
		(*cxt)->FillRectangle(rc, cxt.black_);
		(*cxt)->DrawText(str_.c_str(), str_.length(), cxt.textformat_, rc, cxt.white_ );
		
		mat.Offset(0,rc_.Height());
		this->InnerDraw(cxt);

		mat.PopTransform();

	}

}

#include "D2DEmptyControls.h"
void D2DLineInput::OnCommand(int typ)
{

	if ( typ == 2 )
	{
		
		FRectF rc(0,0, FSizeF(rc_.Width(), TEMP_HEIGHT ));
		
		//auto one = std::make_shared<D2DSampleOutput>();

		auto one = std::make_shared<D2DEmptyControls>();
		one->CreateControl(parent_window_, this, rc, STAT_DEFAULT, NONAME );
		this->Add(one);

		


		_ASSERT(this->controls_.size() == 1);


		parent_control_->SendMesage(WM_D2D_SET_ACTIVE_CONTROL, 1, 1 );
	}

}

// --------------------------------------------------------------------------------------------------------------------------------------
D2DSampleOutput::D2DSampleOutput()
{
	
}
void D2DSampleOutput::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DControls::CreateControl(parent, pacontrol, rc, stat, name, local_id );

	this->rc_ = rc;
}
LRESULT D2DSampleOutput::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	switch( message )
	{
		

	}

	if ( r == 0 )
		r = InnerWndProc(b,message,wParam,lParam);
	return r;
}

void D2DSampleOutput::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // 左上を(0,0)として表示
		
		auto rc = rc_.ZeroRect();

		auto br = mk_color(D2RGB(50,50,50));
		(*cxt)->FillRectangle(rc, br);
		
		
		//this->InnerDraw(cxt);

		mat.PopTransform();

	}

}
