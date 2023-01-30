#pragma once

#define FROZEN_LAKE_ROW_COUNT	8
#define FROZEN_LAKE_COL_COUNT	8


float Random();


struct RC
{
	RC():row(0),col(0){}
	RC(int r, int c):row(r),col(c){}

	int row;
	int col;

	static RC GetRC(int state)
	{
		int row = state / FROZEN_LAKE_COL_COUNT;
		int col = (state-row*FROZEN_LAKE_COL_COUNT);

		_ASSERT( 0 <= col && col < FROZEN_LAKE_COL_COUNT && row < FROZEN_LAKE_ROW_COUNT );
		return RC(row,col);
	}
};

class Env
{
	struct RCQ
	{
		RCQ(){}
		RC rc;
		float* Q;
	};

	public :
		RCQ QTable[FROZEN_LAKE_ROW_COUNT][FROZEN_LAKE_COL_COUNT]; // QTable[row][col]

		// Qは直列に並べた方が表示に便利
		float BufferQ[FROZEN_LAKE_COL_COUNT*FROZEN_LAKE_ROW_COUNT*4];

		int* map_;
		int speed_;
		bool running_;
	public :
		
		Env();
		
		void clear();
		

		float& Q(int s, int a)
		{
			auto rc = RC::GetRC(s);
			return QTable[rc.row][rc.col].Q[a];
		}

		float GetMaxQ(float* Qs, int* kouho, int kouho_cnt, int* action)
		{
			int k = kouho[0];
			float q = Qs[k];
			*action=k;
			for(int i=1; i < kouho_cnt;i++)
			{
				k = kouho[i];
				if ( q < Qs[k] )
				{
					*action = k;
					q = Qs[k];					
				}
			}
			return q;
		}
		

		//void GetMaxQ_Render()
		//{			
		//	// 横軸：action
		//	// 縦軸：state(pos)

		//	//std::wcout << L"\n";
		//	int j = 0;
		//	for(int r=0; r < FROZEN_LAKE_ROW_COUNT; r++ )
		//	{
		//		for(int c=0; c<FROZEN_LAKE_COL_COUNT; c++)
		//		{					
		//			WCHAR cb[256];

		//			int a = 0;
		//			int kouho[] = {0,1,2,3};
		//			float q = GetMaxQ(QTable[r][c].Q, kouho,4, &a);

		//			WCHAR ch[] = {L'左',L'上', L'右', L'下'};


		//			//StringCbPrintf(cb,256,L"%c[%d]:%f,", ch[a],j++, q);	
		//			StringCbPrintf(cb,256,L"%c:%f,", ch[a], q);	

		//			//std::wcout << cb;
		//		}
		//		//std::wcout << L"\n";
		//	}			
		//}
};