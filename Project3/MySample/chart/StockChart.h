#pragma once

struct InternetInfo;

struct DataProviderInfo
{
	LPVOID info;

	std::wstring cd;
	std::wstring interval;

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

		static bool GetJson(LPCWSTR url, std::function<void(IStream*)> completefunc);
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
	int qnt;

	bool load( LPCSTR csv_row, ULONG len );
};

struct Candle
{
	CandleData raw;
	
	float vpos[5];
	void conv( std::function<float(float)> func) ; 
};

struct FigureTrimline
{
	float vy;
	money val;
};



class StockChart
{
	public :

		StockChart( V6::FRectF rc);
		
		bool Load(DataProvider& dp,DataProviderInfo& dpi);
		void LoadAsync(DataProvider* dp,DataProviderInfo* dpi, std::function<void(void)> complete);
	
		void Draw(ID2D1RenderTarget* cxt);
		
		void GenChartData(IStream* sm, std::vector<CandleData>& ar );
		void GenChartCandle(std::vector<CandleData>& ar, std::vector<Candle>& out, std::vector<FigureTrimline>& trimout);

		void SetSize(V6::FSizeF vsz);

		bool MouseMove(V6::FPointF pt);
		void WriteDataToDb(LPCWSTR cd, const std::vector<CandleData>& ar);

		LPCWSTR GetNowValue(money* val);
		bool IsEmpty(){ return xar_.empty(); }


		std::vector<Candle> xar_;
		std::vector<FigureTrimline> ar_trim_;

		V6::FRectF vrect_;
		ComPTR<IDWriteTextFormat> trim_textformat_;
		ComPTR<IDWriteTextFormat> money_textformat_;

		std::function<float(float)> v2money_;

		std::function<float(money m1)> money2vpos_;

		money mouse_place_value_;
		std::wstring cd_;
		std::string now_date_;

	protected :
		void DrawTrimline(ID2D1RenderTarget* cxt);
		void DrawCandle(ID2D1RenderTarget* cxt);
		void DrawParabolic(ID2D1RenderTarget* cxt);


		const char* SearchDate( float x );

};


struct PrimeStockDataItem
{
	std::wstring cd;
	money regularMarketPrice;

};

bool PrimeStockDataLoad( std::map<std::wstring,PrimeStockDataItem>& mapStockData);

