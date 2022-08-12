#pragma once
#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DTreeList.h"

namespace V6
{
	class D2DListbox : public D2DControl
	{
		public :
			D2DListbox();


			virtual FRectF GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc) { rc_ = rc; }
			virtual void Draw(D2DContext& cxt) ;

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;




		protected :
			FRectF rc_;
			TreeList gtree;

	};
};