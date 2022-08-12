#include "pch.h"
#include "D2DWindow.h" 
#include "D2DWindowControl.h"
#include "D2DContext.h"
#include "FilePackTool.h"
using namespace V6;
LRESULT D2DWindow::PostMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	EnterCriticalSection( &message_lock_ );
	
	MSG m(msg,wp,lp);
	post_message_queue_.push_back( m );
	
	LeaveCriticalSection(&message_lock_);
	
	return 0;
}

LRESULT D2DWindow::SendMessage(UINT msg, WPARAM wp, LPARAM lp)
{
	AppBase b(hWnd_, nullptr);
	return WndProc(b, msg, wp, lp);
}

LRESULT D2DWindow::CheckPostMessage(AppBase* pb)
{
	if ( !post_message_queue_.empty() )
	{
		if (TryEnterCriticalSection(&message_lock_))
		{
			auto queue = post_message_queue_;
			post_message_queue_.clear();

			LeaveCriticalSection(&message_lock_);
			
			AppBase bb(hWnd_, nullptr);
			for(auto& it : queue )
				InnerWndProc(bb, it.msg, it.wp, it.lp);

			if ( pb )
				pb->bRedraw=bb.bRedraw;
		}
	}
	return 0;
}

LRESULT D2DWindow::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	CheckPostMessage(&b);

	return InnerWndProc(b, message, wParam, lParam);
}

LRESULT D2DWindow::InnerWndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	static MouseParam mp;
	LRESULT hr = 0;

	_ASSERT(message != WM_SIZE);

	switch( message )
	{
		case WM_MOUSEMOVE :
			mp.pt = FPointF(lParam);						
			lParam = (LPARAM)&mp;
		break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			mp.pt = FPointF(lParam);
			mp.ptprv = mp.pt;
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		break;
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
			mp.pt = FPointF(lParam);
			mp.ptprv = mp.pt;
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		break;
		case WM_MOUSEWHEEL:
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
			::ScreenToClient(b.hWnd, &pt);
			mp.pt = FPointF((float)pt.x, (float)pt.y);
			mp.fwKeys = GET_KEYSTATE_WPARAM(wParam);
			mp.zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		}
		break;
		case WM_LBUTTONDBLCLK:
			mp.pt = FPointF(lParam);
			mp.ptprv = mp.pt;
			b.bRedraw = true;
			lParam = (LPARAM)&mp;
		break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		{
			b.bRedraw = true;
		}
		break;
		case WM_NOTIFY:
		{
			hr = ::SendMessage(b.hWnd,WM_NOTIFY,wParam,lParam);

		}
		break;
		case WM_D2D_GET_RESOURCE_BINARY:
		{									
			static ComPTR<IStream> sm2;

			if ( sm2 == nullptr )
			{
				ComPTR<IStream> sm;
				LPCWSTR fnm = L"res.bin";

				bool bl = FILEPACK::Convert2Stream(fnm,MAGIC_NUMBER, &sm);
				if ( bl )
					bl = FILEPACK::DeCompress(sm, &sm2);
			}

			auto ppsm = (IStream**)lParam;
			*ppsm = sm2; 

			if (sm2)
				sm2.AddRef();

			hr = 1;
		}
		break;
		case WM_D2D_TOOLTIP:
		{
			if ( lParam )
				tooltip_ = *(ToolTipInfo*)lParam;
			else
				tooltip_.bShow = false;

			b.bRedraw = true;
			hr = 1;
		}
		break;

		case WM_TIMER:
		{
			// HEART_BEET��post��push���邽�߂̃^�C�~���O
			if ( wParam == HEART_BEET_ID )
				hr = 1;
		}
		break;				
		
	}

	if ( hr == 0 )
		hr = top_control_->WndProc(b, message, wParam, lParam);

	if ( message == WM_MOUSEMOVE )
	{
		mp.ptprv = mp.pt;
	}



	return hr;
}
FRectF D2DWindow::GetClientRect() const
{
	RECT rc;
	::GetClientRect(hWnd_, &rc);
	return FRectF( (float)rc.left, (float)rc.top, (float)rc.right, (float)rc.bottom );
}
void D2DWindow::ListUp(std::vector<ControlMapItem>& ar)
{
	int row=0;
	int col=0;
	
	top_control_->ListUp(ar, &row,&col);
}
void D2DWindow::DrawToolTip(D2DContext& cxt)
{
	if ( !tooltip_.bShow ) return;
	
	cxt.DFillRect(tooltip_.rc, ColorF::Yellow);
	cxt.DText(tooltip_.rc.LeftTop(5,2), tooltip_.str ); 


	auto wow = [](LPVOID p)->DWORD {

		D2DWindow* win = (D2DWindow*)p;
		Sleep(800);

		win->tooltip_.bShow = false;
		InvalidateRect(win->GetHwnd(),NULL,FALSE);
		return 0;
	};

	DWORD dw;	
	::CreateThread(nullptr,0, wow, this,0,&dw);


}

int D2DWindow::GetControlFromName(LPCWSTR nm, std::vector<D2DControl*>& ret ) const
{
	auto& map = name_map_;
	auto it = map.begin();
	while( it != map.end())
	{
		auto pos = it->first.find(nm);

		if ( pos != -1 )
		{
			ret.push_back(it->second);			
		}
		it++;
	}

	return (int)ret.size();

}