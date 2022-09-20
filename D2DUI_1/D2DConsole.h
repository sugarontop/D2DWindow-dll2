#pragma once

#include "D2DWindowControl.h"
#include "D2DTextbox.h"
#include "D2DColor.h"
#include "D2DScrollbar.h"

namespace V6
{
	
	class D2DSampleOutput : public D2DControls
	{
		public :
			D2DSampleOutput();
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
	};

	class D2DLineInput : public D2DControls
	{
		public :
			D2DLineInput();
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;


			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
			
			std::wstring str_;
		public :
			void OnCommand(int typ);
	};

	// D2DConsole
	//  ->D2DLineInput
	//    ->D2DSampleOutput


	class D2DConsole : public D2DControls
	{
		public :
			D2DConsole();
			virtual ~D2DConsole(){}
			virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
			virtual void Draw(D2DContext& cxt) override;

			virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1);
		protected :
			float sc_dataHeight();
			float sc_barThumbHeight();
			float sc_barTotalHeight();
			bool  sc_MouseMove(FPointF& pt);
			void sc_barSetBottom();
		private :
			D2DColor back_;
			D2DColor fore_;


			std::shared_ptr<D2DTextbox> input_;
			std::shared_ptr<D2DScrollbar> vbar_;

			float lastpos_;
			float offbar_y_;
			int scstat_;
			float scbarThumbHeight_;
	};



};