#pragma once



#include "D2DApp.h"
#include "D2DWindowControl.h"

namespace V6
{
	class D2DDialog: public D2DControls
	{
		public :
			D2DDialog(){};
			virtual ~D2DDialog();

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;

			UIHandle hndl_;

	};

};