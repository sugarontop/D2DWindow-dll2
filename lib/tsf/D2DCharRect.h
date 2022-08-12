#pragma once
namespace V6 {

/*
       start caret  end
0      zS    zC     zE            zEE 
+-------+--+--+--+--+--------------+------------------
 a| b| c| D| E/ F| G| h| i| j| k| l|
--------+--+--+--+--+--------------------------------
        0    rC     rE
--------+--+--+--+--+--------------------------------
        | view area |
--------+--+--+--+--+--------------------------------
*/

// single line caret �ʒu�v�Z
class SingleLineCaret
{
	public :
		SingleLineCaret( int zS, int zEE, int rE, int rC )
		{	
			rC = max(0,rC);
			zS = max(0,zS);

			xassert( zS <= zEE );
			if ( rC <= rE )
			{
				zS_ = zS;
				zEE_ = zEE;
				rE_ = rE;
				rC_ = rC;
			}
			else
			{
				zS_ = zS + rC - rE;
				zEE_ = zEE;
				rE_ = rE;
				rC_ = rE;
			}
			xassert( rC_ <= rE_ );
		}
		
		SingleLineCaret( LPCWSTR s,  int rE )
		{	
			zS_ = 0;
			zEE_ = (int)wcslen(s);
			rE_ = rE;
			rC_ = 0;
		}

		int CaretOff( int one )
		{
			_ASSERT( one == 1 || one == -1 || one == 0 );
			int rC = rC_ + one;
			
			if ( rC >= rE_ )
			{
				rC_ = rC - 3;				

				if ( zEE() - zC() < 4 )
				{
					rC_ = min( rE_, rC);
				}
				else
				{
					zS_ = zS_ + 3;
				}

			}
			else if ( rC <= 0 )
			{
				rC_ = rC + 3;

				if ( zC() < 4 )
				{
					rC_ = max(0, rC);
				}
				else
				{
					zS_ = zS_ - 3;
				}
			}
			else
				rC_ = rC;


			if ( zC() > zEE())
			{
				rC_ = zEE_-zS_;
			}
			else if ( zC() < 0 )
			{
				rC_ = 0;
			}

			zS_ = max(0, zS_ );

			return zS_;
		}


		// �����̐擪�I�t�Z�b�g�ʒu
		int zS(){ return zS_; }

		// �L�����b�g�̐�Έʒu
		int zC(){ return zS_+rC_; }

		//�\�������̍Ō�
		int zE(){ return zS_ + rE_; }

		// �L�����b�g�̑��Έʒu�ATextbox���ł̈ʒu
		int rC(){ return rC_; }

		// �\��������
		int rE(){ return rE_; }

		// �����̒���
		int zEE(){ return zEE_; }

	private :
		int zS_,zEE_;
		int rC_,rE_;

	public :
	

};

class CharsRectF
{
	public :
		CharsRectF();

		void Set(IDWriteTextLayout* tx, bool bSingle);

		CharsRectF(IDWriteTextLayout* tx, bool bSingle );
		~CharsRectF(){ Clear(); }

		const FRectF* Create( LPCWSTR str, const FSizeF&  sz, int slen, int* plen );
		
		void Clear();
		float LineHeight() const { return line_height_; }

		ComPTR<IDWriteTextLayout> GetTextLayout() const { return layout_; }
		const FRectF* GetTextRect() const { return rects_.get(); }
	private :
		std::shared_ptr<FRectF[]> rects_;
		ComPTR<IDWriteTextLayout> layout_;

		bool bSingleLine_;
		int cnt_;		
		float line_height_;		
};


class CreateTextLayout
{
	public :
		CreateTextLayout( IDWriteFactory* fc, LPCWSTR psz, UINT nCnt, IDWriteTextFormat* fmt, const FSizeF& sz, bool IsSibleLine )
		{
			fc_ = fc;
			psz_ = psz;
			len_ = nCnt;
			fmt_ = fmt;
			sz_ = sz;
			IsSibleLine_ = IsSibleLine;
		}

		int Create( int zStartPos, int zCaretPos, IDWriteTextLayout** layout);
	private :
		int CreateMultiTextLayout( int zStartPos, int zCaretPos, IDWriteTextLayout** layout);
		int CreateSingleTextLayout( int zStartPos, int zCaretPos, IDWriteTextLayout** layout);

		IDWriteFactory* fc_;
		LPCWSTR psz_;
		UINT len_;
		IDWriteTextFormat* fmt_;
		FSizeF sz_;
		bool IsSibleLine_;

};





};
