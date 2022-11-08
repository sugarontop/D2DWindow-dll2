#include "pch.h"
#include "D2DDialog.h"
#include "D2DTextbox.h"
#include "D2DDropdownListbox.h"

using namespace V6;
#define  APP (D2DApp::GetInstance())

D2DDialog::~D2DDialog()
{
	int a = 0;
}
void D2DDialog::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id )
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);
	rc_ = rc;

}
LRESULT D2DDialog::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;


	switch( message )
	{
		case WM_D2D_CREATE:
		{	
			hndl_ = *(UIHandle*)lParam;	


			APP.SetCapture(this);

			auto typ = IBridgeTSFInterface::SINGLELINE;

			auto txt = std::make_shared<D2DTextbox>();
			txt->CreateControl(GetParent(), this, typ, FRectF(10,10,FSizeF(200,30)), STAT_DEFAULT, NONAME);
			this->Add(txt);

			auto txt2 = std::make_shared<D2DTextbox>();
			txt2->CreateControl(GetParent(), this, typ, FRectF(10,60,FSizeF(200,30)), STAT_DEFAULT, NONAME);
			this->Add(txt2);

			auto ls = std::make_shared<D2DDropdownListbox>();
			ls->CreateControl(GetParent(), this, FRectF(10,110,FSizeF(200,200)), STAT_DEFAULT, NONAME);
			ls->AddItem(L"xxxxx1");
			ls->AddItem(L"xxxxx2");
			ls->AddItem(L"xxxxx3");
			ls->AddItem(L"xxxxx4");
			ls->AddItem(L"xxxxx5");
			this->Add(ls);

			r = 1;
		}
		break;
		/*case WM_D2D_DESTROY:
		{

			APP.ReleaseCapture();


			delete this;
			r = 1;
		}
		break;*/
		case WM_D2D_ON_DESTROY:
		{

			int a = 0;
		}
		break;
		case WM_KEYDOWN:
		{
			if ( wParam == VK_ESCAPE )
			{
				APP.ReleaseCapture();

				
				
				DestroyControl();

				r = 1;
			}
		}
		break;
	}


	if ( r == 0 )
		r = D2DControls::DefWndProc(b,message,wParam,lParam);

	return r;
}
void D2DDialog::Draw(D2DContext& cxt)
{
	if (BITFLG(STAT_VISIBLE))
    {		
		ComPTR<ID2D1SolidColorBrush> back;
		ComPTR<ID2D1SolidColorBrush> fore;

		(*cxt)->CreateSolidColorBrush(D2RGB(0,0,0), &fore);	
		(*cxt)->CreateSolidColorBrush(D2RGB(180,255,255), &back);

        D2DRectFilter fil(cxt, rc_ );
		(*cxt)->FillRectangle(rc_,back);

		D2DMatrix mat(*cxt);
        mat_ = mat.PushTransform();

       
        mat.Offset(rc_);
		

		//auto textformat = GetParent()->GetContext().textformat_;
		//std::wstring s = L"D2DDialog::Draw";
		//(*cxt)->DrawText(s.c_str(),(UINT32)s.length(), textformat, FRectF(0,0,200,100), fore);


		InnerDraw(cxt);

		mat.PopTransform();
	}
}