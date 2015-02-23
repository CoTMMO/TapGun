#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Enemy.h"

#else

#include "Stage/Enemy.h"

#endif

USING_NS_CC;
using namespace TapGun;




int Enemy::SetAI(int idle, int atk, int appear)
{
	if(AI_ATK_NUM <= atk || AI_APPEAR_NUM <= appear || AI_IDLE_NUM <= idle)
	{
		AIIdle = idle;
		AIAtk = atk;
		AIappear = appear;
		return FALSE;
	}
	else
	{
		AIIdle = -1;
		AIAtk = -1;
		AIappear = -1;
		return TRUE;
	}
}