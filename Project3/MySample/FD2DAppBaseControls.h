#pragma once

#include "D2DMessage.h"

#define BITFLG2(a, x) ((a&x)==x)


namespace V6
{
	class FD2DAppBaseControls
	{
		public :
			FD2DAppBaseControls(){ hndl_.p= nullptr;} 
			virtual ~FD2DAppBaseControls(){}

			void Create(UIHandle parent, LPCWSTR name, FRectF rc, int id);

			virtual bool Draw(ID2D1DeviceContext* cxt) = 0;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;

			UIHandle Get(){ return hndl_; }
		protected :
			UIHandle hndl_;
			

	};



	bool CreateTextFormat( float height, LPCWSTR fontnm, int bold,  IDWriteTextFormat** pfmt );



};