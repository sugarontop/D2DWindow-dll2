#pragma once

#include "D2DApp.h"
#include "D2DWindowControl.h"
#include "D2DColor.h"
namespace V6
{
	
	struct GridViewCellValue
	{
		enum class TYP {inumber, fnumber, cstring, raw};	

		TYP  typ;
		void* value; // link;
	};

	struct Tool
	{
		IDWriteTextFormat* format;
		IDWriteFactory* factory;
	};


	struct Cell
	{
		Cell():t(nullptr){}
		~Cell(){ clear(); }

		void clear()
		{
			if (t) t->Release();
			t = nullptr;
		}

		void set( Tool& tool, const GridViewCellValue& v)
		{
			WCHAR cb[256];

			clear();

			if ( v.typ == GridViewCellValue::TYP::inumber )
			{
				int& a = *(int*)v.value;
				StringCbPrintf(cb,_countof(cb), L"%d", a);
				tool.factory->CreateTextLayout(cb, wcslen(cb), tool.format, 1000,1000, &t );
			}
			else if ( v.typ == GridViewCellValue::TYP::fnumber )
			{
				float& a = *(float*)v.value;
				StringCbPrintf(cb,_countof(cb), L"%f", a);
				tool.factory->CreateTextLayout(cb, wcslen(cb), tool.format, 1000,1000, &t );

			}
			else if ( v.typ == GridViewCellValue::TYP::cstring )
			{
				LPCWSTR a = (LPCWSTR)v.value;
				StringCbPrintf(cb,_countof(cb), L"%s", a);
				tool.factory->CreateTextLayout(cb, wcslen(cb), tool.format, 1000,1000, &t );
			}
			else if ( v.typ == GridViewCellValue::TYP::raw )
			{
				IDWriteTextLayout* t1 = (IDWriteTextLayout*)v.value;
				t1->AddRef();
				t = t1;
			}
		}

		IDWriteTextLayout* t;
	};
	
	struct CellRC
	{
		CellRC():row(0),col(0),pointer(nullptr){};
		CellRC( void* s ):row(0),col(0),pointer(s){};

		void* pointer;
		WORD row;
		WORD col;
		std::wstring buffer;

	};

	
	class D2DDataGridView : public D2DControls
	{
		public :
			D2DDataGridView(){};
			virtual ~D2DDataGridView();


			void SetValue(UINT row, UINT col, const GridViewCellValue& cv);

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			virtual FRectF GetRect() const { return rc_; }
			virtual void SetRect(const FRectF& rc){ rc_ = rc; }
			virtual void Draw(D2DContext& cxt);
			virtual FRectF& GetRectSmooth(){ return rc_; };
			void Clear();
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
		protected :
			void Expand(WORD row, WORD col);
			bool ParseValues(LPCWSTR values, std::vector<CellRC>& ar);
			void SetViewMaxSize(FSizeF sz);
		protected :
			FRectF rc_;

			Cell* table_;

			WORD buffer_row_;
			WORD buffer_col_;


			Tool tool_;

			UIHandle hndl_;
			UINT vrow_top_;
			UINT row_count_;
			float vscroll_x_;
			std::shared_ptr<D2DScrollbar> vscbar_;

			D2DColor fore_,back_;
	};



};