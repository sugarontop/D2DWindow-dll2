#include "pch.h"
#include "comptr.h"
#include "StockChart.h"

using namespace V6;

bool StockChart::Load(DataProvider& dp)
{
	DataProviderInfo info;
	DataRpoviderOut ou;

	
	std::vector<CandleData> ar;

	bool ret = false;
	if ( dp.Connect( info ) )
	{
		auto st = dp.LoadSolidData( info );

		if ( dp.Convert( st, &ou ) )
		{
			GenChartData( ou.sm, ar );

			Candle::matrix mat;
			mat._x22 = 1;
			mat._x32 = 0;

			GenChartCandle( ar, mat, xar_);

			

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

void StockChart::GenChartCandle(std::vector<CandleData>& ar, Candle::matrix& mat, std::vector<Candle>& out)
{
	out.clear();

	money max1=0.0f, min1=9999999.0f;

	for(auto& c : ar)
	{
		max1 = max(c.m2, max1);
		min1 = min(c.m4, min1);
	}

	int step = CalcGraphYMaxYMin(&max1, &min1);



	for(auto& c : ar)
	{
		Candle cd;
		cd.raw = c;
		cd.conv(mat);
		out.push_back(cd);
	}
}

void StockChart::TrimCandle(Candle::matrix& mat, std::vector<Candle>& ar)
{
	for(auto& c : ar)
	{
		Candle& cd = c;
		cd.conv(mat);
	}
}

void StockChart::Draw(ID2D1DeviceContext* cxt)
{
	
	
	FRectF rc(10,60,1000,600);

		
	ComPTR<ID2D1SolidColorBrush> br,bgreen,bred,brw;
		
	cxt->CreateSolidColorBrush(D2RGB(0,0,0), &br);
	cxt->CreateSolidColorBrush(D2RGB(0,200,0), &bgreen);
	cxt->CreateSolidColorBrush(D2RGB(200,0,0), &bred);
	cxt->CreateSolidColorBrush(D2RGB(255,255,255), &brw);

	cxt->DrawRectangle(rc, br);
	cxt->FillRectangle(rc, brw);

	float x=rc.right,off=5;
	x -= off;

	for(auto it = xar_.rbegin(); it != xar_.rend(); it++)	
	{			
		if ( x < 0 )
			break;
		
		auto br1 = ( it->vpos[0] < it->vpos[4] ? bgreen : bred);
		
		rc.SetRect( x, 600-it->vpos[0], x+off, 600-it->vpos[4] );
		cxt->FillRectangle(rc, br1);

		x -= off;
	}
}

float tof(const std::string& s)
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


void Candle::conv( matrix& mat )
{
	vpos[0] = mat._x22*raw.m1 + mat._x32;
	vpos[1] = mat._x22*raw.m2 + mat._x32;
	vpos[2] = mat._x22*raw.m3 + mat._x32;
	vpos[3] = mat._x22*raw.m4 + mat._x32;
	vpos[4] = mat._x22*raw.m4ex + mat._x32;
}