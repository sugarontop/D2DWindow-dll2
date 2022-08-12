#pragma once


#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"

namespace V6
{
	class D2DListboxItemBase
	{
		public :
			D2DListboxItemBase() : idx_(-1){}
			D2DListboxItemBase(int idx): idx_(idx){}
			
			virtual bool Draw(D2DContext& cxt, float width, float height) = 0;
			virtual float RowHeight() = 0;
			virtual float ItemWidth() =0;
		protected :
			int idx_;
	};

	class D2DListboxItemString : public D2DListboxItemBase
	{
		public :
			D2DListboxItemString():width_(0){}
			D2DListboxItemString(int idx, const std::wstring& title):D2DListboxItemBase(idx), title_(title),width_(0){}
			~D2DListboxItemString(){ Clear(); }
			virtual bool Draw(D2DContext& cxt, float width, float height) override;
			virtual float RowHeight() override;
			void Clear();
			virtual float ItemWidth() { return width_; }
		protected :			
			float width_;
			std::wstring title_;
			ComPTR<IDWriteTextLayout> layout_;

	};

	class D2DListboxItemImage : public D2DListboxItemBase
	{
		public :
			D2DListboxItemImage(){}
			D2DListboxItemImage(int idx,ComPTR<ID2D1Bitmap> item):D2DListboxItemBase(idx),img_(item){}
			virtual bool Draw(D2DContext& cxt, float width, float height) override;
			virtual float RowHeight() override;
			virtual float ItemWidth(){ return 0; }
		protected :			
			ComPTR<ID2D1Bitmap> img_;
	};
	class D2DListboxItemControl : public D2DListboxItemBase
	{
		public :
			D2DListboxItemControl(){}
			D2DListboxItemControl(int idx,std::shared_ptr<D2DControl> item):D2DListboxItemBase(idx),ctrl_(item){}
			virtual bool Draw(D2DContext& cxt, float width, float height) override;
			virtual float RowHeight() override;
			virtual float ItemWidth(){ return 0; }
			std::shared_ptr<D2DControl> Control(){ return ctrl_; }
		protected :			
			std::shared_ptr<D2DControl> ctrl_;
	};



	class D2DSimpleListbox : public D2DControl
	{
		friend class D2DDropdownListbox;
		
		public:
			D2DSimpleListbox();

			virtual FRectF GetRect() const { return rc_; }
			virtual FRectF& GetRectSmooth() override { return rc_; }
			virtual void SetRect(const FRectF& rc) { rc_ = rc; }
			virtual void Draw(D2DContext& cxt);
			void Clear();

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;

			int GetSelectedIdx() const { return selected_idx_; }
			virtual int GetTypeid() const override{ return TYP_SIMPLE_LISTBOX; }
			virtual std::wstring GetTreeTyp(USHORT* typ) override;
			
			float RowHeight() const;
			void AddItem( const std::wstring& str);
			void AddBitmapItem(ID2D1Bitmap* bmp);
		protected :
			LRESULT WndProcNormal(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) ;
			LRESULT WndProcForControl(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) ;
	protected:

			bool OnEscape();
			void OnClick();

		
			bool sc_MouseMove(FPointF& pt);
			float sc_barThumbHeight();
			float sc_barTotalHeight();
			float sc_dataHeight();


			bool wsc_MouseMove(FPointF& pt);

		protected :
			int selected_idx_;
			FRectF rc_;
			int mouse_stat_;
			float offbar_y_, offbar_x_;
			int float_idx_;
			int scstat_;
			USHORT typ_;

			float scbarThumbHeight_;
			float scdata_;
			float scbai_;

			float hscWidth_;

			std::vector< std::shared_ptr<D2DListboxItemBase> > items_;
			D2DColor border_clr_,back_clr_,float_clr_,select_clr_;
	};

};