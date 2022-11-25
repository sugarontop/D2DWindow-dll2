#include "pch.h"
#include "FD2DFrame.h"
#include "D2D1UI_1.h"
using namespace V6;

void FD2DFrame::SizeArrange(UINT cx, UINT cy)
{
	std::vector<std::shared_ptr<FD2DFrameItem>> y_sort(ar_);

	std::sort(y_sort.begin(), y_sort.end(), [](auto a, auto b)->bool{
			
		if ( (*a).row_ == (*b).row_ )
			return (*a).col_ < (*b).col_;

		return (*a).row_ < (*b).row_;
	});

	struct TopHeight
	{
		int t;
		int th;
	};

		
	std::map<int, TopHeight> mh;
	int j = -1;
	int lasty = 0; 
	int h=0;
	for(auto& it : y_sort )
	{
		FRectF rc = D2DGetRect(it->prc_);

		if (j!=it->row_)
		{
			mh[it->row_].t = lasty;

			if ( it->row_ == LASTRC )
				mh[it->row_].th = cy-lasty;
			else
				mh[it->row_].th = rc.Height();

			j = it->row_;
			lasty += rc.Height();
		}
	}

	int pry = 0;
	int left = 0;
		
	for(auto& it : y_sort )
	{
		FRectF rc = D2DGetRect(it->prc_);

		left = ( it->row_ != pry ? 0 : left);
		
		int w = rc.right-rc.left;

		rc.top = mh[it->row_].t;
		rc.bottom = rc.top + mh[it->row_].th;
		rc.left = left;
		rc.right = rc.left + w;

		pry = it->row_;
		left += (rc.right-rc.left);


		if ( it->row_ == LASTRC)
			rc.bottom = cy;

		if ( it->col_ == LASTRC)
			rc.right = cx;


//??		D2DSetRect(it->prc_, rc);
	}




}