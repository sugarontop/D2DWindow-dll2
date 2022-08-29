#pragma once
namespace V6
{
	class D2DSampleControls: public D2DAppBaseControls
	{
		public :
			D2DSampleControls(){};

			virtual bool Draw(ID2D1DeviceContext* cxt) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			FRectF rc_;
			std::wstring str_;


			std::vector<FPointF> ar_;
			D2DMat mat_;

	};


};