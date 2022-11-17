// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"

static TP_CALLBACK_ENVIRON poolCBEnviron;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		// 環境整備
		//TP_CALLBACK_ENVIRON poolCBEnviron;
		InitializeThreadpoolEnvironment(&poolCBEnviron);
		auto pPool = CreateThreadpool(NULL);
		SetThreadpoolThreadMinimum(pPool, 1);
		SetThreadpoolThreadMaximum(pPool, 5);
		SetThreadpoolCallbackPool(&poolCBEnviron, pPool);
	}
	break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// ::CreateThreadの代わりに
HANDLE MyCreateThread(int,int,LPTHREAD_START_ROUTINE th, LPVOID prm, DWORD, DWORD* ret)
{
	struct X
	{
		LPTHREAD_START_ROUTINE th_;
		LPVOID parameter;
		HANDLE eventHandle_;

		static void Jackson5(PTP_CALLBACK_INSTANCE Instance,PVOID prm,PTP_WORK Work)
		{	
			X* x = (X*)prm;
			x->th_(x->parameter);
			

			SetEventWhenCallbackReturns(Instance, x->eventHandle_);

			CloseThreadpoolWork(Work);
			delete x;
		}
	};

	HANDLE h = CreateEvent(NULL, TRUE, FALSE, NULL);
	X* x = new X();
	x->th_ = th;
	x->parameter = prm;
	x->eventHandle_ = h;

	SubmitThreadpoolWork(CreateThreadpoolWork(X::Jackson5, x, &poolCBEnviron));

	*ret = 0;
	return h;
}