#pragma once



#ifdef _WINDLL
#define CLASSEXPORT __declspec(dllexport)
#else
#define CLASSEXPORT __declspec(dllimport)
#endif



struct AppBase
{
	AppBase():hWnd(0),card(0),bRedraw(false){}
	AppBase(HWND h, void* p): hWnd(h),card(p),bRedraw(false){}
	HWND hWnd;	
	void* card;
	bool bRedraw;
	void GetClientRect( int* cx, int* cy )
	{
		RECT rc;
		::GetClientRect(hWnd,&rc);

		*cx = rc.right - rc.left;
		*cy = rc.bottom - rc.top;
	}
	void Redraw() const
	{
		InvalidateRect(hWnd,NULL,FALSE);
	}

};

struct MenuItem
{
	int id;
	int typ;
	LPCWSTR str;
};
