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


		for (int j = 0; j < 3; j++)
		{
			enemyData[i].AIAtk[j] = -999;
			enemyData[i].AIMove[j] = -999;
		}
		enemyData[i].AIappear = -999;//出現パターン
		enemyData[i].AILife = -999;

		//
		for(int j = 0; j < 3; j++)
		{
			enemyData[i].stsWaitToAtk[j] = 0.0f;//移動してから攻撃するまでの時間
			enemyData[i].stsWaitToMove[j] = 0.0f;//移動してから攻撃するまでの時間
		}
		enemyData[i].nowShot = 0;
		enemyData[i].maxShot = 0;//
	}
}