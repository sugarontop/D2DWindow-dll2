#pragma once
#include "D2DTextbox.h"


namespace V6
{
	class D2DConsole : public D2DTextbox
	{
		public :
			D2DConsole();
			virtual ~D2DConsole(){}

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);





	};



};