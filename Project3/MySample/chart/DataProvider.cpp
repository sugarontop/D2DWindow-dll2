#include "pch.h"
#include "StockChart.h"
#include "inet.h"


ULONG UnixTime(int yyyy, int mm, int dd );
bool StreamDuplicate( IStream* src, IStream** out);
bool StreamToFile(LPCWSTR dstFileName, IStream* src);

bool InetDataProvider::Connect(DataProviderInfo& info)
{
	InternetInfo* pi = CreateInternetInfo( );

	auto dds = UnixTime(2021,1,1);
	auto now = UnixTime(0,0,0); // <--today


	if (info.interval == L"1wk")
	{
		dds = UnixTime(2014,1,1); // <--tekitou
	}


	WCHAR cb[256];

	auto cd = info.cd; // L"SPY";
	auto interval = info.interval; // 1d or 1wk


	//StringCbPrintfW(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=1d&events=history&includeAdjustedClose=true",cd.c_str(),dds, now);
	StringCbPrintfW(cb,256,L"https://query1.finance.yahoo.com/v7/finance/download/%s?period1=%d&period2=%d&interval=%s&events=history&includeAdjustedClose=true",cd.c_str(),dds, now, interval.c_str());

	pi->bGet = true;
	pi->url = ::SysAllocString(cb);
	pi->postdata = nullptr;

	info.info = pi;

	return true;
}
DataProviderResult InetDataProvider::LoadSolidData(DataProviderInfo& info)
{
	DWORD dw;

	InternetInfo* pi = (InternetInfo*)info.info;

	HANDLE h = ::CreateThread(0,0,InetAsync, (LPVOID)pi, 0, &dw);

	::WaitForSingleObject(h, INFINITE);

	DataProviderResult ret;

	ret.result = (200 == pi->result);
	ret.info = info.info;

	return ret;
}

bool InetDataProvider::Convert(DataProviderResult& res, DataRpoviderOut* out)
{
	InternetInfo* info = (InternetInfo*)res.info;

	out->sm = info->pstream;


	//StreamToFile(L"test.txt", info->pstream);

	DeleteInternetInfo(info);

	return true;
}

/////////////////////////////////////////////////////////////////////
bool FileDataProvider::Connect(DataProviderInfo& info)
{
	static LPCWSTR fnm = L"test.txt";
	info.info = (LPVOID)fnm;
	
	return true;
}
DataProviderResult FileDataProvider::LoadSolidData(DataProviderInfo& info)
{
	DataProviderResult ret;
	ret.result = false;

	LPCWSTR fnm = (LPCWSTR)info.info;
	auto h = ::CreateFile2(fnm, GENERIC_READ,FILE_SHARE_READ,OPEN_EXISTING, nullptr);

	if ( h != INVALID_HANDLE_VALUE)
	{
		DWORD len;
		char cb[256];


		ComPTR<IStream> is;
		auto r = CreateStreamOnHGlobal(NULL,FALSE,&is );

		while(::ReadFile(h, cb,256,&len,nullptr) && len > 0)
		{
			ULONG dd;
			is->Write(cb, 256, &dd);
		}
		::CloseHandle(h);

		is.AddRef();
		ret.out = is;
		ret.result = true;
		ret.info = nullptr;
	}

	return ret;
}
bool FileDataProvider::Convert(DataProviderResult& res, DataRpoviderOut* out)
{
	out->sm = res.out;
	
	return true;
}















#define _SECOND ((__int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

ULONG UnixTime(int yyyy, int mm, int dd )
{
	if ( yyyy < 1970 )
	{
		struct tm tm_now;
		__time64_t long_time;
		_time64( &long_time );		
		localtime_s(&tm_now, &long_time );

		yyyy = 1900+ tm_now.tm_year;
		mm = 1+ tm_now.tm_mon;
		dd = tm_now.tm_mday;
	}


	const ULONG oneday = 86400; // 86400sec

	SYSTEMTIME st={};
	SYSTEMTIME st2={};

	st.wYear=1970;st.wMonth=1;st.wDay=1;

	st2.wYear=yyyy;st2.wMonth=mm;st2.wDay=dd;

	FILETIME ft,ft2;
	SystemTimeToFileTime(&st,&ft);
	SystemTimeToFileTime(&st2,&ft2);

	ULONGLONG number = (((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime;
	ULONGLONG number2 = (((ULONGLONG)ft2.dwHighDateTime) << 32) + ft2.dwLowDateTime;

	auto distance = (number2-number)/_DAY;
	auto ret = distance*oneday;


	return (ULONG)ret;

}
bool StreamDuplicate( IStream* src, IStream** out)
{
	ComPTR<IStream> is;
	auto r = CreateStreamOnHGlobal(NULL,FALSE,&is );
	
	DWORD len;
	char cb[256];

	while( S_OK == src->Read(cb,256,&len) && len > 0)
	{
		DWORD dd;
		is->Write(cb, len, &dd);
	}

	is.AddRef();
	*out = is;

	return true;
}

bool StreamToFile(LPCWSTR dstFileName, IStream* src)
{
	auto h = ::CreateFile2(dstFileName, GENERIC_WRITE,FILE_SHARE_WRITE,CREATE_NEW, nullptr);

	if ( h != INVALID_HANDLE_VALUE)
	{
		DWORD len;
		char cb[256];


		ULARGE_INTEGER dd;
		LARGE_INTEGER d={};
		src->Seek(d,STREAM_SEEK_SET,&dd);

		while( S_OK == src->Read(cb,256,&len) && len > 0)
		{
			DWORD dd;
			::WriteFile(h, cb, 256, &dd, nullptr);
		}
		::CloseHandle(h);
	}
	

	return ( h != INVALID_HANDLE_VALUE);
}
