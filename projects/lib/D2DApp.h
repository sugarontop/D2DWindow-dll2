#pragma once

#include "AppBase.h"
#pragma warning( disable : 4267 )


namespace V6 
{
	class D2DCaptureObject;
	class CLASSEXPORT D2DApp
	{
		public :
			D2DApp(){}
			static D2DApp& GetInstance();
		public :
			virtual void SetCapture(D2DCaptureObject* target)=0;
			virtual D2DCaptureObject* ReleaseCapture()=0;
			virtual bool IsCapture(D2DCaptureObject* target)=0;
			virtual D2DCaptureObject* GetCapture()=0;
			virtual D2DCaptureObject* GetCapture2()=0;
			virtual int Count()=0;
			virtual int IsCaptureEx(D2DCaptureObject* target)=0;

		
	};



};

std::vector<std::wstring> SplitW( LPCWSTR str, LPCWSTR split );

