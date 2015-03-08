#include "cocos2d.h"

#include "Stage/EnemyTable.h"
#include "Base/Unit.h"

USING_NS_CC;
using namespace TapGun;




void EnemyTable::InitAll()
{
	for(int i = 0; i < MAX_STANDBYPOS; i++)
	{
		//未使用座標には-999.0fを入れておきます
		standbyPos[i] = Vec3(-999.0f, -999.0f, -999.0f);
	}

	for(int i = 0; i < MAX_TARGETPOS; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			//未使用座標には-999.0fを入れておきます
			targetPos[i][j] = Vec3(-999.0f, -999.0f, -999.0f);
		}
	}


	for(int i = 0; i < 100; i++)
	{
		enemyData[i].kind = EKIND_TWIN;//敵は現在すべてツインマシンガンで初期化
		for(int j = 0; j < 3; j++)
		{
			enemyData[j].nextEnemiesNum[j] = -1;
		}
		enemyData[i].number = -1;
		enemyData[i].standbyPos = Vec3(-999.0f, -999.0f, -999.0f);
		for(int j = 0; j < 3; j++)
		{
			enemyData[i].targetPos[j] = Vec3(-999.0f, -999.0f, -999.0f);
		}
		enemyData[i].alive = FALSE;
		enemyData[i].finishFlag = FALSE;

		enemyData[i].hitpoint = 0;


		enemyData[i].AIappear = AI_APPEAR_NUM;//出現パターン
		enemyData[i].AIAtk = AI_ATK_NUM;
		enemyData[i].AIMove = AI_MOVE_NUM;
		enemyData[i].AILife = AI_LIFE_NUM;

		//
		for(int j = 0; j < 3; j++)
		{
			enemyData[i].stsAtkFrame[j] = 0.0f;//攻撃間隔（秒）
		}

		enemyData[i].stsWaitFrame = 0.0f;//出現してから移動し始めるまでの時間
		enemyData[i].nowShot = 0;
		enemyData[i].maxShot = 0;//
	}
}