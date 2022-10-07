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


			struct dot
			{
				FPointF pt;
				USHORT idx;
			};


			std::vector<dot> ar_;
			D2DMat mat_;
			USHORT dot_idx_;


	};


};