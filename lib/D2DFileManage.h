#pragma once
#include "D2DApp.h"
#include "D2DCapture.h"
#include "D2DWindowControl.h"
namespace V6 {

class D2DFileManage;

class BOne
{
	public :
		BOne():nn_(0),bOpen_(false){};
		BOne(LPCWSTR txt,LPCWSTR dir):nn_(0),dir_(dir),bOpen_(false){}; // text_ = ::SysAllocString(txt);}
		
		virtual ~BOne(){};

		virtual void Draw(D2DContext& cxt, D2DMatrix& mat, ID2D1Bitmap** img);
		void SetText(LPCWSTR txt){lstrcpy(info_.cFileName, txt);  }

		virtual bool OnClick(const FPointF& pt);
		virtual bool OnDblClick(const FPointF& pt);
		virtual UINT ChildCount(){ return 1;}
		virtual bool operator < (BOne& a );

		bool bOpen_;
		std::wstring dir_;
		WIN32_FIND_DATA info_;

		static std::function<int(BOne*)> click_;

	protected :
		WORD nn_;
		D2DMat mat_;
		ComPTR<IDWriteTextLayout> textlayout_; 
};
class BOnes : public BOne
{
	public :
		BOnes(){}
		BOnes(LPCWSTR dir,LPCWSTR parent_dir):BOne(dir,parent_dir){}
		virtual void Draw(D2DContext& cxt, D2DMatrix& mat, ID2D1Bitmap** img) override;
		
		virtual bool OnClick(const FPointF& pt) override;

		float Height() const{ return height_;}
		virtual UINT ChildCount();
		virtual bool OnDblClick(const FPointF& pt);
		virtual bool operator < (BOnes& a );

		void clear();

		std::vector<std::shared_ptr<BOne>> ar_;
		float height_;

		std::wstring fullname_;
	protected:
		

};

class D2DFileManage : public D2DControls
{
public :
	D2DFileManage(){};	

	enum bmp { folder=0, file=1 };

	virtual void Draw(D2DContext& cxt) override;
	virtual LRESULT WndProc(AppBase& b, UINT message, WPARAM wParam, LPARAM lParam) override;
	virtual void CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id = -1) override;
	virtual std::wstring GetTreeTyp(USHORT* typ) override;
	
	void make_root( LPCWSTR root_dir );
	int RootChange(std::wstring dir);

	static std::function<void(std::wstring)> OnClick_;
	
	enum TYP { SOLO, TREE };
	void ChangeTyp(TYP ty){ typ_ = ty; }
	
protected :
	std::shared_ptr<BOnes> root_;
	ComPTR<ID2D1Bitmap> bmp_[2];
	TYP typ_;
	
	
};

};

