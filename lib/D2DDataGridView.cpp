#include "pch.h"
#include "D2DScrollbar.h"
#include "D2DDataGridView.h"


using namespace V6;

#define FONT_HEIGHT 11.0f
#define ROW_HEIGHT (FONT_HEIGHT+4.0f)
#define HEADER_HEIGHT 0.0f
#define TAB_BAR_HEIGHT 0

void D2DDataGridView::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	fore_ = ColorF::Black;
	back_ = ColorF::White;

	rc_ = rc;

	buffer_row_ = 100;
	buffer_col_ = 10;
	row_count_ = 0;

	table_ = new Cell[buffer_row_*buffer_col_];
}
D2DDataGridView::~D2DDataGridView()
{
	Clear();
}
void D2DDataGridView::Expand(WORD row, WORD col)
{
	if ( row >= buffer_row_ )
	{
		WORD new_buffer_row_ = 0;
		while( row >= new_buffer_row_ )
		{
			new_buffer_row_ += buffer_row_ * 2;
		}

		Cell* newtable = new Cell[new_buffer_row_*buffer_col_];

		memcpy(newtable, table_, sizeof(Cell)*buffer_row_*buffer_col_);

		for(UINT i=0; i < (UINT)buffer_row_*buffer_col_; i++)
		{
			if (table_[i].t)
				table_[i].t->AddRef();
		}

		delete [] table_;

		table_ = newtable;

		buffer_row_ = new_buffer_row_;
	}

	if ( col >= buffer_col_ )
	{
		WORD new_buffer_col_ = 0;
		while( col >= new_buffer_col_ )
		{
			new_buffer_col_ += buffer_col_ * 2;
		}

		Cell* newtable = new Cell[buffer_row_*new_buffer_col_];

		memcpy(newtable, table_, sizeof(Cell)*buffer_row_*buffer_col_);
		
		for(UINT i=0; i < (UINT)buffer_row_*buffer_col_; i++)
		{
			if (table_[i].t)
				table_[i].t->AddRef();
		}

		delete [] table_;

		table_ = newtable;
		
		buffer_col_ = new_buffer_col_;
	}

}

void D2DDataGridView::SetValue(UINT row, UINT col, const GridViewCellValue& cv)
{
	// 1 start
	_ASSERT( row > 0 && col > 0 );
	
	// 0 start
	row--;
	col--;
	
	Expand(row,col);

	_ASSERT ( row < buffer_row_ && col < buffer_col_ );

	Cell& pc = table_[buffer_col_*row + col];
		
	pc.set(tool_, cv);


}

void D2DDataGridView::Clear()
{
	delete [] table_;

	table_ = nullptr;
	buffer_row_=0;
	buffer_col_=0;

	if ( tool_.format )
	{
		tool_.factory->Release();
		tool_.format->Release();
		tool_.factory = nullptr;
		tool_.format = nullptr;
	}
}

void D2DDataGridView::Draw(D2DContext& cxt)
{
	if (BITFLG(STAT_VISIBLE))
    {		
		ComPTR<ID2D1SolidColorBrush> back;
		ComPTR<ID2D1SolidColorBrush> fore;
		
		(*cxt)->CreateSolidColorBrush(fore_, &fore);	
		(*cxt)->CreateSolidColorBrush(back_, &back);



		D2DMatrix mat(*cxt);
        mat.PushTransform();

        D2DRectFilter fil(cxt, rc_ );
		(*cxt)->FillRectangle(rc_,back);

        mat_ = mat.PushTransform();
        mat.Offset(rc_);

		mat.PushTransform();	

			const float h = ROW_HEIGHT,  w = 100.0f;
			FPointF pt(0,0);

			vrow_top_ =  (int)(vscbar_->LogicalOffset() / ROW_HEIGHT+0.5f);

			UINT ir = 0;

			for(auto row=vrow_top_; row < buffer_row_; row++ )
			{
				mat.PushTransform();
				for(auto col=0; col < buffer_col_; col++ )
				{
					auto& cell = table_[row*buffer_col_ + col];

					if ( cell.t )
						(*cxt)->DrawTextLayout(pt,cell.t,fore, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP);

					mat.Offset(w, 0);
				}
				mat.PopTransform();


				if ( (++ir)*ROW_HEIGHT > rc_.Height() )
					break;

				mat.Offset(0,h);
				
			}
		mat.PopTransform();


		mat.PushTransform();	
			mat.Offset(vscroll_x_, HEADER_HEIGHT );
			vscbar_->Draw2(cxt);
		mat.PopTransform();


		mat.PopTransform();
	}
}
LRESULT D2DDataGridView::WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT r = 0;
	
	switch(message)
	{
		case WM_D2D_CREATE:
		{
			if ( S_OK == (DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**) &tool_.factory)))
			{				
				tool_.factory->CreateTextFormat(L"MS –¾’©", NULL, DWRITE_FONT_WEIGHT_REGULAR,
					DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,FONT_HEIGHT, LOCALE, &tool_.format);
			}


			hndl_ = *(UIHandle*)lParam;		

			auto scV = std::make_shared<D2DScrollbar>();
			scV->CreateControl(parent_window_, this, FRectF(0,0,BARW,200), STAT_DEFAULT, L"VSCBAR");
			this->Add(scV);
			vscbar_ = scV;

			vrow_top_ = 0;
			vscroll_x_ = 0;

			r = 1;
		}
		break;
		case WM_D2D_DESTROY:
		{
			delete this;
			r = 1;
		}
		break;
		case WM_D2D_SET_GRIDVIEW_VALUE:
		{
			if ( wParam == 0 )
			{
				LPCWSTR values = (LPCWSTR)lParam;

				std::vector<CellRC> ar;
				
				ParseValues( values, ar );

				row_count_ = 0;
				for(auto& it : ar )
				{
					GridViewCellValue cv; 

					auto s = (LPCWSTR)it.buffer.c_str();

					cv.typ = GridViewCellValue::TYP::cstring;
					cv.value = (LPVOID)s;
					SetValue(it.row,it.col, cv);

					row_count_ = it.row;
				}								
			}
			else if ( wParam == 1 )
			{
				FSizeF sz;
				sz.height = (row_count_ ) * ROW_HEIGHT;
				SetViewMaxSize(sz);
			}

			r = 1;
		}
		break;
		case WM_MOUSEWHEEL:
		{
			MouseParam* mp = (MouseParam*)lParam;
            auto pt = mat_.DPtoLP(mp->pt);
            auto md = (rc_.PtInRect(pt) ? 1 : 0);

            if ( md == 1 )
            {
				r = InnerWndProc(b,message,wParam,lParam);

				if( r == 0)
				{
					float a = 0;
					if (mp->zDelta > 0)
						a = -ROW_HEIGHT;
					else if (mp->zDelta < 0)
						 a= ROW_HEIGHT;

					vscbar_->Offset(a);
				}
				r = 1;
            }
		}
		break;
	}

	if ( r == 0 )
		r = DefWndProc(b,message,wParam,lParam);
		//r = InnerWndProc(b,message,wParam,lParam);

	return r;


}
void D2DDataGridView::SetViewMaxSize(FSizeF all_data_view_sz)
{				
	//_ASSERT((0 < sz.width) && (0 < sz.height));
	_ASSERT((0 < all_data_view_sz.height));

	vscroll_x_ = rc_.Width()-BARW;
	//hscroll_x_ = 0;

	vscbar_->SetStat(STAT_DEFAULT);
	//sch_->SetStat(STAT_DEFAULT);

	vscbar_->SetMaxSize( all_data_view_sz.height );
	//sch_->SetMaxSize( sz.width );
	//sch_->SetSize(rc_.Size());


	FSizeF tsz(rc_.Size());
	tsz.height -= (HEADER_HEIGHT + TAB_BAR_HEIGHT);
	vscbar_->SetSize(tsz);

	//if (sz.width <= rc_.Size().width)
	//	sch_->SetStat(0);
	if (all_data_view_sz.height <= rc_.Size().height-HEADER_HEIGHT)
		vscbar_->SetStat(0);
	
}
bool D2DDataGridView::ParseValues(LPCWSTR values, std::vector<CellRC>& ar)
{
	CellRC cell;

	auto a = SplitW(values, L"&");


	auto Str2RC = [](LPCWSTR rc, WORD& row, WORD& col)
	{
		_ASSERT(rc[0] <= L'Z');

		col = rc[0]-L'A'+1;

		LPCWSTR s = (rc+1);
		row = _wtoi(s);
	};


	for(auto& aa : a)
	{
		auto b = SplitW(aa.c_str(), L"=");

		if ( b.size() == 2 )
		{
			auto rc = b[0].c_str();
			auto val = b[1];

			WORD row,col;
			Str2RC(rc, cell.row, cell.col );

			cell.buffer = val;
			
			ar.push_back(cell);
		}
	}

	return true;
}