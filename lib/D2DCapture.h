#pragma once

#include "AppBase.h"

#include "D2DMessage.h"



#define BITFLG(x)	((stat_&x)==x)
#define BITFLG2(a, x) ((a&x)==x)
namespace V6
{
	class D2DContext;
	class D2DCaptureObject
	{
	private :
		//D2DCaptureObject* parent1_; // <-- �g�p���ĂȂ�
	protected :
		DWORD stat_;
		
	public :
		D2DCaptureObject():stat_(STAT_VISIBLE){}

		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) = 0;
		//virtual D2DCaptureObject* GetParent(){ return parent1_; }
		virtual bool OnChangeFocus(bool bActive, D2DCaptureObject* pnew) { return false; }


		virtual void OnGraphicsResources(D2DContext& cxt, bool bDestroyOrCreate){};
		virtual bool IsDialog() const { return BITFLG(STAT_MODAL); }
		
		virtual void DestroyEx(){Destroy(); }
		virtual int GetTypeid() const { return TYP_NULL; }
		 

		DWORD GetStat() const { return stat_; }
		void Destroy(){ stat_ = STAT_DEAD; }
		void Enable(bool bEnable=true) { stat_ = (bEnable ? (STAT_ENABLE|stat_) : (stat_&~STAT_ENABLE)); }
	};


};
