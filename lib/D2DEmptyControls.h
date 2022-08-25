#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6
{
	class D2DEmptyControls : public D2DControls
	{
		public :
			D2DEmptyControls(){};

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc){ rc_ = rc; }
			virtual void Draw(D2DContext& cxt) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		private :
			int bk_mode_;

	};
};

