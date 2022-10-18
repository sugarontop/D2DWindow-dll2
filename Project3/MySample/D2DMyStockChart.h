#pragma once


#include "D2DAppBaseControls.h"
#include "chart\StockChart.h"

namespace V6 {

class D2DMyStockChart: public D2DAppBaseControls
	{
		public :
			

			StockChart stock_chart_;

			D2DMyStockChart(float cx, float cy):stock_chart_(FRectF(0,0,cx,cy))
			{
				rc_.top = 0;
				rc_.left = 0;				
				rc_.bottom = cy;
				rc_.right = cx;
			
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