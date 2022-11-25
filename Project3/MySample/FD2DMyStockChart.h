#pragma once


#include "FD2DAppBaseControls.h"
#include "chart\StockChart.h"

#define TOPBAR_HEIGHT 40.0f
#define WM_D2D_APP_SETDEFAULT_CD		(WM_D2D_USER_FIRST+1)
#define WM_D2D_APP_ON_CHART_CHANGED		(WM_D2D_USER_FIRST+2)
#define WM_D2D_APP_SET_CD				(WM_D2D_USER_FIRST+3)

class FD2DDialogTest;


namespace V6 {


class ChartShot
{
	public:
		ChartShot(const StockChart* org);

		void Draw(FRectF rc, ID2D1RenderTarget* cxt);

		void Load(std::wstring cd, UIHandle hdl);

		std::shared_ptr<StockChart> stock_chart_;
};



class FD2DMyStockChart: public FD2DAppBaseControls
{
	public :

		std::unique_ptr<StockChart> stock_chart_;

		FD2DMyStockChart(){};

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
		void DrawPrimeChart(ID2D1DeviceContext* cxt);

		bool CreateMemoryView(ID2D1RenderTarget* target, FSizeF sz, ID2D1Bitmap** pview);
		void ShowDialog(int n, FRectF rc);

		ComPTR<ID2D1Bitmap> view_;
		
		std::vector<dot> ar_;
		D2DMat mat_;
		USHORT dot_idx_;
			

		UIHandle txtCD_, txtIntv_, now_value_;
		std::shared_ptr<FD2DDialogTest> dlg_;
		std::map<std::wstring, PrimeStockDataItem> prime_;

		std::vector<std::shared_ptr<ChartShot>> shots_;
};

class FD2DMyStockDataView: public FD2DAppBaseControls
{
	public :

		FD2DMyStockDataView(){};

		virtual bool Draw(ID2D1DeviceContext* cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

		FRectF rc_;
		std::wstring str_;
		D2DMat mat_;
		USHORT dot_idx_;
			
		UIHandle cd_, hgridview_;

		
};


};