#include "cocos2d.h"

#include "Stage/Enemy.h"

USING_NS_CC;
using namespace TapGun;



void Enemy::SetNextEnemy(int num1, int num2, int num3)
{
	nextEnemiesNum[0] = num1;
	nextEnemiesNum[1] = num2;
	nextEnemiesNum[2] = num3;
}


void Enemy::SetTargetPos(Vec3 pos[3])
{
	for (int i = 0; i < 3; i++)
	{
		targetPos[i] = pos[i];
	}

}


int Enemy::SetAI(int appear, int atk, int move, int life)
{
	if (AI_ATK_NUM <= atk || AI_APPEAR_NUM <= appear || AI_MOVE_NUM <= move || AI_LIFE_NUM <= life)
	{
		AIappear = AI_APPEAR_NUM;
		AIAtk = AI_ATK_NUM;
		AIMove = AI_MOVE_NUM;
		AILife = AI_LIFE_NUM;
		return FALSE;
	}
	else
	{
		AIappear = appear;
		AIAtk = atk;
		AIMove = move;
		AILife = life;
		return TRUE;
	}
}