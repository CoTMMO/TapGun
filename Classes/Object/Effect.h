
#ifndef __EFFECT_H__
#define __EFFECT_H__

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Sprite3D.h"

#else

#include "Base/Sprite3D.h"

#endif

namespace TapGun
{
	class Effect
	{
	public:
		Effect( const Effect& p) = delete;
		Effect& operator=( const Effect& P) = delete;
		static Effect* getInstance( void);
		
		void setPlayerMuzzle( cocos2d::Sprite3D* parentData, const std::string& pointName);
		void setEnemyMuzzle( cocos2d::Sprite3D* parentData, const std::string& pointName1, const std::string& pointName2);
		
		void muzzleUpdate( void);
		
		void setPlayerHitEffect( cocos2d::Sprite3D* sprite3d);
		void setPlayerHitEffect( cocos2d::Sprite3D* sprite3d, cocos2d::Vec3 vec);
		void setEnemyHitEffect( cocos2d::Sprite3D* sprite3d);
		void setEnemyHitEffect( cocos2d::Sprite3D* sprite3d, cocos2d::Vec3 vec);

	private:
		enum EffectNum
		{
			PlayerNum =	1,
			EnemyNum = 32,
			
			PlayerMuzzleAnime = 3,
			EnemyMuzzleAnime = 2,
			EnemyMuzzleCount = 16,
		};
		
		struct EffectData
		{
			bool flag;
			cocos2d::BillBoard* billboard;
			cocos2d::Node* node;
		};
		
		struct PlayerMuzzle
		{
			bool shotFlag;
			int count;
			_Sprite3D* sprite3D[PlayerMuzzleAnime];
		};
		
		struct EnemyMuzzle
		{
			bool shotFlag;
			int count;
			_Sprite3D* sprite3DR[EnemyMuzzleAnime];
			_Sprite3D* sprite3DL[EnemyMuzzleAnime];
		};

		EffectData *playerEffect[PlayerNum];
		EffectData *enemyEffect[EnemyNum];
		PlayerMuzzle *playerMuzzle;
		EnemyMuzzle *enemyMuzzle[EnemyMuzzleCount];
		
		Effect();
	};
}

#endif // __EFFECT_H__
