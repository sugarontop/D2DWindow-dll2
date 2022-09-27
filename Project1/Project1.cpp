#include "pch.h"
#include "Project1.h"
#include "AppBase.h"
#include "d2d1UI_1.h"


using namespace V6;

#define  APP (D2DApp::GetInstance())
UIHandleWin hwin;


#pragma comment(lib,"D2DUI_1.lib")


#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


	if ( 0!=::CoInitialize(0) )
	    return FALSE;

	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF|_CRTDBG_LEAK_CHECK_DF);



    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = NULL;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

static UIHandleWin CreateTopControl(HWND hWnd)
{
	auto uihwin = D2DCreateMainHWnd(hWnd, 14);    
	auto root = D2DGetRootControls(uihwin);
	//--------------------------------------------
	
	
	// Textbox controlを作成
	FRectF rctextbox(100, 40, FSizeF(600, 30));
	UIHandle tx1 = D2DCreateTextbox(root, rctextbox, false, STAT_DEFAULT, L"textbox1");		
	D2DSetFont(tx1, L"メイリオ", 20, true, 0 );
	D2DSetText(tx1, L"Left alignment"); 


	rctextbox.Offset(0,40);
	UIHandle tx2 = D2DCreateTextbox(root, rctextbox, false, STAT_DEFAULT, L"textbox2");		
	D2DSetFont(tx2, L"メイリオ", 20, true,1 );
	D2DSetText(tx2, L"Center"); 

	rctextbox.Offset(0,40);
	UIHandle tx3 = D2DCreateTextbox(root, rctextbox, false, STAT_DEFAULT, L"textbox3");		
	D2DSetFont(tx3, L"メイリオ", 20, true,2 );
	D2DSetText(tx3, L"Right"); 

	UIHandle tx4 = D2DCreateTextbox(root, FRectF(100,200,1000,1000), true, STAT_DEFAULT, L"textbox4");	
	D2DSetFont(tx4, L"MS 明朝", 16, false,0 );


	//UIHandle tx4 = D2DCreateConsole(root, FRectF(900,10,FSizeF(500,800)), STAT_DEFAULT, L"console1", 1);	
	//D2DSetFont(tx4, L"HGS行書体", 25, false,0 );

	//----------------------------------------------
	return uihwin;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    AppBase app;
    app.hWnd = hWnd;
    LRESULT r = 0;

	switch (message)
    {
		case WM_CREATE:
		{
			hwin = CreateTopControl(hWnd);
			D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0); //<--OnRestructGraphicsResources(cxt,hWnd);
		}
		break;
		case WM_SIZE:
        {
            D2DForceWndProc(hwin, app, message, wParam,lParam);
        }
		break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				BeginPaint(hWnd, &ps);

				{
					auto cxt = D2DGetDeviceContext(hwin);
					cxt->BeginDraw();
					D2D1_MATRIX_3X2_F mat = {0};

					mat._11 = 1.0f;
					mat._22 = 1.0f;

					cxt->SetTransform(mat);
					cxt->Clear(D2RGB(255,255,255));

					D2DDraw(hwin, hWnd);

					auto hr = cxt->EndDraw();

					if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
					{
						D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 0, 0);  //<--OnDiscardGraphicsResources(cxt);
						D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 1, 0);  // create Context
						D2DForceWndProc(hwin, app, WM_D2D_RESOURCES_UPDATE, 2, 0);   //<--OnRestructGraphicsResources(cxt,hWnd);
					}
					else
					{
						D2DSwapChain(hwin, hWnd);
					}
            	}
				EndPaint(hWnd, &ps);
				return 0;
			}
			break;
		case WM_DESTROY:
			D2DDestroyWindow(hwin);
			PostQuitMessage(0);
			r=1;
		break;
		case WM_KEYDOWN:
        {
            bool bShift   = ((GetKeyState(VK_SHIFT)& 0x80) != 0);       
            bool bCtrl = ((GetKeyState(VK_CONTROL) & 0x80) != 0);

            auto c = D2DGetCapture();

            if ( c.typ == TYP_TEXTBOX)
            {                
                if (wParam == 0x56 && bCtrl) // ctrl-V
                {
                    //ClipboardCopyPasteText(hWnd, c, true );
                    r =  1;
                }
                else if (wParam == 0x43 && bCtrl) // ctrl-C
                {
                    //ClipboardCopyPasteText(hWnd, c, false);
                    r = 1;
                }
            }

            if ( r == 0)
                r = D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
        case WM_KILLFOCUS:
		case WM_SETFOCUS:
        case WM_CHAR:
        {
            r =  D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
		case WM_KEYUP:
        case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEWHEEL:
		case WM_LBUTTONDBLCLK:
        {
            r = D2DDefWndProc(hwin, app, message, wParam,lParam);			
        }
        break;

        case WM_MOUSEMOVE:
        {
            r =  D2DDefWndProc(hwin, app, message, wParam,lParam);
        }
        break;
       
		 case WM_D2D_ONIME_ONOFF:
		 {
			D2DDefWndProc(hwin ,app, message, wParam, lParam);

		 }
		 break;
    }

	if ( r == 0 )
		r = ::DefWindowProc(hWnd, message, wParam, lParam);

    if ( app.bRedraw )
        InvalidateRect(hWnd, NULL, FALSE);


    return r;
}


