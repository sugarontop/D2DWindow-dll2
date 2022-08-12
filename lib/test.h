#pragma once


#include "D2DApp.h"
#include "D2DWindowControl.h"

namespace V6 
{


	class CLASSEXPORT Test
	{
		public :
			Test();
			~Test(){}
			virtual void CreateControl(D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1)
			{

				
			}
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) 
			{

				return 0;
			}
			virtual void Draw(D2DContext& cxt)
			{


			}

			//std::vector<int> ar;
			void* p;

	};

};
