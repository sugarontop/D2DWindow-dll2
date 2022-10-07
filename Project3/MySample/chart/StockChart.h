#pragma once



struct DataProviderInfo
{
	LPVOID info;
};
struct DataProviderResult
{
	LPVOID info;
	bool result;

	ComPTR<IStream> out;
};

struct DataRpoviderOut
{
	ComPTR<IStream> sm;

};


class DataProvider
{
	public :
		DataProvider(){}

		std::stringstream sm_;

		virtual bool Connect(DataProviderInfo& info) = 0;
		virtual DataProviderResult LoadSolidData(DataProviderInfo& into) =0;
		virtual bool Convert(DataProviderResult& res, DataRpoviderOut* out)=0;
};


class InetDataProvider : public DataProvider
{
	public :
		InetDataProvider(){}

		virtual bool Connect(DataProviderInfo& info);
		virtual DataProviderResult LoadSolidData(DataProviderInfo& info);
		virtual bool Convert(DataProviderResult& res, DataRpoviderOut* out);
};

class FileDataProvider : public DataProvider
{
	public :
		FileDataProvider(){}

		virtual bool Connect(DataProviderInfo& info);
		virtual DataProviderResult LoadSolidData(DataProviderInfo& info);
		virtual bool Convert(DataProviderResult& res, DataRpoviderOut* out);
};

typedef float money;

struct CandleData
{
	char date[12];
	money m1,m2,m3,m4,m4ex;
	float qnt;

	bool load( LPCSTR csv_row, ULONG len );
};

struct Candle
{
	CandleData raw;
	
	float vpos[5];

	struct matrix
	{
		float _x22,_x32;
	};

	void conv( matrix& mat);
	
};




class StockChart
{
	public :
		
		bool Load(DataProvider& dp);
		
		void Draw(ID2D1DeviceContext* cxt);
		void GenChartData(IStream* sm, std::vector<CandleData>& ar );
		void GenChartCandle(std::vector<CandleData>& ar, Candle::matrix& mat, std::vector<Candle>& out);
		void TrimCandle(Candle::matrix& mat, std::vector<Candle>& ar);

		std::vector<Candle> xar_;
};