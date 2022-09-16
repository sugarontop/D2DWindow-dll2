#include "pch.h"
#include "D2DConsole.h"
#include "D2DColor.h"
#include "D2DMessage.h"


using namespace V6;
using namespace TOOL;


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


	this->back_ = ColorF::Black;
	this->fore_ = ColorF::White;
	lastpos_ = 0;


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
			}
		}
		break;
		case WM_D2D_ON_TEXTBOX_RETURN:
		{
			if ( (D2DControl*)wParam == static_cast<D2DControl*>(input_.get()) )
			{
				int a = 0;

				D2DLineInput* p = (D2DLineInput*)input_->GetLink();

				p->OnCommand(2);
				//OnCommand()
				r = 1;
			}
		}
		break;
	}

	if ( r == 0 )
		r = D2DControls::DefWndProc(b,message,wParam,lParam);



	return r;
}


void D2DConsole::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		D2DRectFilter f(cxt, rc_);

		mat_ = mat.Offset(rc_); // ¶ã‚ð(0,0)‚Æ‚µ‚Ä•\Ž¦
		
		auto rc = rc_.ZeroRect();
		
		auto back = mk_color(back_);
		

		(*cxt)->FillRectangle(rc, back);
	
		(*cxt)->DrawRectangle(rc, cxt.black_);
		
		this->InnerDraw(cxt);


		mat.Offset(rc_.Width(), 0);
		vbar_->Draw2(cxt);

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
		r = D2DControls::DefWndProc(b,message,wParam,lParam);
	return r;
}
void D2DLineInput::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // ¶ã‚ð(0,0)‚Æ‚µ‚Ä•\Ž¦
		
		auto rc = rc_.ZeroRect();

		
		(*cxt)->FillRectangle(rc, cxt.black_);
		(*cxt)->DrawText(str_.c_str(), str_.length(), cxt.textformat_, rc, cxt.white_ );
		
		mat.Offset(0,rc_.Height());
		this->InnerDraw(cxt);

		mat.PopTransform();

	}

}
void D2DLineInput::OnCommand(int typ)
{

	if ( typ == 2 )
	{
		
		FRectF rc(0,0, FSizeF(rc_.Width(), TEMP_HEIGHT ));
		auto one = std::make_shared<D2DSampleOutput>();
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
		r = D2DControls::DefWndProc(b,message,wParam,lParam);
	return r;
}

void D2DSampleOutput::Draw(D2DContext& cxt)
{
	if ( BITFLG(STAT_VISIBLE) )
	{
		D2DMatrix mat(*cxt);
		mat.PushTransform();

		mat_ = mat.Offset(rc_); // ¶ã‚ð(0,0)‚Æ‚µ‚Ä•\Ž¦
		
		auto rc = rc_.ZeroRect();

		auto br = mk_color(D2RGB(50,50,50));
		(*cxt)->FillRectangle(rc, br);
		
		
		//this->InnerDraw(cxt);

		mat.PopTransform();

	}

}
