#include "cocos2d.h"

#include "Stage/EnemyTable.h"
#include "Base/Unit.h"

USING_NS_CC;
using namespace TapGun;




void EnemyTable::InitAll()
{
	for(int i = 0; i < MAX_STANDBYPOS; i++)
	{
		//���g�p���W�ɂ�-999.0f�����Ă����܂�
		standbyPos[i] = Vec3(-999.0f, -999.0f, -999.0f);
	}

	for(int i = 0; i < MAX_TARGETPOS; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			//���g�p���W�ɂ�-999.0f�����Ă����܂�
			targetPos[i][j] = Vec3(-999.0f, -999.0f, -999.0f);
		}
	}


	for(int i = 0; i < 100; i++)
	{
		enemyData[i].kind = EKIND_TWIN;//�G�͌��݂��ׂăc�C���}�V���K���ŏ�����
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


		enemyData[i].AIappear = AI_APPEAR_NUM;//�o���p�^�[��
		enemyData[i].AIAtk = AI_ATK_NUM;
		enemyData[i].AIMove = AI_MOVE_NUM;
		enemyData[i].AILife = AI_LIFE_NUM;

		//
		for(int j = 0; j < 3; j++)
		{
			enemyData[i].stsAtkFrame[j] = 0.0f;//�U���Ԋu�i�b�j
		}

		enemyData[i].stsWaitFrame = 0.0f;//�o�����Ă���ړ����n�߂�܂ł̎���
		enemyData[i].nowShot = 0;
		enemyData[i].maxShot = 0;//
	}
}