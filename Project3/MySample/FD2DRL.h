#pragma once
namespace V6
{

	class FD2DRLeControls: public FD2DAppBaseControls
	{
		public :
			FD2DRLeControls(){};

			virtual bool Draw(ID2D1DeviceContext* cxt) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			void SetPitRect( FSizeF rcSize, float* Q, int rowcnt, int colcnt);
			void DrawQvalue(ID2D1DeviceContext* cxt,ID2D1SolidColorBrush* br, FPointF pt, int s, float* Q4);


			float* Q_;
			int rowcnt_;
			int colcnt_;
			FRectF pitRect_;

			ComPTR<IDWriteTextFormat> textformat_,textformat2_;

			std::function<void(UIHandle,INT_PTR)> Eventhandler_;

			D2DMat mat_;
			FRectF rc_;
			bool bRedraw_;
						

	};

};

