#include "pch.h"
#include "D2DDataGridView.h"


using namespace V6;

#define FONT_HEIGHT 11.0f

void D2DDataGridView::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	InnerCreateWindow(parent,pacontrol,stat,name,local_id);

	fore_ = D2RGB(0,0,0);
	back_ = D2RGB(255,255,255);
	rc_ = rc;

	buffer_row_ = 100;
	buffer_col_ = 10;

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

		const float h = FONT_HEIGHT+4.0f,  w = 100.0f;
		FPointF pt(0,0);

		for(auto row=0; row < min(50, buffer_row_); row++ )
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

			mat.Offset(0,h);
		}
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

				for(auto& it : ar )
				{
					GridViewCellValue cv; 

					auto s = (LPCWSTR)it.buffer.c_str();

					cv.typ = GridViewCellValue::TYP::cstring;
					cv.value = (LPVOID)s;
					SetValue(it.row,it.col, cv);




				}

			}
			r = 1;
		}
		break;

		//case WM_D2D_COMMAND_SET:
		//{
		//	//var k3 = dd.create("type=grid&id=10105&x=650&y=50&cx=150&cy=450&nm=gd1");
		//	//k3.set("color=#FFFFA0&bkcolor=#111111");
		//	//k3.set("add&title=ABCDEFG1&val=000000");

		//	if ( (UINT_PTR)this == (UINT_PTR)wParam)
		//	{
		//		LPCWSTR cmdstr = (LPCWSTR)lParam;

		//		auto ar = SplitW(cmdstr,L"&");
		//		std::wstring cmd;
		//		
		//		std::wstring title;
		//		for(auto& it : ar)
		//		{
		//			auto ar2 = SplitW(it.c_str(), L"=");

		//			if ( ar2.size() == 1 )
		//			{
		//				cmd = ar2[0];

		//			}
		//			else if ( ar2.size() == 2)
		//			{
		//				if ( cmd==L"add" && (ar2[0] == L"title") )
		//				{
		//					title = ar2[1];
		//				}	
		//				else if ( cmd==L"add" && (ar2[0] == L"val") )
		//				{
		//					std::wstring& val = ar2[1];

		//					Row r;
		//					r.colcnt = 2;
		//					r.value = new PIDWriteTextLayout[2];

		//					auto& cxt = this->GetParent()->GetContext();

		//					IDWriteTextLayout *playout, *playout2;
		//					if (SOK(cxt.wfactory_->CreateTextLayout(title.c_str(), title.length(), cxt.textformat_, 1000,1000, &playout )))
		//						r.value[0] = playout;
		//					if (SOK(cxt.wfactory_->CreateTextLayout(val.c_str(), val.length(), cxt.textformat_, 1000,1000, &playout2 )))
		//						r.value[1] = playout2;

		//					Items_.push_back(r);
		//				}
		//				else if (ar2[0] == L"color")
		//				{
		//					D2DColor clr(ar2[1].c_str());
		//					fore_ = clr;
		//				}
		//				else if (ar2[0] == L"bkcolor")
		//				{
		//					D2DColor clr(ar2[1].c_str());
		//					back_ = clr;
		//				}
		//			}

		//		}
		//		r = 1;
		//	}


		//}
		//break;

	}
	return r;


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