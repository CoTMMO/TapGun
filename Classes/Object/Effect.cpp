#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "Effect.h"
#include "Sound.h"

#else

#include "Object/Effect.h"
#include "System/Sound.h"

#endif

USING_NS_CC;
using namespace std;
using namespace TapGun;

Effect::Effect()
{
	for( auto &p : playerEffect)
	{
		p = new EffectData;
		p -> billboard = BillBoard::create();
		p -> billboard -> retain();
		p -> node = Node::create();
		p -> node -> retain();
	}
	for( auto &p : enemyEffect)
	{
		p = new EffectData;
		p -> billboard = BillBoard::create();
		p -> billboard -> retain();
		p -> node = Node::create();
		p -> node -> retain();
	}
	
	playerMuzzle = new PlayerMuzzle;
	for( int i = 0; i < PlayerMuzzleAnime; i++)
	{
		char buf[64];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		sprintf( buf, "p_muzzle%02d", i + 1);
#else
		sprintf( buf, "P_Muzzle/p_muzzle%02d", i + 1);
#endif
		playerMuzzle -> sprite3D[i] = _Sprite3D::create( buf);
		playerMuzzle -> sprite3D[i] -> setVisible( false);
		playerMuzzle -> sprite3D[i] -> setPosition3D( Vec3( 0, 0, 0));
		playerMuzzle -> sprite3D[i] -> setRotation3D( Vec3( 90, 0, 180));
		playerMuzzle -> sprite3D[i] -> retain();
	}
	
	for( int i = 0; i < EnemyMuzzleCount; i++)
	{
		enemyMuzzle[i] = new EnemyMuzzle;
	}
}

Effect* Effect::getInstance( void)
{
	static Effect* P;
	if( !P) P = new Effect;
	return P;
}

void Effect::setPlayerHitEffect( Sprite3D* sprite3d)
{
	for( int i = 0; i < EnemyNum; i++)
	{
		if( playerEffect[i] -> flag == false)
		{
			auto cache = SpriteFrameCache::getInstance();
			
			auto animation = Animation::create();
			
			for( int i = 0; i < 8; i++)
			{
				char buf[256];
				sprintf( buf, "P_Hit%02d.png", i + 1);
				animation -> addSpriteFrame( cache -> getSpriteFrameByName( buf));
			}
			
			animation -> setDelayPerUnit( 0.05f);
			animation -> setRestoreOriginalFrame( true);
			
			auto action = Animate::create( animation);
			
			auto callfunc = CallFunc::create( [=](void)->void
											 {
												 playerEffect[i] -> billboard -> removeFromParent();
												 playerEffect[i] -> flag = false;
											 });
			
			auto sequence = Sequence::create( action, callfunc, NULL);
			
			enemyEffect[i] -> billboard -> runAction( sequence);
			
			enemyEffect[i] -> billboard -> setPosition3D( Vec3( 0.1, 1.5, 0.5));
			
			enemyEffect[i] -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));
			
			enemyEffect[i] -> billboard -> setScale( 0.006f);
			sprite3d -> addChild( playerEffect[i] -> billboard);
			playerEffect[i] -> flag = true;
			return;
		}
	}
}

void Effect::setPlayerHitEffect( Sprite3D* sprite3d, Vec3 vec)
{
	for( int i = 0; i < EnemyNum; i++)
	{
		if( playerEffect[i] -> flag == false)
		{
			auto cache = SpriteFrameCache::getInstance();
			
			auto animation = Animation::create();
			
			for( int i = 0; i < 8; i++)
			{
				char buf[256];
				sprintf( buf, "P_Hit%02d.png", i + 1);
				animation -> addSpriteFrame( cache -> getSpriteFrameByName( buf));
			}
			
			animation -> setDelayPerUnit( 0.05f);
			animation -> setRestoreOriginalFrame( true);
			
			auto action = Animate::create( animation);
			
			auto callfunc = CallFunc::create( [=](void)->void
									{
										playerEffect[i] -> billboard -> removeFromParent();
										playerEffect[i] -> flag = false;
									});
			
			auto sequence = Sequence::create( action, callfunc, NULL);
			
			enemyEffect[i] -> billboard -> runAction( sequence);
			
			enemyEffect[i] -> billboard -> setPosition3D( Vec3( 0.1, 1.5, 0.5));
			
			enemyEffect[i] -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));
			
			enemyEffect[i] -> billboard -> setScale( 0.006f);
			sprite3d -> addChild( playerEffect[i] -> billboard);
			playerEffect[i] -> flag = true;
			return;
		}
	}
}

void Effect::setEnemyHitEffect( Sprite3D* sprite3d)
{
	for( int i = 0; i < EnemyNum; i++)
	{
		if( enemyEffect[i] -> flag == false)
		{
			auto cache = SpriteFrameCache::getInstance();
			
			auto animation = Animation::create();
			
			for( int i = 0; i < 8; i++)
			{
				char buf[256];
				sprintf( buf, "E_Hit%02d.png", i + 1);
				animation -> addSpriteFrame( cache -> getSpriteFrameByName( buf));
			}
			
			animation -> setDelayPerUnit( 0.05f);
			animation -> setRestoreOriginalFrame( true);
			
			auto action = Animate::create( animation);
			
			auto callfunc = CallFunc::create( [=](void)->void
											 {
												 enemyEffect[i] -> flag = false;
												 enemyEffect[i] -> node -> removeFromParent();
												 enemyEffect[i] -> billboard -> removeFromParent();
											 });
			
			auto sequence = Sequence::create( action, callfunc, NULL);
			
			enemyEffect[i] -> billboard -> runAction( sequence);
			
			enemyEffect[i] -> billboard -> setPosition3D( Vec3( 0.1, 1.5, 0.5));
			
			enemyEffect[i] -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));
			
			enemyEffect[i] -> billboard -> setScale( 0.003f);
			
			enemyEffect[i] -> node -> addChild( enemyEffect[i] -> billboard);
			sprite3d -> addChild( enemyEffect[i] -> node);
			enemyEffect[i] -> flag = true;
			return;
		}
	}
}

void Effect::setEnemyHitEffect( Sprite3D* sprite3d, Vec3 vec)
{
	for( int i = 0; i < EnemyNum; i++)
	{
		if( enemyEffect[i] -> flag == false)
		{
			auto cache = SpriteFrameCache::getInstance();
			
			auto animation = Animation::create();
			
			for( int i = 0; i < 8; i++)
			{
				char buf[256];
				sprintf( buf, "E_Hit%02d.png", i + 1);
				animation -> addSpriteFrame( cache -> getSpriteFrameByName( buf));
			}
			
			animation -> setDelayPerUnit( 0.05f);
			animation -> setRestoreOriginalFrame( true);
			
			auto action = Animate::create( animation);
			
			auto callfunc = CallFunc::create( [=](void)->void
											 {
												 enemyEffect[i] -> flag = false;
												 enemyEffect[i] -> billboard -> removeFromParent();
											 });
			
			auto sequence = Sequence::create( action, callfunc, NULL);
			
			enemyEffect[i] -> billboard -> runAction( sequence);
			
			enemyEffect[i] -> billboard -> setPosition3D( vec);
			
			enemyEffect[i] -> node -> setRotation3D( Vec3( 0, -sprite3d -> getRotation3D().y, 0));
			
			enemyEffect[i] -> billboard -> setScale( 0.003f);
			sprite3d -> addChild( enemyEffect[i] -> billboard);
			enemyEffect[i] -> flag = true;
			return;
		}
	}
}

void Effect::setPlayerMuzzle( Sprite3D* parentData, const string& pointName)
{
	auto sound = Sound::getInstance();
	if( playerMuzzle -> shotFlag == true) { return; }
	playerMuzzle -> shotFlag = true;
	playerMuzzle -> count = 0;
	auto point = parentData -> getAttachNode( pointName);
	if( !point) { return; }
	
	for( int i = 0; i < PlayerMuzzleAnime; i++)
	{
		char buf[64];
//		playerMuzzle -> sprite3D[i] -> setVisible( false);
		point -> addChild( playerMuzzle -> sprite3D[i]);
		sound -> playSE( "Shot");
	}
	int a = 0;
}

void Effect::setEnemyMuzzle( Sprite3D* parentData, const string& pointName1, const string& pointName2)
{
	auto sound = Sound::getInstance();

	auto point1 = parentData -> getAttachNode( pointName1);
	if( !point1) { return; }
	auto point2 = parentData -> getAttachNode( pointName2);
	if( !point2) { return; }
	
	for( int i = 0; i < EnemyMuzzleCount; i++)
	{
		if( enemyMuzzle[i] -> shotFlag == true) { continue; }
		enemyMuzzle[i] -> shotFlag = true;
		enemyMuzzle[i] -> count = 0;
		
		for( int j = 0; j < EnemyMuzzleAnime; j++)
		{
			char buf[64];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			sprintf( buf, "e_muzzle%02d", j + 1);
#else
			sprintf( buf, "E_Muzzle/e_muzzle%02d", j + 1);
#endif
			enemyMuzzle[i] -> sprite3DR[j] = _Sprite3D::create( buf);
			enemyMuzzle[i] -> sprite3DR[j] -> setVisible( false);
			enemyMuzzle[i] -> sprite3DR[j] -> setPosition3D( Vec3( 0, 0, 0));
			enemyMuzzle[i] -> sprite3DR[j] -> setRotation3D( Vec3( 0, 270, 180));
			point1 -> addChild( enemyMuzzle[i] -> sprite3DR[j]);
			
			enemyMuzzle[i] -> sprite3DL[j] = _Sprite3D::create( buf);
			enemyMuzzle[i] -> sprite3DL[j] -> setVisible( false);
			enemyMuzzle[i] -> sprite3DL[j] -> setPosition3D( Vec3( 0, 0, 0));
			enemyMuzzle[i] -> sprite3DL[j] -> setRotation3D( Vec3( 0, 270, 180));
			point2 -> addChild( enemyMuzzle[i] -> sprite3DL[j]);
		}
		sound -> playSE( "Shot");
		return;
	}
}

void Effect::muzzleUpdate( void)
{
	if( playerMuzzle -> shotFlag)
	{
		if( playerMuzzle -> count == PlayerMuzzleAnime)
		{
			playerMuzzle -> shotFlag = false;
			playerMuzzle -> count = 0;
			for( auto &p : playerMuzzle -> sprite3D) { p -> removeFromParent(); }
			return;
		}
		
		switch( playerMuzzle -> count)
		{
		case 0:
			playerMuzzle -> sprite3D[0] -> setVisible( true);
			break;
		case 1:
			//playerMuzzle -> sprite3D[0] -> setVisible( false);
			playerMuzzle -> sprite3D[1] -> setVisible( true);
			break;
		case 2:
			//playerMuzzle -> sprite3D[1] -> setVisible( false);
			playerMuzzle -> sprite3D[2] -> setVisible( true);
			break;
		default:
			break;
		}
		playerMuzzle -> count++;
	}
	
	for( auto &p : enemyMuzzle)
	{
		if( p -> shotFlag)
		{
			if( p -> count == EnemyMuzzleAnime)
			{
				p -> shotFlag = false;
				p -> count = 0;
				for( auto &r : p -> sprite3DR) { r -> setVisible( false); }
				for( auto &l : p -> sprite3DL) { l -> setVisible( false); }
				return;
			}
			
			switch( p -> count)
			{
			case 0:
				p -> sprite3DR[0] -> setVisible( true);
					
				p -> sprite3DL[0] -> setVisible( true);
				break;
			case 1:
				p -> sprite3DR[0] -> setVisible( false);
				p -> sprite3DR[1] -> setVisible( true);
					
				p -> sprite3DL[0] -> setVisible( false);
				p -> sprite3DL[1] -> setVisible( true);
				break;
			default:
				break;
			}
			p -> count++;
		}
	}
}