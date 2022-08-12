#pragma once






//
// �`���̎w�蕔����ID2D1Bitmap�֕ϊ�
//
bool RenderTargetToBitmp(ID2D1RenderTarget* rt, D2D1_RECT_U rc, ID2D1Bitmap** out);


//
// Stream����Bitmap���Č�
//
//bool CreateBitampFromStream(ID2D1RenderTarget* pRenderTarget, IStream* sm, ID2D1Bitmap** nbmp);


bool IStreamToBitmap(ID2D1RenderTarget* rt, IStream* sm, ID2D1Bitmap** bmp);

//
// ������IWICBitmap�ɕϊ���Apng�Ȃǂ̃t�@�C���ɕϊ��B
//
bool SaveBitmapToFile(LPCWSTR fnm, ID2D1Bitmap* srcBitmap);

//
// �w���ʃT�C�Y�ŁA���������ID2D1RenderTarget���쐬
//
bool CreateMemoryRenderTarget( UINT width, UINT height, ID2D1RenderTarget** out);

//
// ID2D1Bitmap����IWICBitmap�֕ϊ�
//
bool BitmpaToIWICBitmap(ID2D1Bitmap* srcBitmap, IWICBitmap** out);


//
// Bitmap����Stream�֕ϊ�
//
bool BitmapToIStream(ID2D1Bitmap* pBitmap, IStream* out);


// ID2D1DeviceContext�Ȃ̂ŁA�g���ɂ���
bool BitmapSaveToFile(ID2D1DeviceContext* cxt, LPCWSTR fnm, ID2D1Image* bmp2);
