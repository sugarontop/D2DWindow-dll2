#include "pch.h"
#include "comptr.h"
#include "StockChart.h"

using namespace V6;




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
	
	std::vector<CandleData> ar;

	bool ret = false;
	if ( dp.Connect( info ) )
	{
		auto st = dp.LoadSolidData( info );

		if ( dp.Convert( st, &ou ) )
		{
			GenChartData( ou.sm, ar );


			GenChartCandle( ar, xar_);

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



int CalcGraphYMaxYMin(money* pmax1, money* pmin1)
{
	money& _max = *pmax1;
	money& _min = *pmin1;

	_ASSERT( _min < _max );

	auto k = (int)log10(_max-_min);

	int step = pow(10, k);

	_max = _max + step;
	_min = _min - step;

	return step;
}

void StockChart::GenChartCandle(std::vector<CandleData>& ar,  std::vector<Candle>& out)
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


	int basean [] = {1,2,5,10,20,50,100,200,500,1000,2000,5000,10000,20000,50000};

	auto trim_f1 = [&basean](float m, int rank)->float
	{
		_ASSERT( rank < 16 );
				
		int* an = basean; // {10,100,1000,10000};

		float af = (float)basean[rank];
		
		int a = m /(int)af;
		return (float)(a*an[rank]);
	};

	int k = 0;
	std::vector<float> trim;


	for(;;)
	{
		auto max1 = trim_f1(money_max,k);
		auto min1 = trim_f1(money_min,k);
		int cnt = (max1-min1 )/basean[k];

		if ( cnt < 10 )
		{
			for(int i= 0; i < cnt; i++ )	
			{
				trim.push_back(min1);
				min1 += basean[k];
			}
			break;
		}
		k++;
	}






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
}


void StockChart::Draw(ID2D1DeviceContext* cxt)
{
	FRectF figure_rc(30,80, vrect_.Size());

		
	ComPTR<ID2D1SolidColorBrush> br,bgreen,bred,brw;
		
	cxt->CreateSolidColorBrush(D2RGB(0,0,0), &br);
	cxt->CreateSolidColorBrush(D2RGB(0,180,0), &bgreen);
	cxt->CreateSolidColorBrush(D2RGB(180,0,0), &bred);
	cxt->CreateSolidColorBrush(D2RGB(255,255,255), &brw);

	cxt->DrawRectangle(figure_rc, br);
	cxt->FillRectangle(figure_rc, brw);

	float x=figure_rc.right, off=5;
	x -= off;

	for(auto it = xar_.rbegin(); it != xar_.rend(); it++)	
	{			
		if ( x < figure_rc.left )
			break;
		
		float* c = (*it).vpos;

		auto br1 = ( c[0] < c[3] ? bgreen : bred);
		
		FRectF rc( x, figure_rc.bottom-c[0], x+off, figure_rc.bottom-c[3] );
		cxt->FillRectangle(rc, br1);

		x -= off;
	}
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
		qnt= tof(ar[6]);

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