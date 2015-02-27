
#ifndef __ENEMY_TABLE_H__
#define __ENEMY_TABLE_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Enemy.h"

#else

#include "Stage/Enemy.h"

#endif

namespace TapGun
{

#define MAX_STANDBYPOS 100
#define MAX_TARGETPOS 100

	class EnemyTable
	{
	public:
		Enemy enemyData[100];
		cocos2d::Vec3 standbyPos[MAX_STANDBYPOS];//
		cocos2d::Vec3 targetPos[MAX_TARGETPOS][3];//

		int finishNumber;//ìGÇ™ç≈ëÂâΩëÃèoÇƒÇ≠ÇÈÇ©
		void InitAll();
		
	private:

	};
}

#endif // __MUZZLE_FLASH_H__
