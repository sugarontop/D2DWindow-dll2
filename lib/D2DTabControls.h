
#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6 {
class D2DTabControls : public D2DControls
{
	public :
		D2DTabControls(){};	
		virtual void Draw(D2DContext& cxt) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		virtual std::wstring GetTreeTyp(USHORT* typ) override;
		virtual void SetText(LPCWSTR str) override;
		virtual void SetRect(const FRectF& rc) override;

		float DrawTab(D2DContext& cxt, USHORT tabidx);
		D2DControl* GetControlFromIdx(USHORT idx);

		D2DControls* AddNewTab(LPCWSTR tabnm);
		void DelTab(USHORT idx);
		void SizeFix(){ size_fix_ = true;};
		void ExportWndMessage(std::function<LRESULT(UINT,WPARAM,LPARAM)> fn){export_wnd_prc_ = fn;}
	protected :
		
		std::vector<FRectF> tabrects_;
		USHORT tab_idx_;
		bool size_fix_;
		D2DColor back_,fore_;

		D2DMat mat2_, mat_tab_;
		USHORT tab_typ_;
		std::function<LRESULT(UINT,WPARAM,LPARAM)> export_wnd_prc_;

};




};
