#pragma once


#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DSimpleListbox.h"
#include "D2DInterface.h"
namespace V6
{
	class D2DDropdownListbox : public D2DControls, public XDropdownListBox
	{
	friend class D2DSimpleListbox;
	public:
		D2DDropdownListbox(){};
		
		virtual FRectF GetRect() const { return rc_; }
		virtual FRectF& GetRectSmooth() override { return rc_; }
		virtual void SetRect(const FRectF& rc) { rc_ = rc; }
		virtual void Draw(D2DContext& cxt);

		virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
		virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		virtual std::wstring GetTreeTyp(USHORT* typ) override;

		virtual int GetTypeid() const override{ return TYP_DROPDOWNLISTBOX; }
		void AddItem( std::wstring text);
	public :		
		virtual void xAddItem(int idx, std::wstring title);
		virtual int xItemCount() ;
		virtual int xGetSelect();
		virtual bool xSetSelect(int idx);
		virtual void xClear();

		std::function<DWORD(void* sender,LPCWSTR funcnm, void* p )> click_;
	protected :
		void OnClick();
		void OnCloseListbox(int selected_idx);
		
		
	protected :
		FRectF button_rc_;
		int selected_idx_;
		FRectF rc_;
		int mouse_mode_;
		float listbox_height_;

		std::shared_ptr<D2DSimpleListbox> ls_;

		std::map<int, std::wstring> str_items_;

		std::shared_ptr<D2DListboxItemBase> selected_item_;
	};
};




