#include "pch.h"
#include "comptr.h"
#include "StockChart.h"
#include "memodb.h"

using namespace V6;


StockChart::StockChart( V6::FRectF rc):vrect_(rc)
{
	

}

void StockChart::SetSize(FSizeF vsz)
{
	vrect_.SetSize(vsz);

}


const char* StockChart::SearchDate( float ptx )
{
	FRectF figure_rc = vrect_.ZeroRect();
	float x=figure_rc.right; 
	const float off=5;
	int typ = 1;	
	
	if ( typ == 0 )
	{
		for(auto it = xar_.rbegin(); it != xar_.rend(); it++)	
		{			
			if ( x-off <= ptx && ptx <= x)
			{
				return (*it).raw.date;
			}
			x -= off;
		}
	}
	else if ( typ == 1 )
	{		
		int n = 1;
		int k = (int)xar_.size();
		
		for(int it = k-1; it >= 0; it-=n )
		{			
			if ( x-off <= ptx && ptx <= x)
			{
				return xar_[it].raw.date;
			}

			n = max(1, (int)((x - ptx ) / off));

			x -= off*n;
		}
	}
	return nullptr;
}


bool StockChart::MouseMove(FPointF pt)
{
	if ( v2money_ && pt.y > 0)
	{
		// pt.y
		mouse_place_value_ = v2money_(vrect_.bottom-pt.y);


		// pt.x
		auto s = SearchDate( pt.x );

		if ( s )
		{
			now_date_ = s;
			return true;
		}
	}
	return false;
}

LPCWSTR StockChart::GetNowValue(money* val)
{
	*val = mouse_place_value_;

	static WCHAR date[64];

	::MultiByteToWideChar(CP_ACP,0,now_date_.c_str(), (int)now_date_.length(), date, 64 );
	
	return date;
}

void StockChart::LoadAsync(DataProvider* pdp, DataProviderInfo* pdpi, std::function<void(void)> complete)
{
	struct X
	{
		DataProvider* pdp;
		DataProviderInfo* pdpi;
		StockChart* sc;
		std::function<void(void)> complete;

		~X()
		{
			delete pdp;
			delete pdpi;
		}


		static DWORD CALLBACK Jackson5(LPVOID p)
		{
			X* x = (X*)p;
			{
				x->sc->Load(*x->pdp, *x->pdpi);

				x->complete();
			}
			delete x;
			return 0;
		};
	};

	X* x = new X();
	x->sc = this;
	x->pdpi = pdpi;
	x->pdp = pdp;
	x->complete = complete;


	DWORD dw;
	::CreateThread(0,0, X::Jackson5, x,0,&dw);
}



bool StockChart::Load(DataProvider& dp, DataProviderInfo& dpi)
{
	DataProviderInfo& info = dpi;
	DataRpoviderOut ou;

	cd_ = info.cd;
	
	std::vector<CandleData> ar;

	bool ret = false;
	if ( dp.Connect( info ) )
	{
		auto st = dp.LoadSolidData( info );

		if ( st.result && dp.Convert( st, &ou ) )
		{
			GenChartData( ou.sm, ar );
			GenChartCandle( ar, xar_, ar_trim_);

			WriteDataToDb(info.cd.c_str(), ar);

			ret = true;
		}
	}
	return ret;
}



void StockChart::GenChartData(IStream* sm, std::vector<CandleData>& ar )
{
	char cb[256];
	ULONG len;

	LARGE_INTEGER li={};
	ULARGE_INTEGER uli;
	if ( S_OK != sm->Seek(li, STREAM_SEEK_SET, &uli))
		return;
	
	while( S_OK == sm->Read(cb,256,&len) && len > 0 )
	{
		char* p = cb;
		ULONG i=0;
		while( i < len &&  *p != '\r' && *p != '\n' )
		{
			++p;
			++i;
		}

		CandleData c;
		if ( c.load(cb, i) )
			ar.push_back(c);


		li.QuadPart += (LONGLONG)(i+1);

		if (*p == '\r')
			li.QuadPart += 1;


		if ( S_OK != sm->Seek(li, STREAM_SEEK_SET, &uli))
			break;
	}
}


void StockChart::GenChartCandle(std::vector<CandleData>& ar,  std::vector<Candle>& out, std::vector<FigureTrimline>& trimout)
{
	out.clear();

	money money_max=0.0f, money_min=9999999.0f;

	for(auto& c : ar)
	{
		if ( 0 < c.m4 )
		{
			money_max = max(c.m2, money_max);
			money_min = min(c.m3, money_min);
		}
	}

	float vymax = vrect_.Height();
	float vymin = 0;

	float yscale = (vymax-vymin)/(money_max-money_min);
	float yoff = 0;

	auto scale_func = [yscale, money_min, yoff](money m1)->float
	{		
		return yscale*(m1-money_min) + yoff;
	};

	auto r_scale_func = [yscale, money_min, yoff](float y)->float
	{		
		return (y - yoff)/yscale+money_min;
	};

	v2money_ = r_scale_func;

	int basean [] = {1,2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000};

	auto trim_f1 = [&basean](float m, int rank)->float
	{
		_ASSERT( rank < 16 );
				
		int* an = basean; // {10,100,1000,10000};

		float af = (float)basean[rank];
		
		int a = (int)(m /(int)af);
		return (float)(a*an[rank]);
	};

	int k = 0;
	std::vector<float> trim;


	for(;;)
	{
		auto max1 = trim_f1(money_max,k);
		auto min1 = trim_f1(money_min,k);
		int cnt = (int)(max1-min1 )/basean[k];

		if ( cnt < 10 )
		{
			min1 += basean[k];

			for(int i= 0; i < cnt; i++ )	
			{
				trim.push_back(min1);
				min1 += basean[k];
			}
			break;
		}
		k++;
	}

	std::vector<FigureTrimline> vtrim;

	for(auto& it : trim)
	{		
		FigureTrimline y;
		y.val = it;
		y.vy = scale_func(y.val);
		vtrim.push_back(y);
	}

	trimout = vtrim;

	for(auto& c : ar)
	{
		if ( 0 < c.m4 )
		{
			Candle cd;
			cd.raw = c;
			cd.conv(scale_func);
			out.push_back(cd);
		}
	}

	money2vpos_ = scale_func;
}


void StockChart::Draw(ID2D1RenderTarget* cxt)
{
	D2DMatrix mat(cxt);
	mat.PushTransform();
	mat.Offset(0, vrect_.top );

	FRectF figure_rc = vrect_.ZeroRect();

		
	ComPTR<ID2D1SolidColorBrush> black,brw;
		
	cxt->CreateSolidColorBrush(D2RGB(0,0,0), &black);
	cxt->CreateSolidColorBrush(D2RGB(255,255,255), &brw);

	cxt->DrawRectangle(figure_rc, black);
	cxt->FillRectangle(figure_rc, brw);

	DrawCandle(cxt);

	DrawTrimline(cxt);

	//DrawParabolic(cxt);

	mat.PopTransform();
}

void StockChart::DrawCandle(ID2D1RenderTarget* cxt)
{
	ComPTR<ID2D1SolidColorBrush> black,bgreen,bred,brw,btrim;
	
	cxt->CreateSolidColorBrush(D2RGB(8,153,129), &bgreen);
	cxt->CreateSolidColorBrush(D2RGB(242,54,69), &bred);
	
	FRectF figure_rc = vrect_.ZeroRect();
	float x=figure_rc.right;
	const float off=5;
	x -= off;

	for(auto it = xar_.rbegin(); it != xar_.rend(); it++)	
	{			
		if ( x < figure_rc.left )
			break;
		
		float* c = (*it).vpos;

		auto br1 = ( c[0] < c[3] ? bgreen : bred);
		
		FRectF rc( x, figure_rc.bottom-c[0], x+off, figure_rc.bottom-c[3] );
		cxt->FillRectangle(rc, br1);

		FRectF rc2( rc.left+2, figure_rc.bottom-c[1], rc.left+3, figure_rc.bottom-c[2] );
		cxt->FillRectangle(rc2, br1);

		x -= off;
	}


}
void StockChart::DrawParabolic(ID2D1RenderTarget* cxt)
{
	// ñ¢äÆê¨,Å@pending

	if ( xar_.size() < 300 ) return;
	
	// calc
	money test = xar_[300].raw.m4 * 0.9;
	auto ypos = money2vpos_(test);


	// draw

	ComPTR<ID2D1SolidColorBrush> black,bgreen,bred,brw,btrim;
	cxt->CreateSolidColorBrush(D2RGB(0,0,0), &black);

	FRectF figure_rc = vrect_.ZeroRect();
	float x=figure_rc.right;
	const float off=5;
	x -= off;
	
	D2D1_ROUNDED_RECT rrc;
	rrc.radiusX = 2;
	rrc.radiusY = 2;

	for(auto it = xar_.rbegin(); it != xar_.rend(); it++)	
	{
		FRectF rc( x, figure_rc.bottom-ypos, x+off, figure_rc.bottom-ypos+off );
		rrc.rect = rc;
		cxt->DrawRoundedRectangle(rrc, black);
		x -= off;
	}
}

void StockChart::DrawTrimline(ID2D1RenderTarget* cxt)
{
	FRectF figure_rc = vrect_.ZeroRect();
	WCHAR cb[64];

	ComPTR<ID2D1SolidColorBrush> btrim;
	cxt->CreateSolidColorBrush(D2RGB(171,171,171), &btrim);
	for(auto& it : ar_trim_)
	{
		FPointF pt1(figure_rc.left,figure_rc.bottom-it.vy);
		FPointF pt2(figure_rc.right,pt1.y);

		cxt->DrawLine(pt1,pt2,btrim );
	}

	const float value_font_height = 10.0f;

	for(auto& it : ar_trim_)
	{
		float y = figure_rc.bottom-it.vy;

		FRectF rc(0,y,1000,y+value_font_height);

		StringCbPrintf(cb,_countof(cb),L"%-8.1f", it.val );

		cxt->DrawText(cb, (UINT32)wcslen(cb), trim_textformat_, rc, btrim);
	}


	FRectF rc(0,figure_rc.top,1000,figure_rc.top+50);
	StringCbPrintf(cb,_countof(cb),L"%s", cd_.c_str() );
	cxt->DrawText(cb, (UINT32)wcslen(cb), money_textformat_, rc, btrim);
}




void StockChart::WriteDataToDb(LPCWSTR cd, const std::vector<CandleData>& ar)
{
	std::vector<MEMODB::StockItem> ar2( ar.size());
	int k = 0;

	for(auto& it :ar)
	{
		MEMODB::StockItem s;
		s.m1 = it.m1;
		s.m2 = it.m2;
		s.m3 = it.m3;
		s.m4 = it.m4;
		s.volume = it.qnt;
		lstrcpyA(s.date, it.date);
		s.typ = MEMODB::StockItem::TYP::DAY;

		ar2[k++] = s;
	}
	
	MEMODB::FileWrite(cd, ar2);
}



inline float tof(const std::string& s)
{
	return (float)atof(s.c_str());
}

bool CandleData::load( LPCSTR csv_row, ULONG len )
{
	char cb[256];

	char* dst = cb;
	const char* p = csv_row;
	ULONG i = 0;

	std::vector<std::string> ar;

	while( i < len)
	{
		dst = cb;
		while(*p != ',' && *p != '\n' && *p !=0) 
		{
			*dst++ = *p++;
			i++;
		}

		*dst = 0;

		ar.push_back(cb);

		++p;
		++i;
	}

	_ASSERT( 7 <= ar.size());

	if ( '0' <= ar[0][0] && ar[0][0] <='9')
	{
		lstrcpyA(date, ar[0].c_str());	
		m1 = tof(ar[1]);
		m2 = tof(ar[2]);
		m3 = tof(ar[3]);
		m4 = tof(ar[4]);
		m4ex = tof(ar[5]);
		qnt= (int)tof(ar[6]);

		return true;
	}
	

	return true;
}


void Candle::conv( std::function<float(float)> func ) // float ysc, float money_min, float yoff )
{
	vpos[0] = func(raw.m1); // ysc*(raw.m1-money_min) + yoff;
	vpos[1] = func(raw.m2); 
	vpos[2] = func(raw.m3); 
	vpos[3] = func(raw.m4); 
	vpos[4] = func(raw.m4ex);
}

//////////////////////////////////////////////////////////////////////////////

#include "HiggsJson.h"

using namespace HiggsJson;


std::wstring IStreamToString(IStream* sm)
{
	char cb[256];

	ULONG cnt = 0;

	std::stringstream asm1;
	while( S_OK == sm->Read(cb,256,&cnt) && cnt > 0)
		asm1.write(cb, cnt);

	std::string xasm1 = asm1.str();

	int len = ::MultiByteToWideChar(CP_UTF8,0,xasm1.c_str(), xasm1.length(), 0,0);
	std::vector<WCHAR> wcb(len);
	::MultiByteToWideChar(CP_UTF8,0,xasm1.c_str(), xasm1.length(), &wcb[0], len);

	return std::wstring(&wcb[0],len);
}

bool PrimeStockDataLoad( std::map<std::wstring,PrimeStockDataItem>& mapStockData)
{
	WCHAR cb[256];

	std::wstring cds=L"";

	int i = 0;
	for(auto& s : mapStockData )
	{
		if (i!=0)
			cds += L',';
		
		cds += s.first;
		i++;		
	}
	
	StringCbPrintfW(cb,256,L"https://query1.finance.yahoo.com/v7/finance/quote?symbols=%s",cds.c_str());

	std::function<void(IStream*)> cmpl = [&](IStream* sm){

		//WCHAR* ParseList( LPCWSTR str, std::vector<Higgs>& ar );
		
		std::map<std::wstring,Higgs> m;

		std::wstring src = IStreamToString(sm);

		WCHAR* p1 = ParseMap( src.c_str(), m );

		auto h = m[L"quoteResponse"];

		std::map<std::wstring,Higgs> m2;
		ParseMap( h.head, m2 );

		auto h2 = m2[L"result"];

		std::vector<Higgs> results;
		ParseList( h2.head, results);


		for(auto& it : results )
		{
			std::map<std::wstring,Higgs> m3;
			ParseMap(it.head, m3);

			auto h4 = m3[L"regularMarketPrice"];
			auto h5 = m3[L"symbol"];


			std::wstring s4(h4.head, h4.len);


			std::wstring cd = ToStr(h5);
			money m4 = (money)_wtof(s4.c_str());


			PrimeStockDataItem k;
			k.cd = cd;
			k.regularMarketPrice = m4;

			mapStockData[cd] = k;
		}
	};

	InetDataProvider::GetJson(cb, cmpl);


	return true;
}


