#pragma once



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
	
		void Draw(ID2D1DeviceContext* cxt);
		
		void GenChartData(IStream* sm, std::vector<CandleData>& ar );
		void GenChartCandle(std::vector<CandleData>& ar, std::vector<Candle>& out, std::vector<FigureTrimline>& trimout);

		void SetSize(V6::FSizeF vsz);

		bool MouseMove(V6::FPointF pt);
		void WriteDataToDb(LPCWSTR cd, const std::vector<CandleData>& ar);

		std::vector<Candle> xar_;
		std::vector<FigureTrimline> ar_trim_;

		V6::FRectF vrect_;
		ComPTR<IDWriteTextFormat> trim_textformat_;
		ComPTR<IDWriteTextFormat> money_textformat_;

		std::function<float(float)> v2money_;
		money mouse_place_value_;
		std::wstring cd_;

	protected :
		void DrawTrimline(ID2D1DeviceContext* cxt);

};