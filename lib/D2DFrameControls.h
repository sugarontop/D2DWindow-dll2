#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6
{
	//‰æ–Ê‚ð•ªŠ„‚·‚é‚½‚ß‚ÌƒNƒ‰ƒX

	class D2DFrameControls : public D2DControls
	{
		public :
			D2DFrameControls(){};

			virtual void CreateControl(D2DWindow* parent, D2DControls* parentcontrol, const FRectF& rc,int rowcnt, int colcnt, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc){ rc_ = rc; }
			virtual void Draw(D2DContext& cxt) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			void AutoResize(int mode);

			D2DControls* Get(int idx);
			void Set(D2DControls* newctrl, int idx);

		private :
			int row_cnt_;
			int col_cnt_;

			std::vector<FSizeF> area_;

	};
};
