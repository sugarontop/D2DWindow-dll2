#pragma once

#include <windows.h>
#include <vector>

namespace MEMODB
{

struct StockItem
{
	enum class TYP { DAY=1,WEEK=2 };

	TYP typ;

	char date[10];
	float m1;
	float m2;
	float m3;
	float m4;
	UINT volume;
};


bool FileRead(LPCWSTR cd, std::vector<StockItem>& ret);

bool FileWrite(LPCWSTR cd, const std::vector<StockItem>& ret);

};

