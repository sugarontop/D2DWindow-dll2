#pragma once


#ifdef _WINDLL
#define DLLEXPORT extern "C" __declspec(dllexport)
#else
#define DLLEXPORT extern "C"
#endif

#include "D2DMessage.h"
#include "D2DInterface.h"
namespace V6 {

class D2DWindow;
class D2DControls;


DLLEXPORT  void WINAPI D2DDestroyControl(UIHandle hcs  );
DLLEXPORT  void WINAPI D2DDestroyWindow(UIHandleWin main  );
DLLEXPORT LRESULT WINAPI D2DDefWndProc(UIHandleWin main ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);
DLLEXPORT void WINAPI D2DForceWndProc(UIHandleWin main, AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);
DLLEXPORT LRESULT WINAPI D2DDefControlProc(UIHandle ctrls ,AppBase& app, UINT message, WPARAM wParam, LPARAM lParam);



DLLEXPORT bool WINAPI D2DResource(D2DWindow* win,  D2D1_RECT_F& rc );
DLLEXPORT HANDLE WINAPI D2DInstanceInitail(INT_PTR p );

DLLEXPORT UIHandle WINAPI D2DCreateWhiteWindow( UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateSquarePaper(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateTextbox(UIHandle hctrl, const D2D1_RECT_F& rc, bool multiline, DWORD stat, LPCWSTR name, int id=-1, int ext=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateConsole(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id, int ext=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateButton(UIHandle hctrl , const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateDropdownListbox(UIHandle ctrls , const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DCreateControls(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1);
DLLEXPORT UIHandle WINAPI D2DMessageBox(UIHandle hctrl,  const D2D1_RECT_F& rc, LPCWSTR title, LPCWSTR message);
DLLEXPORT UIHandle WINAPI D2DFloatingMenu(UIHandleWin hwin, const D2D1_RECT_F& rc, LPVOID sender, MenuItem* items, int items_cnt);
DLLEXPORT UIHandle WINAPI D2DCreateStatic(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR text, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateEmptyControls(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateXXXControls(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateSimpleControls(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT UIHandle WINAPI D2DCreateListbox(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateWhiteControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateClientControls(LPVOID captureobj, DelegateDrawFunc func1, DelegateProcFunc func2, UIHandle ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateScrollbar(UIHandle ctrls, bool bVertical, DWORD stat, LPCWSTR name, int id);
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithScrollbar(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateControlsWithMove(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateMDISplitFrame(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateChildWindow(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateFileManage(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );


DLLEXPORT UIHandle WINAPI D2DCreateLogin(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DCreateDataGridView(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT UIHandle WINAPI D2DCreateTabControls(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );
DLLEXPORT UIHandle WINAPI D2DAddNewTab(UIHandle hctrl, LPCWSTR nm);
DLLEXPORT UIHandle WINAPI D2DGetTab(UIHandle hctrl, USHORT idx);
DLLEXPORT UIHandle WINAPI D2DGetControlFromIdx(UIHandle hctrl, USHORT idx);
DLLEXPORT bool WINAPI D2DIsControls(UIHandle hctrl);

DLLEXPORT void WINAPI D2DInnerDraw(UIHandle hctrl);
DLLEXPORT void WINAPI D2DDefaultDraw(UIHandle hctrl);
DLLEXPORT UIHandle WINAPI D2DCreateSqlDataGrid(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id );


DLLEXPORT UIHandle WINAPI D2DCreateImage(UIHandle hctrl, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id=-1 );

DLLEXPORT bool WINAPI D2DSetProcfunction(UIHandle hctrl, DelegateProcFunc func2);

DLLEXPORT UIHandle WINAPI D2DCast(void* target);


DLLEXPORT bool WINAPI D2DGetDWriteFormat(UIHandle h, IDWriteTextFormat** out);

DLLEXPORT void WINAPI D2DSetFocus(UIHandle hctrl);
DLLEXPORT void WINAPI D2DSetText(UIHandle hctrl, LPCWSTR str);
DLLEXPORT bool WINAPI D2DSetFont(UIHandle hctrl, LPCWSTR fontnm, float height, bool bold=false, int align =0);
DLLEXPORT DWORD WINAPI D2DSetStat(UIHandle hctrl, DWORD stat);
DLLEXPORT void WINAPI D2DSetColor(UIHandle hctrl, ColorF back, ColorF front, ColorF border );
DLLEXPORT void WINAPI D2DReadOnly(UIHandle hctrl, bool readonly);
DLLEXPORT void WINAPI D2DEnable(UIHandle hctrl, bool enable);

DLLEXPORT int WINAPI D2DAddItem(UIHandle hctrl, LPCWSTR str);
DLLEXPORT void WINAPI D2DSelectItem(UIHandle hctrl, int idx);
DLLEXPORT void WINAPI D2DSetTabText(UIHandle hctrl, LPCWSTR str);

DLLEXPORT DWORD WINAPI D2DGetStat(UIHandle hctrl);

DLLEXPORT void WINAPI D2DInsertText(UIHandle hctrl, LPCWSTR str, int len, int start_pos);
DLLEXPORT BSTR WINAPI D2DGetText(UIHandle hctrl, bool bAll=true );

DLLEXPORT int WINAPI D2DSendMessage(UIHandle hctrl, UINT msg, WPARAM, LPARAM);
DLLEXPORT int WINAPI D2DTabSendMessage(UIHandle hctrl, UINT msg, WPARAM, LPARAM);

DLLEXPORT UIHandleWin WINAPI D2DCreateMainHWnd( HWND hWnd, float font_height, int typ=0 );

DLLEXPORT UIHandle WINAPI D2DGetRootControls(UIHandleWin hMainWnd );
DLLEXPORT UIHandle WINAPI D2DGetControlFromID(UIHandleWin hMainWnd, UINT id);
DLLEXPORT UIHandle WINAPI D2DGetControlFromName(UIHandleWin hMainWnd, LPCWSTR nm);

DLLEXPORT UIHandleWin WINAPI D2DGetWindow(UIHandle hctrl);
DLLEXPORT UIHandle WINAPI D2DGetParent(UIHandle hctrl);

DLLEXPORT void WINAPI D2DGetTextFormat(UIHandle hctrl, IDWriteTextFormat** out);

DLLEXPORT ID2D1DeviceContext* WINAPI D2DGetDeviceContext(UIHandleWin main);

DLLEXPORT void WINAPI D2DSetCapture(UIHandle hctrl);
DLLEXPORT UIHandle WINAPI D2DGetCapture();
DLLEXPORT bool WINAPI D2DIsCapture(UIHandle hctrl);
DLLEXPORT void WINAPI D2DReleaseCapture();

DLLEXPORT void WINAPI D2DSwapChain(UIHandleWin main, HWND hWnd  );
DLLEXPORT void WINAPI D2DDraw(UIHandleWin main, void* option  );

DLLEXPORT void WINAPI D2DDrawText(UIHandle hctrl, D2D1_POINT_2F pt,  LPCWSTR str );






DLLEXPORT D2D1_RECT_F WINAPI D2DGetRect(UIHandle hctrl );
DLLEXPORT D2D1_RECT_F* WINAPI D2DGetRect2(UIHandle hctrl);

DLLEXPORT int  WINAPI D2DGetId(UIHandle hctrl );

DLLEXPORT BSTR WINAPI D2DGetName(UIHandle hctrl);
DLLEXPORT BSTR WINAPI D2DGetLocalName(UIHandle hctrl);

DLLEXPORT void WINAPI D2DSetRect( UIHandle hctrl, D2D1_RECT_F rc );
DLLEXPORT UIHandle WINAPI D2DSetTopControl(UIHandle hctrl);
DLLEXPORT UIHandle WINAPI D2DSetLastControl(UIHandle hctrl);
DLLEXPORT void WINAPI D2DClear(UIHandle hctrl);

DLLEXPORT void WINAPI D2DScrollbarSetMaxSize(UIHandle hctrl, float height);


//DLLEXPORT void D2DEventHandlerOnClick( UIHandle hctrl, D2DEventHandler handler);
DLLEXPORT void WINAPI D2DEventHandler( UIHandle hctrl, D2DEventHandlerDelegate handler);

////////
//DLLEXPORT UIHandle D2DCreateDropdownListbox(D2DWindow* win, D2DControls* ctrls, const D2D1_RECT_F& rc, DWORD stat, LPCWSTR name, int id);
DLLEXPORT XDropdownListBox* WINAPI D2DConvert(UIHandle r);


DLLEXPORT D2D1_RECT_F* WINAPI D2DRectAnimation(const D2D1_RECT_F&  rcS, const D2D1_RECT_F& rcE, D2D1_RECT_F* p, int p_size, int style);
DLLEXPORT void WINAPI D2DSmoothRect(int typ, int id, UIHandle hctrl, D2D1_RECT_F* target, D2D1_RECT_F dstRect);
DLLEXPORT bool WINAPI D2DStream2Bitmap( IStream* bmpstream, ID2D1RenderTarget* target, ID2D1Bitmap** bmp);

DLLEXPORT void WINAPI D2DMDICreateChildView(UIHandle hctrl,int typ);
DLLEXPORT UIHandle WINAPI D2DMDIGetControl(UIHandle hctrl, LPCWSTR nm);

DLLEXPORT bool WINAPI D2DSetOnClick(UIHandle hctrl, DelegateClick func);

DLLEXPORT int WINAPI D2DAddBitmapItem(UIHandle hctrl, ID2D1Bitmap* bmp);


DLLEXPORT ID2D1RenderTarget* WINAPI D2DGetRenderTarget(UIHandle hctrl);

DLLEXPORT void D2DRedraw(UIHandle hctrl);
DLLEXPORT void D2DGetClientRect(UIHandle hctrl, D2D1_RECT_F* prc);
};