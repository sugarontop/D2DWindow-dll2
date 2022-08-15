#pragma once
#include "D2DApp.h"
#include "D2DMessage.h"

namespace V6
{
	class D2DAppBaseControls
	{
		public :
			D2DAppBaseControls(){ hndl_.p= nullptr;} 


			void Create(UIHandle parent, LPCWSTR name, FRectF rc, int id);

			virtual bool Draw(ID2D1DeviceContext* cxt) = 0;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;

			UIHandle Get(){ return hndl_; }
		protected :
			UIHandle hndl_;
			

	};



	bool CreateTextFormat( float height, LPCWSTR fontnm, int bold,  IDWriteTextFormat** pfmt );



};