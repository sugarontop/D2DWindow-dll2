#pragma once


#include "D2DAppBaseControls.h"
#include "chart\StockChart.h"

namespace V6 {

class D2DMyStockChart: public D2DAppBaseControls
	{
		public :
			D2DMyStockChart(){};

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
			StockChart stock_chart_;

			UIHandle cd_, intv_;

	};

};