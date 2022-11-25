#pragma once

#include "D2DMessage.h"


namespace V6 {

#define LASTRC	99


struct FD2DFrameItem
{
	FD2DFrameItem(DWORD yrow, DWORD xcol,  UIHandle prc):row_(yrow),col_(xcol),prc_(prc){}

	UIHandle prc_; 
	DWORD row_,col_;
};

class FD2DFrame
{
	public :		
		FD2DFrame(){};

		void Add(DWORD yrow, DWORD xcol,  UIHandle prc)
		{
			ar_.push_back(std::make_shared<FD2DFrameItem>(yrow,xcol,prc));

		}


		void SizeArrange(UINT cx, UINT cy);


		std::vector<std::shared_ptr<FD2DFrameItem>> ar_;
};






};
