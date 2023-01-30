#include "pch.h"

#include <iostream>
#include <random>
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <strsafe.h>
#include <sstream>
#include "QLearning.h"

#define S 0
#define F 0
#define H -1
#define G 1

#define GOAL 1
#define HOLE 9
#define KABE  -1

//int map[] = 
//{ S,F,F,F,
//  F,H,F,H,
//  F,F,F,H,
//  H,F,F,G 
//};

//int map[] = {
// S, F, F, H, F, F, F, F,F, F, F, H, F, F, F, F,
// F, F, H, H, F, F, H, F,H, F, F, H, F, F, F, F,
// H, F, F, H, F, H, F, F,H, F, F, H, F, F, F, F,
// F, F, F, H, F, H, F, H,H, F, F, H, F, F, F, F,
// F, H, H, H, F, H, F, F,H, F, F, H, F, F, F, F,
// F, H, F, H, F, H, F, F,H, F, F, H, F, F, F, F,
// F, F, F, F, F, H, H, F,H, F, F, H, H, H, H, H,
// F, H, F, F, F, H, H, F,H, F, F, F, F, F, F, G
//};


int map[] = {
 S, H, H, F, H, F, H, F,
 F, H, H, F, H, F, F, F,
 F, H, F, F, H, F, H, F,
 F, H, H, F, H, F, H, F,
 F, H, F, F, F, F, H, F,
 F, H, F, H, F, F, H, F,
 F, F, F, H, F, H, H, F,
 F, H, F, H, F, H, F, G
};


#undef S
#undef F
#undef H
#undef G


float Random()
{
	static std::random_device seed_gen;
	static std::default_random_engine engine(seed_gen());
	static std::uniform_int_distribution<> dist(0, 100);
	return dist(engine) / 100.0f;
}

// V(s)=R(s) + ��max��T(s'| s,a)*V(s')

int goal_maze_ret_s_a_Q(Env& env,float epsilon, float eta, float gamma);

std::vector<std::wstring> rireki;


Env::Env()
{
	map_ = map;
	running_ = false;
	
	ZeroMemory(BufferQ,sizeof(BufferQ));

	clear();
}

void Env::clear()
{
	int j = 0;
	for(int r=0; r < FROZEN_LAKE_ROW_COUNT; r++ )
	{
		for(int c=0; c<FROZEN_LAKE_COL_COUNT; c++)
		{
			QTable[r][c].Q = &BufferQ[(FROZEN_LAKE_COL_COUNT*r+c)*4];

			QTable[r][c].rc = RC(r,c);

			for(int a=0; a < 4; a++ )
				QTable[r][c].Q[a] = (map_[j] == -1 ? 0 : 0.5f); //Random());

			j++;
		}
	}		
}


int main_QLearning(Env& env)
{
	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::uniform_int_distribution<> dist(0, 100); 
	int  story = 1;
	
	float eta = 0.2f;  //# �w�K��
	float gamma = 0.8f;// 0.8f;  //# ���Ԋ�����
	float epsilon =0.0f; // 0.0f; // 0:Q�w�K, <0: SRSA
	

	//std::wcout << L"Q�̏����l";
	//env.GetMaxQ_Render();


	int kk=0;
	while (story < 200)
	{		
		int cnt=goal_maze_ret_s_a_Q(env,epsilon, eta, gamma);
			
		epsilon = epsilon / 2.0f;
		story++;
	}

	//�Ō�Ɍ��肳�ꂽ�o�H��\��

	//std::wcout << L"\n";
	//std::wcout << L"Q�̍ŏI�l";
	//env.GetMaxQ_Render();

	return 0;
}

int get_action(Env& env,int state, float epsilon, int prv_action)
{
	int action = 0;
	if (Random() < epsilon)
	{
		int rnd = (int)(100.0f*Random()) % 3; // from 0 to 3
		action = rnd;// ���X�AQ�𖳎������ړ�������Ԃ�
	}
	else
	{
		RC rc = RC::GetRC(state);
		auto Qs = env.QTable[rc.row][rc.col].Q;


		//* ?? ������
		
		//int ac0[] = {0,1,3};
		//int ac1[] = {1,2,0};
		//int ac2[] = {2,1,3};
		//int ac3[] = {3,2,0};
		//int* kouho = ac0;
		//if (prv_action==0)
		//	kouho = ac0;
		//else if (prv_action==1)
		//	kouho = ac1;
		//else if (prv_action==2)
		//	kouho = ac2;
		//else if (prv_action==3)
		//	kouho = ac3;
		//
		//env.GetMaxQ(Qs, kouho,3, &action );

		int acc[] = {0,1,2,3};
		env.GetMaxQ(Qs, acc, 4, &action );
		
	}
	return action;
}



bool range( int xx, int s, int e )
{
	return ( s <= xx && xx <= e);
}

int get_s_next(Env& env,int* state, int action)
{
	RC rc =RC::GetRC(*state);
	
	int map_ = map[*state];

	if ( map_ == 0 )
	{
		if ( action == 0 && range(rc.col,1,FROZEN_LAKE_COL_COUNT-1)) // left
		{
			*state += -1;
			return 0;
		}
		if ( action == 1 && range(rc.row,1,FROZEN_LAKE_ROW_COUNT-1)) // top 1�s��
		{
			*state += -FROZEN_LAKE_COL_COUNT;
			return 0;
		}
		if ( action == 2 && range(rc.col,0,FROZEN_LAKE_COL_COUNT-2))// right
		{
			*state += 1;
			return 0; 
		}
		if ( action == 3 && range(rc.row,0,FROZEN_LAKE_ROW_COUNT-2)) // down 1�s��
		{
			*state += FROZEN_LAKE_COL_COUNT; 
			return 0;
		}
	}
	else if ( map_ == 1 )
		return GOAL;
	else if (map_ == -1)
		return HOLE;

	return KABE;
}

int goal_maze_ret_s_a_Q(Env& env,float epsilon, float eta, float gamma)
{
	int s = 0; // �ꏊ
	int a = 0; //�A�N�V����
	int s_next = 0;
	int a_next = 0;

	
	float reward = 0;
	int cnt = 1;


	while(1)
	{		
		int ss = s;
		int xa = get_s_next(env,&ss, a);


		if (xa == 0 || xa == KABE || xa == HOLE)
		{
			s_next = ss;
		}

		reward = (xa == GOAL ? 1.0f : 0.0f);

		a_next = get_action(env, s_next, epsilon, -1); // ���̏ꏊ�ł̎��̃A�N�V����
	
		// env.Q(s,a)�̕]���l(Q)���X�V����
		// eta: �w�K��, gamma:���Ԋ�����
		if (xa == GOAL)
		{
			auto xxx = reward - env.Q(s,a);


			env.Q(s,a) += eta * xxx; //(reward - env.Q(s,a));


			TRACE( L"goal [xxx=%f]\n", eta*xxx );
		}
		else
		{
			//env.Q(s_next) ��Qmax�l�̏ꍇ Q�w�K

			auto xxx = 0.0f;

			if ( xa == 0 )
				xxx = (gamma * env.Q(s_next,a_next) - env.Q(s,a));
			else if ( xa == KABE )
				xxx = (gamma * env.Q(s_next,a_next) - env.Q(s,a));
			else if ( xa == HOLE )
			{
				xxx = 0; 
				//_ASSERT( 0 == gamma * env.Q(s_next,a_next) - env.Q(s,a));
			}

			Sleep( env.speed_ );
			
			//�V����Q����l(s,a)�@���@�Â�Q����l(s,a) + �w�K��*[ ���Ԋ�����*�ړ����Q�f�[�^(s_next,a_next) - �Â�Q����l(s,a) ]
			//env.Q(s,a) += eta * (reward + gamma * env.Q(s_next,a_next) - env.Q(s,a));


			_ASSERT(reward==0);
			env.Q(s,a) += eta * (reward + xxx);
		}



		if (xa == GOAL)
		{
			// �ŒZ�ł̃S�[���ł͂Ȃ���������Ȃ��̂ł܂��Ď��s�����Aepisode��max�܂�		
				
			return cnt;
		}
		else if (xa == HOLE )
		{
			//�@hole�ɗ������̂�start�����蒼���AQ�l�͂��̂܂܈ێ�
			a = 0;
			a_next = 0;
			s_next = 0;
			cnt = 0;

			TRACE( L"restart\n" );

		}

		s = s_next;
		a = a_next;
		cnt++;
	}

	return cnt;

}
