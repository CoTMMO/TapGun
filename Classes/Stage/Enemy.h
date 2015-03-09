
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

#include "Base/Sprite3D.h"


namespace TapGun
{
//この定義は今後異なるヘッダファイルに移し替えます
#define FALSE -1
#define TRUE 1

	//エネミーの攻撃タイプ
	enum _ENEMY_ATK_
	{
		AI_ATK_NONE = 0,//攻撃モーションを行わない
		AI_ATK_FAKE = -1,//威嚇攻撃を行う
		AI_ATK_SSHOT = 7,//立ち撃ちを行う
		AI_ATK_SLIDE = 4,//サイドジャンプ射撃
		AI_ATK_ACRO = 5,//アクロバティック
	};

	//エネミーの出現方法
	enum _ENEMY_APPEAR_
	{
		AI_APPEAR_RUN = 1,//走りながら登場
		AI_APPEAR_WALK = 2,//歩き
		AI_APPEAR_SWALK = 3,//横歩き
		AI_APPEAR_SLIDE = 4,//サイドジャンプ
		AI_APPEAR_FJUMP = 6,//前ジャンプ
		AI_APPEAR_ACRO = 5,//アクロバティック
	};

	//エネミーの移動方法
	enum _ENEMY_MOVE_
	{
		AI_MOVE_NONE = 0,//動かない
		AI_MOVE_RUN = 1,//走りながら登場
		AI_MOVE_WALK = 2,//歩き
		AI_MOVE_SWALK = 3,//横歩き
		AI_MOVE_SLIDE = 4,//サイドジャンプ
		AI_MOVE_ACRO = 5,//アクロバティック
	};

	//エネミーのライフサイクル
	enum _ENEMY_LIFE_
	{
		AI_LIFE_ONCE = 11,//アクションは一度のみ
		AI_LIFE_SWITCH = 12,//2点間を移動する
		AI_LIFE_STOP = 13,//最終地点でとまる
	};

	class Enemy
	{
	public:
		cocos2d::Vec3 standbyPos;//
		cocos2d::Vec3 targetPos[3];

		int SetAI(int appear, int atk, int move, int life);
		void SetNextEnemy(int num1, int num2, int num3);
		void SetTargetPos(cocos2d::Vec3 pos[3]);
		int number;
		int kind;//敵の種類
		int alive;//生存チェック
		int nextEnemiesNum[3];//この敵が消えたときに、次に出てくる敵
		int finishFlag;//この敵が消えると（倒す、または逃げる）とウェーブ終了（TRUE/FALSE）

		int hitpoint;
		//
		int AIappear;//出現パターン
		int AIAtk;
		int AIMove;
		int AILife;

		int nowShot;//現在のショット回数
		int maxShot;//連射回数
		//float atkFrame;//攻撃間隔（秒）
		//float waitFrame;//出現してから移動し始めるまでの時間
		float sleepTime;//
		float stsAtkFrame[3];//攻撃間隔（秒）
		float stsWaitFrame;//出現してから移動し始めるまでの時間

	private:

	};
}

#endif // __ENEMY_H__
