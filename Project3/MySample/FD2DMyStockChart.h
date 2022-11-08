#pragma once


#include "FD2DAppBaseControls.h"
#include "chart\StockChart.h"

#define TOPBAR_HEIGHT 40.0f
#define WM_D2D_APP_SETDEFAULT_CD		(WM_D2D_USER_FIRST+1)
#define WM_D2D_APP_ON_CHART_CHANGED		(WM_D2D_USER_FIRST+2)


class FD2DDialogTest;


namespace V6 {



class FD2DMyStockChart: public FD2DAppBaseControls
{
	public :
			

		StockChart stock_chart_;

		FD2DMyStockChart(float cx, float cy):stock_chart_(FRectF(0,TOPBAR_HEIGHT,cx,cy))
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

	protected :
		void InnerDraw(ID2D1RenderTarget* cxt);
		bool CreateMemoryView(ID2D1RenderTarget* target, FSizeF sz, ID2D1Bitmap** pview);
		void ShowDialog(int n);

		ComPTR<ID2D1Bitmap> view_;
		
		std::vector<dot> ar_;
		D2DMat mat_;
		USHORT dot_idx_;
			

		UIHandle cd_, intv_, now_value_;
		std::shared_ptr<FD2DDialogTest> dlg_;

};

class FD2DMyStockDataView: public FD2DAppBaseControls
{
	public :

		FD2DMyStockDataView(float cx, float cy)
		{
			// "rc_" is setted in WM_D2D_CREATE
		}

		virtual bool Draw(ID2D1DeviceContext* cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		FRectF rc_;
		std::wstring str_;
		D2DMat mat_;
		USHORT dot_idx_;
			
		UIHandle cd_, hgridview_;

		
};


};