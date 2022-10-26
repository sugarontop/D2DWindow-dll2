#include "pch.h"
#include "memodb.h"

namespace MEMODB{


struct XHandle
{
	DWORD magic;	
	DWORD row_size;
};

static FILETIME _Date2Number(LPCWSTR date);

#define MAGICNUMBER 6661

bool FileWrite(LPCWSTR cd, const std::vector<StockItem>& ar)
{
	std::wstring fnm = cd;
	fnm += L".bin";
	
	auto h = ::CreateFile(fnm.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,0,nullptr );

	if ( h != INVALID_HANDLE_VALUE )
	{
		XHandle x;
		x.magic = MAGICNUMBER;
		x.row_size = sizeof(StockItem);
		
		::WriteFile(h, (LPVOID)&x, sizeof(x), nullptr, nullptr);
		
		for(const StockItem& it : ar )
		{
			::WriteFile(h, &it, sizeof(StockItem), nullptr, nullptr);
		}

		::CloseHandle(h);
		return true;
	}
	return false;
}

bool FileRead(LPCWSTR cd, std::vector<StockItem>& ret)
{
	std::wstring fnm = cd;
	fnm += L".bin";
	
	auto h = ::CreateFile(fnm.c_str(),GENERIC_READ,0,nullptr,OPEN_EXISTING,0,nullptr );

	if ( h != INVALID_HANDLE_VALUE )
	{
		XHandle x;
		DWORD len;
		if ( ::ReadFile(h, (LPVOID)&x, sizeof(x), &len, nullptr) && len > 0 && x.magic == MAGICNUMBER)
		{
			ret.clear();

			StockItem si;
			
			while( ::ReadFile(h, (LPVOID)&si, sizeof(StockItem), &len, nullptr) && len > 0)
			{
				ret.push_back(si);
			}
			::CloseHandle(h);
			return true;
		}

		::CloseHandle(h);
	}
	return false;
}


static FILETIME _Date2Number(LPCWSTR date)
{
	FILETIME ft={};
	SYSTEMTIME st={};
	
	
	
	
	st.wYear = 2022;
	st.wMonth = 1;
	st.wDay = 1;
	
	SystemTimeToFileTime( &st, &ft );
	return ft;
}


};