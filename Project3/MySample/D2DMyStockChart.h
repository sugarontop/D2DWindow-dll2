#pragma once


#include "D2DAppBaseControls.h"
#include "chart\StockChart.h"

#define TOPBAR_HEIGHT 40.0f
#define WM_D2D_APP_SETDEFAULT_CD	(WM_D2D_USER_FIRST+1)

namespace V6 {

class D2DMyStockChart: public D2DAppBaseControls
	{
		public :
			

			StockChart stock_chart_;

			D2DMyStockChart(float cx, float cy):stock_chart_(FRectF(0,TOPBAR_HEIGHT,cx,cy))
			{
				// "rc_" is setted in WM_D2D_CREATE
			}

			virtual bool Draw(ID2D1DeviceContext* cxt) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			FRectF rc_;
			std::wstring str_;

			ComPTR<IDWriteTextFormat> wformat_;

			struct dot
			{
				FPointF pt;
				USHORT idx;
			};


			std::vector<dot> ar_;
			D2DMat mat_;
			USHORT dot_idx_;
			

			UIHandle cd_, intv_;

	};

};